#include <stdarg.h>
#include "commonTypes.h"

void print_s32(s32 v);

void print_u32(u32 v);

void print_s64(s64 v);

void print_u64(u64 v);

void print_hex32(u32 v);

void print_hex64(u64 v);

void kprintf(const char* fmt, ...);
