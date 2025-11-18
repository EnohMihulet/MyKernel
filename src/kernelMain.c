#include "../include/bootInfo.h"
#include "../include/commonUART.h"
#include "../include/printUtils.h"
#include "../include/exceptionHandling.h"


void kernelMain(const BootInfo* bootInfo) {
	setupExceptionVectors();

	kprintf("Exception vectors setup successfully.\n");

	gicInit();
	timerInit1hz();
	enableIRQ();

	kprintf("Timer and GIC initialized. IRQs enabled.\n");

	uart_init_115200();
	kprintf("Hello MyKernel!\n");
	printBootInfo(bootInfo);
	
	for (;;) ;
}
