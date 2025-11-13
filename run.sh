#!/usr/bin/env bash
set -euo pipefail

KERNEL_ELF="$(cd "$(dirname "$0")" && pwd)/build/kernel.elf"

qemu-system-aarch64 \
	-M virt -cpu cortex-a57 -m 512 \
	-nographic -serial mon:stdio \
	-device loader,file="${KERNEL_ELF}",cpu-num=0 \
	-d unimp -no-reboot
