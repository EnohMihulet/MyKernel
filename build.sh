#!/usr/bin/env bash
set -euo pipefail

CC=clang
LD=ld.lld
OBJDUMP=llvm-objdump

TARGET=aarch64-none-elf

AFLAGS=(-target "${TARGET}")
CFLAGS=(
	-target "${TARGET}"
	-ffreestanding
	-fno-stack-protector
	-fno-builtin
	-fno-pic
	-nostdlib
	-Wall -Wextra -Wno-unused-function
)

SRC_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${SRC_DIR}/build"
OBJ_DIR="${BUILD_DIR}/src"
mkdir -p "${OBJ_DIR}"

LDFLAGS=(
  -T "${SRC_DIR}/kernel.ld"
  --gc-sections
  -Map "${BUILD_DIR}/kernel.map"
)

shopt -s nullglob
ASM_SOURCES=("${SRC_DIR}"/src/*.s "${SRC_DIR}"/src/*.S)
C_SOURCES=("${SRC_DIR}"/src/*.c)
shopt -u nullglob

OBJS=()

for f in "${ASM_SOURCES[@]:-}"; do
	base="$(basename "$f")"
	out="${OBJ_DIR}/${base%.*}.o"
	echo "Assembling ${base} -> ${out}"
	"${CC}" "${AFLAGS[@]}" -c "$f" -o "$out"
	OBJS+=("$out")
done

for f in "${C_SOURCES[@]:-}"; do
	base="$(basename "$f")"
	out="${OBJ_DIR}/${base%.*}.o"
	echo "Compiling ${base} -> ${out}"
	"${CC}" "${CFLAGS[@]}" -c "$f" -o "$out"
	OBJS+=("$out")
done

echo "Linking -> ${BUILD_DIR}/kernel.elf"
"${LD}" "${LDFLAGS[@]}" -o "${BUILD_DIR}/kernel.elf" "${OBJS[@]}"

echo "Done. Artifacts:"
ls -l "${BUILD_DIR}"

