#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>

// Small application designed to remove ld.so.preload if it is detected.
// Remember to compile me with the -static flag in case any LD_PRELOAD malware is intercepting the important file IO functions.

// This won't work with vlany, since vlany's persistence will just result in ld.so.preload being rewritten.
// However, the location of the library can still be viewed,

int main(void)
{
    if(getuid() != 0) { printf("[-] Please run me as root.\n"); return -1; }

    char buf[2048];
    ssize_t rbytes;
    int fd = open("/etc/ld.so.preload", O_RDONLY);
    if(fd < 0)
    {
        printf("[-] ld.so.preload either truly does not exist, or a deeper kernel space hook is intercepting open()\n");
        printf("Probably safe to continue with installation.\n");
        return fd;
    }
        
    while((rbytes = read(fd, &buf, sizeof(buf))) > 0)
    {
        printf("ld.so.preload presence detected.\n");
        printf("Location of potentially malicious preloaded library:\n%s\n", buf);
        printf("REMEMBER THIS SO YOU CAN SNOOP AROUND THE LIBRARY AFTER INSTALLATION.\n");
        printf("Now removing ld.so.preload (LOL BYE)\n");
        int u = unlink("/etc/ld.so.preload");
        if(u < 0) { printf("Removing ld.so.preload failed.\nIt is recommended to ^C if you see this.\n"); return u; }
        printf("ld.so.preload should now be gone.\n");
        printf("It's now safe to continue with installation. :)\n");
    }

    close(fd);

    return 0;
}
