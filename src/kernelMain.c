#include "../include/bootInfo.h"
#include "../include/commonUART.h"


void kernelMain(const BootInfo* bootInfo) {

	uart_init_115200();

	uart_puts("Hello, MyKernel!\n");

	printBootInfo(bootInfo);
	
	for (;;) ;
}
