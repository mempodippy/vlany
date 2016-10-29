// This very small application is able to differentiate between a normal Linux environment and an LXC environment.
// This is because output from ps does not correlate consistently with results from sysinfo().
// By comparing the output of both ps and sysinfo(), we can differentiate between a regular environment and an LXC environment.

#include <stdio.h>
#include <sys/sysinfo.h>

int main(int argc, char *argv[])
{
    int count;
    FILE *ps = popen("ps axHo lwp,cmd|wc -l", "r");
    fscanf(ps, "%d", &count);
    pclose(ps);

    struct sysinfo si;
    sysinfo(&si);
    if(si.procs - 200 > count && argc == 1) { printf("[!] definitely in LXC\n"); return 1; }

    printf("[+] either not in LXC or an LXC environment was not detected\n"); return 0;
}
