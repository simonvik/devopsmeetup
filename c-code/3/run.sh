#!/bin/bash -x

rm -f example
gcc main.c ../common/helpers.c -o example

(
	cd ../../rootfs
	sudo ../c-code/3/example | less
)

