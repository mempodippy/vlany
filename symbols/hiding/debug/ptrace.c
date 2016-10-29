long ptrace(void *request, pid_t pid, void *addr, void *data)
{
    #ifdef DEBUG
        printf("[vlany] ptrace() called\n");
    #endif

    if(!old_ptrace) old_ptrace = get_symbol(RTLD_NEXT, CPTRACE);

    #ifndef PTRACE_BUG
        return old_ptrace(request, pid, addr, data);
    #endif

    if(owned()) return old_ptrace(request, pid, addr, data);

    char *ptrace_bug_msg = strdup(PTRACE_BUG_MSG);
    xor(ptrace_bug_msg);
    printf("%s\n", ptrace_bug_msg);
    cleanup(ptrace_bug_msg, strlen(ptrace_bug_msg));
    exit(-1);
}
