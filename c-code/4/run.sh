#!/bin/bash -x

rm -f example
gcc main.c ../common/helpers.c -o example

(
	cd ../../
	mkdir -p rootfs/.oldroot
	mkdir -p rootfs/proc
	sudo  ./c-code/4/example | less
)

