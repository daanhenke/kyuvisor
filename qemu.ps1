# This is broken. Couldn't get vmx to be enabled in Win10 without disabling HyperV, look at qemu.sh for proper arguments
& "C:\Program Files\qemu\qemu-system-x86_64.exe" -L "C:\Program Files\qemu" -bios ".\OVMF.fd" -drive file=fat:rw:build/out -smp 3 -m 512 -vga std -monitor stdio
