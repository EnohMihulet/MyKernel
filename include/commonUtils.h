#pragma once
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

static inline void reverse(char* str, u16 len) {
	u16 start = 0;
	u16 end = len - 1;
	while (start < end) {
		char temp = str[start];
		str[start++] = str[end];
		str[end--] = temp;
	}
}

static inline void sitos(s64 num, char* str) {
	u16 i = 0;
	u8 isNeg = 0;

	if (num == 0) {
		str[0] = '0';
		str[1] = '\0';
		return;
	}

	if (num < 0) {
		isNeg = 1;
		num = -num;
	}

	while (num != 0) {
		u8 r = num % 10;
		num /= 10;
		str[i++] = r + '0';
	}

	if (isNeg) str[i++] = '-';

	str[i] = '\0';

	reverse(str, i);
}

static inline void uitos(u64 num, char* str) {
	u16 i = 0;

	if (num == 0) {
		str[0] = '0';
		str[1] = '\0';
		return;
	}

	while (num != 0) {
		u8 r = num % 10;
		num /= 10;
		str[i++] = r + '0';
	}

	str[i] = '\0';

	reverse(str, i);
}

static inline void htos(u64 hex, char* str) {
	for (u8 i = 0; i < 16; i++) {
		u8 r = hex % 16;
		hex /= 16;
		str[i] = HEX_STRING[r];
	}
	str[16] = 'x';
	str[17] = '0';


	str[18] = '\0';

	reverse(str, 18);
}
