// This program will attempt to use libdl's functions to locate any possible modifications to libc functions via LD_PRELOAD attacks.
// If your malware can hide from all of these detection methods, then you officially have a secure piece of userland malware.
// You can add more symbols to check below. You don't really need that many.

// Hooking dlsym will stop dladdr_check from returning anything other than 0. You can also hook dladdr, and just make it segfault in certain conditions.
// Hiding from dlsym is strange. But it can be done. I promise. :p

// Hooking dlinfo is easier than hooking dlsym in my opinion. Hooking dlinfo will stop dlinfo_check from
// revealing your malware.

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <link.h>
#include <dlfcn.h>

// add your own libc symbols to check if you wish
static const char *symbols[] = {
    "rename", "renameat", "renameat2", "fread",
    "stat", "stat64", "fstat", "fstat64",
    "lstat", "lstat64", "__lxstat", "__lxstat64",
    "__fxstat", "__fxstat64", "__xstat", "__xstat64",
    "ptrace", "fwrite", "fwrite_unlocked", "fputs_unlocked",
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
    "listxattr", "llistxattr", "flistxattr",
    "getxattr", "lgetxattr", "fgetxattr", "setxattr",
    "lsetxattr", "fsetxattr", "removexattr", "lremovexattr",
    "fremovexattr", "getutent", "getutxent", "pututline",
    "getpwent", "syslog", "__syslog_chk", "socket",
    NULL
};

int dladdr_check(void)
{
    void *dls_handle;
    const char *symbol;
    int i = 0, hooked_funcs = 0;

    if(!(dls_handle = dlopen("libc.so.6", RTLD_LAZY))) exit(EXIT_FAILURE);

    #ifdef VERBOSE
        printf("[+] beginning dlsym/dladdr check.\n");
    #endif

    while((symbol = symbols[i++]))
    {
        #ifdef VERBOSE
            printf("[+] checking \033[1;32m%s\033[0m.\n", symbol);
        #endif

        void *real_symbol_addr, *curr_symbol_addr;
        real_symbol_addr = dlsym(dls_handle, symbol);
        curr_symbol_addr = dlsym(RTLD_NEXT, symbol);

        if(real_symbol_addr != curr_symbol_addr)
        {
            Dl_info real_nfo, curr_nfo;

            dladdr(real_symbol_addr, &real_nfo);
            dladdr(curr_symbol_addr, &curr_nfo);

            #ifdef VERBOSE
                printf("[-] function %s possibly \033[1;31mhijacked\033[0m / location of shared object file: %s\n", symbol, curr_nfo.dli_fname);
            #endif
            hooked_funcs++;
        }
    }

    dlclose(dls_handle);
    #ifdef VERBOSE
        printf("\n[+] dlsym/dladdr check finished.\n");
    #endif

    return hooked_funcs;
}

void dlinfo_check(void)
{
    struct link_map *lm;
    dlinfo(dlopen(NULL, RTLD_LAZY), 2, &lm);

    #ifdef VERBOSE
        printf("[+] beginning dlinfo check.\n");
    #endif

    while(lm != NULL)
    {
        if(strlen(lm->l_name) > 0) printf("%p %s\n", (void *)lm->l_addr, lm->l_name);
        lm = lm->l_next;
    }

    #ifdef VERBOSE
        printf("[+] dlinfo check finished.\n");
    #endif
}

int main(void)
{
    if(getenv("LD_PRELOAD")) printf("OK.. LD_PRELOAD is visible in the local environment variables.. little warning\n");
    if(access("/etc/ld.so.preload", F_OK) != -1) printf("OK.. /etc/ld.so.preload DOES definitely exist.. little warning\n");
    printf("[+] finished basic checks\n\n");

    dlinfo_check(); printf("\n");

    int hooked_funcs = dladdr_check();
    if(hooked_funcs > 0) printf("[!] the dladdr check revealed that there are %d possibly hooked functions. YOUR MALWARE SUUUUCKS.\n", hooked_funcs);
    if(hooked_funcs == 0) printf("[+] no modifications to any libc functions were found. no LD_PRELOAD malware loaded, or your malware is decent.\n");

    return 0;
}