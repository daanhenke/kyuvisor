#!/bin/sh

qemu-system-x86_64 -bios ./OVMF.fd -drive file=fat:rw:build/out -smp 3 -m 512 -vga std -enable-kvm -monitor stdio -cpu host # -d guest_errors 
