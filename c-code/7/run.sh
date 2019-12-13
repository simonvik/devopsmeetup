#!/bin/bash -x

rm -f example
gcc main.c ../common/helpers.c -o example -lseccomp

(
	cd ../../
	mkdir -p rootfs/.oldroot
	mkdir -p rootfs/proc
	sudo  ./c-code/7/example |& less  +G
	sudo  strace -fF ./c-code/7/example |& less  +G
)

