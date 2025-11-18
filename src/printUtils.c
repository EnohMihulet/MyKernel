#include <stdarg.h>
#include "../include/printUtils.h"
#include "../include/commonUART.h"

void print_s32(s32 v) {
	char buff[16];
	sint_to_s(v, buff);
	uart_puts(buff);
}

void print_u32(u32 v) {
	char buff[16];
	uint_to_s(v, buff);
	uart_puts(buff);
}

void print_s64(s64 v) {
	char buff[24];
	sll_to_s(v, buff);
	uart_puts(buff);
}

void print_u64(u64 v) {
	char buff[24];
	ull_to_s(v, buff);
	uart_puts(buff);
}

void print_hex32(u32 v) {
	char buff[10];
	uint_to_hexs(v, buff);
	uart_puts(buff);
}

void print_hex64(u64 v) {
	char buff[20];
	ull_to_hexs(v, buff);
	uart_puts(buff);
}

void kprintf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	for (const char* p = fmt; *p; ++p) {
		if (*p != '%') { uart_putc(*p); continue; }

		if (*(p+1) == '%') { uart_putc('%'); ++p; continue; }

		enum { LEN_NONE, LEN_L, LEN_LL } len = LEN_NONE;
		++p;
		if (*p == 'l') {
			if (*(p+1) == 'l') { len = LEN_LL; p += 1; }
			else { len = LEN_L;  }
			++p;
		}

		switch (*p) {
		case 'c': {
			int c = va_arg(args, int);
			uart_putc((char)c);
			break;
		}
		case 's': {
			const char* s = va_arg(args, const char*);
			if (!s) s = "(null)";
			while (*s) { uart_putc(*s++); }
			break;
		}
		case 'd': case 'i': {
			if (len == LEN_NONE) { s32 v = va_arg(args, s32); print_s32(v); }
			else { s64 v = va_arg(args, s64); print_s64(v); }
			break;
		}
		case 'u': {
			if (len == LEN_NONE) { u32 v = va_arg(args, u32); print_u32(v); }
			else { u64 v = va_arg(args, u64); print_u64(v); }
			break;
		}
		case 'x': case 'X': {
			if (len == LEN_NONE) { u32 v = va_arg(args, u32); print_hex32(v); }
			else { u64 v = va_arg(args, u64); print_hex64(v); }
			break;
		}
		case 'p': {
			uintptr_t pv = (uintptr_t)va_arg(args, void*);
			print_hex64((u64)pv);
			break;
		}
		default:
			uart_putc('%');
			uart_putc(*p);
			break;
		}
	}

	va_end(args);
}
