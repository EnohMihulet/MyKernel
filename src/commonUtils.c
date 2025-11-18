#include "../include/commonUtils.h"

void reverse(char* str, u16 len) {
	u16 start = 0;
	u16 end = len - 1;
	while (start < end) {
		char temp = str[start];
		str[start++] = str[end];
		str[end--] = temp;
	}
}

void sint_to_s(s32 num, char* str) {
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

void uint_to_s(u32 num, char* str) {
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

void sll_to_s(s64 num, char* str) {
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

void ull_to_s(u64 num, char* str) {
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

void uint_to_hexs(u32 hex, char* str) {
	for (u8 i = 0; i < 8; i++) {
		u8 r = hex % 16;
		hex /= 16;
		str[i] = HEX_STRING[r];
	}

	str[8] = '\0';

	reverse(str, 8);
}

void ull_to_hexs(u64 hex, char* str) {
	for (u8 i = 0; i < 16; i++) {
		u8 r = hex % 16;
		hex /= 16;
		str[i] = HEX_STRING[r];
	}

	str[16] = '\0';

	reverse(str, 16);
}
