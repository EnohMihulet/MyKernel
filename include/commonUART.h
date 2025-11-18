#pragma once
#include <stdarg.h>
#include "commonTypes.h"
#include "commonUtils.h"

#define UART0_BASE 0x09000000u

// 32-bit MMIO registers
#define UART_DR      (*(volatile u32*)(UART0_BASE + 0x00))
#define UART_RSR_ECR (*(volatile u32*)(UART0_BASE + 0x04))
#define UART_FR      (*(volatile u32*)(UART0_BASE + 0x18))
#define UART_IBRD    (*(volatile u32*)(UART0_BASE + 0x24))
#define UART_FBRD    (*(volatile u32*)(UART0_BASE + 0x28))
#define UART_LCR_H   (*(volatile u32*)(UART0_BASE + 0x2C))
#define UART_CR      (*(volatile u32*)(UART0_BASE + 0x30))
#define UART_IFLS    (*(volatile u32*)(UART0_BASE + 0x34))
#define UART_IMSC    (*(volatile u32*)(UART0_BASE + 0x38))
#define UART_RIS     (*(volatile u32*)(UART0_BASE + 0x3C))
#define UART_MIS     (*(volatile u32*)(UART0_BASE + 0x40))
#define UART_ICR     (*(volatile u32*)(UART0_BASE + 0x44))

// FR bits
#define FR_TXFF      (1u << 5)  // TX FIFO full
#define FR_RXFE      (1u << 4)  // RX FIFO empty
#define FR_BUSY      (1u << 3)  // UART busy shifting out

// LCR_H bits
#define LCRH_BRK     (1u << 0)
#define LCRH_PEN     (1u << 1)
#define LCRH_EPS     (1u << 2)
#define LCRH_STP2    (1u << 3)
#define LCRH_FEN     (1u << 4)
#define LCRH_WLEN_8  (3u << 5)  // 8-bit word length

// CR bits
#define CR_UARTEN    (1u << 0)
#define CR_TXE       (1u << 8)
#define CR_RXE       (1u << 9)

static inline void uart_init_115200() {
	UART_CR = 0;
	while (UART_FR & FR_BUSY) { continue; }

	UART_RSR_ECR = 0;
	UART_ICR = 0x7FFu;

	UART_IBRD = 13u;
	UART_FBRD = 1u;

	UART_LCR_H = LCRH_FEN | LCRH_WLEN_8;

	UART_CR = CR_UARTEN | CR_TXE | CR_RXE;
}

static inline void uart_putc(char c) {
	while (UART_FR & FR_TXFF) { continue; }
	UART_DR = (u32)(u8)c;
}

static inline s32 uart_getc(s32 *out) {
	if (UART_FR & FR_RXFE) return 0;
	*out = (s32)(UART_DR & 0xFFu);
	return 1;
}

static inline void uart_puts(const char* s) {
	for (; *s; ++s) {
		if (*s == '\n') uart_putc('\r');
		uart_putc(*s);
	}
}
