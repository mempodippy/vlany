void xor(char *p)
{
    #ifdef DEBUG
        printf("[vlany] xor() called\n");
    #endif

    int i, key=0xAC;
    for(i=0; i<strlen(p); i++) p[i] ^= key;
}

void cleanup(void *var, int len)
{
    #ifdef DEBUG
        printf("[vlany] cleanup() called\n");
    #endif

    memset(var, 0x00, len); free(var);
}
