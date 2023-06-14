#!/usr/bin/env bash

appName="energymetter"


### Color Functions ##
ESC=$(printf '\033') RESET="${ESC}[0m"
RED="${ESC}[31m" GREEN="${ESC}[32m" YELLOW="${ESC}[33m"
BLUE="${ESC}[34m" MAGENTA="${ESC}[35m" CYAN="${ESC}[36m"
greenprint() { printf "${GREEN}%s${RESET}\n" "$1"; }
blueprint() { printf "${BLUE}%s${RESET}\n" "$1"; }
redprint() { printf "${RED}%s${RESET}\n" "$1"; }
yellowprint() { printf "${YELLOW}%s${RESET}\n" "$1"; }
magentaprint() { printf "${MAGENTA}%s${RESET}\n" "$1"; }
cyanprint() { printf "${CYAN}%s${RESET}\n" "$1"; }
### Color Functions ##

setup()
{
    sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
    sudo apt install clang-format-12 -y
    sudo apt install minicom -y
}

init()
{

    # First Boot
    x="./pico-sdk/pico_sdk_init.cmake"

    if [ ! -f "$x" ]; then
        echo "$(yellowprint 'Not Started - Initializing Submodules')"
        startingPath=$PWD
        cd pico-sdk && git submodule update --init
        cd lib/tinyusb && git submodule update --init
        cd $startingPath
    fi
}

cleanFolder()
{
    echo "$(yellowprint 'Deleting all Build Folders')"
    rm -rf build
}

gitupdate()
{
    echo "$(yellowprint 'Git pull')"
    git pull origin master;

    cd pico-sdk && git checkout master && git pull origin master
    cd lib/tinyusb && git checkout master && git pull origin master
    cd ../../../

}

runBuild()
{
    if [ -d "./build" ]
    then
        echo "
            $(yellowprint 'Rebuild')
            "

    else
        echo "
            $(yellowprint 'Full Buils')
            "
        cmake -B build -S .
    fi

    # Get Number os CPU
    cpu_num=$(lscpu | grep "Core(s) per socket" | cut -d ':' -f2 | xargs)
    cpu_num=$((cpu_num * 2))
    # Build
    make -C build -j$cpu_num
}

copyToPico()
{
    echo "$(yellowprint 'Copy *.u2f to rasp')"

    # WINDOWS WSL
    if grep -qi microsoft /proc/version; then
        read -p "Insert $(greenprint 'Pico assigned Letter ') ex. d -> " l;
        #TODO confirm if binary file *u2f exist
        sudo mkdir -p /mnt/$l/
        sudo mount -t drvfs $l: /mnt/$l/

        echo "$(blueprint 'Copying to rasp') - Please wait..."
        cp build/$appName.uf2 /mnt/$l/

    # NATIVE LINUX
    else
        if [ -d "/media/$USER/RPI-RP2" ]; then
            cp build/$appName.uf2 /media/$USER/RPI-RP2
        else
            echo "$(redprint 'FAIL') Raspberry not connected or not in bootsel mode"
        fi
    fi
}

debug()
{
        searchRapsSerial=$(ls /dev/ttyACM0)

        if [ ! -z $searchRapsSerial ]; then
            clear;
            echo "$(yellowprint '* PICOCOM -> Press CTRL+A X to exit *')"
            sleep 2
            sudo picocom -b 115200 /dev/ttyACM0;
        else
            echo "$(redprint 'FAIL') Raspberry not ready to print"
        fi
}


# Clean
if [[ "$1" == clear || "$1" == clean ]]
then
    cleanFolder;

# Build
elif [[ "$1" == build ]]
then
    runBuild;
# Update
elif [[ "$1" == update ]] 
then
    cleanFolder;
    gitupdate;
# Init
elif [[ "$1" == init ]] 
then
    init;
# Setup
elif [[ "$1" == setup ]] 
then
    setup;

# CMAKE
elif [[ "$1" == copy ]]
then
    copyToPico;
# Clean , Build and Copy
elif [[ "$1" == buildandcopy ]]
then
    cleanFolder;
    runBuild;
    copyToPico;
# CMAKE
elif [[ "$1" == debug ]]
then
    debug;
else
    echo "$(blueprint '* Insert pretended command *')"
    exit
fi

echo "$(greenprint '* DONE *')"
