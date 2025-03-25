#!/bin/bash

SCRIPT_PATH=$(realpath $(dirname "$0"))

if [ -f "${SCRIPT_PATH}/build.cfg" ]; then
    source "${SCRIPT_PATH}/build.cfg"
else
    source "${SCRIPT_PATH}/build.cfg.default"
fi

source ${TOOLCHAIN}

MAKE="make ARCH=arm CROSS_COMPILE=aarch64-poky-linux- -j16"

extract_machine() {
    if [ -f "${DEPLOY_DIR}/machine_name.txt" ]; then
        cat "${DEPLOY_DIR}/machine_name.txt"
        return
    fi
    local filename=$(find ${DEPLOY_DIR}/ -type l -name 'u-boot-*.dtb' -printf %P)
    local machine_name=${filename#u-boot-}
    echo ${machine_name%.dtb}
}

extract_board_defconfig() {
    local machine_name=$(extract_machine)
    echo "${machine_name//-/_}_defconfig"
}

# Function to align a file by adding padding to make its size a multiple of `pad_size`
align_file() {
    binary_file="$1"
    pad_size="$2"

    # Get the current size of the file
    current_size=$(stat --format=%s "$binary_file")

    # Calculate padding needed to make the file size a multiple of `pad_size`
    padding_needed=$(expr $pad_size - \( $current_size % $pad_size \))

    # If no padding is needed (already aligned), exit early
    if [ "$padding_needed" -eq "$pad_size" ]; then
        return
    fi

    # Use dd to add zero padding (or any other byte pattern) to the file
    dd if=/dev/zero bs=1 count="$padding_needed" >> "$binary_file"
}

if [ -z "$BOARD" ]; then
    BOARD=$(extract_board_defconfig)
fi

# Require success of commands
set -e

# print each command executed
set -o xtrace

# Function to align a file by adding padding to make its size a multiple of `pad_size`
align_file() {
    binary_file="$1"
    pad_size="$2"

    # Get the current size of the file
    current_size=$(stat --format=%s "$binary_file")

    # Calculate padding needed to make the file size a multiple of `pad_size`
    padding_needed=$(expr $pad_size - \( $current_size % $pad_size \))

    # If no padding is needed (already aligned), exit early
    if [ "$padding_needed" -eq "$pad_size" ]; then
        return
    fi

    # Use dd to add zero padding (or any other byte pattern) to the file
    dd if=/dev/zero bs=1 count="$padding_needed" >> "$binary_file"
}

build(){
    $MAKE

    cp ${DEPLOY_DIR}/u-boot-tfa.its .
    cp ${DEPLOY_DIR}/bl31.bin .
    align_file u-boot-nodtb.bin 64
    align_file bl31.bin 64
    uboot-mkimage -f u-boot-tfa.its u-boot-tfa.itb
    uboot-mkimage -E -B 0x40 -F -k ${DEPLOY_DIR} -r u-boot-tfa.itb -K u-boot.dtb

    ${BSP_DIR}/meta-hailo-bsp/recipes-bsp/hailo-secureboot-scripts-native/files/hailo15_boot_image_sign.sh ${CC_DIR} ${DEPLOY_DIR}/customer.key u-boot.dtb devicetree u-boot.dtb.signed
    ${BSP_DIR}/meta-hailo-bsp/recipes-bsp/hailo-secureboot-scripts-native/files/hailo15_boot_image_sign.sh ${CC_DIR} ${DEPLOY_DIR}/customer.key spl/u-boot-spl.bin image u-boot-spl.bin
    cp u-boot.dtb.signed u-boot-spl.bin ${DEPLOY_DIR}
    cp spl/u-boot-spl ${DEPLOY_DIR}/u-boot-spl.elf
    cp u-boot-tfa.itb ${DEPLOY_DIR}
}

if [ $# -eq 0 ]
then
    $MAKE "${BOARD}"
    build
elif [ "$1" = "nodef" ]
then
    build
else
    $MAKE $1
fi

