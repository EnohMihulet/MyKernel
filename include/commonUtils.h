#pragma once
#include <stdarg.h>
#include "commonTypes.h"

static const char* HEX_STRING = "0123456789ABCDEF";

static inline void* memcpy(void* dst, const void* src, u64 n) {
	u8* d = (u8*)dst;
	const u8* s = (u8*)src;
	for (u64 i = 0; i < n; i++) {
	    d[i] = s[i];
	}
	return dst;
}

static inline void* memset(void* dst, s16 val, u64 n) {
	u8* d = (u8*)dst;
	for (u64 i = 0; i < n; i++) d[i] = val;
	return dst;
}

static inline u8 strncmp(char* str1, char* str2, u64 n) {
	char* s1 = str1;
	char* s2 = str2;

	for (u64 i = 0; i < n; i++) {
		if (*s1 != *s2 || *s1 == 0 || *s2 == 0) break;
		s1++;
		s2++;
	}

	if (*s1 == *s2) return 0;
	if (*s1 > *s2) return 1;
	return -1;
}

static inline bool isdigit(char c) {
	return (c >= '0' && c <= '9');
}

void reverse(char* str, u16 len);

void sint_to_s(s32 num, char* str);
void uint_to_s(u32 num, char* str);

void sll_to_s(s64 num, char* str);
void ull_to_s(u64 num, char* str);

void uint_to_hexs(u32 hex, char* str);
void ull_to_hexs(u64 hex, char* str);

