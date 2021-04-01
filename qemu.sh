#!/bin/sh

qemu-system-x86_64 -bios ./OVMF.fd -drive file=fat:rw:build/out -smp 3 -m 512 -vga std -monitor stdio -enable-kvm -cpu host,vmx=on -d in_asm
