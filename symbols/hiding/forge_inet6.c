FILE *forge_inet6(const char *filename)
{
    #ifdef DEBUG
        printf("[vlany] forge_inet6() called\n");
    #endif

    HOOK(old_fopen, CFOPEN);

    FILE *tmp, *pnt = old_fopen(filename, "r");

    if((tmp = tmpfile()) == NULL) return pnt;

    char line[1024], sum[128], interface[128];
    int s1, s2, s3, s4;

    while(fgets(line, sizeof(line), pnt) != NULL)
    {
        sscanf(line, "%s %d %d %d %d %s", sum, &s1, &s2, &s3, &s4, interface);
        fputs(line, tmp);
    }

    fclose(pnt);
    if(tmp != NULL) fseek(tmp, 0, SEEK_SET);
    return tmp;
}
