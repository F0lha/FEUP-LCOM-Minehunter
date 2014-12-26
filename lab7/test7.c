#include <minix/syslib.h>
#include <minix/drivers.h>


#define SER_LSR 5
#define SER_DATA 0
#define SER_TX_RDY (1<<5)


#include "test7.h"
#include "ModPortaSerie.h"


int ser_test_conf(unsigned short base_addr) {


	unsigned long Divisor_Latch, dll = 0,dlm = 0;
	changeDLAB(base_addr, 1);

	sys_inb(base_addr + DLL, &dll);
	Divisor_Latch |= (dll & 0xFF);
	sys_inb(base_addr + DLM, &dlm);
	Divisor_Latch = (dlm & 0xFF) << 8;

	changeDLAB(base_addr, 0);
	unsigned long bit_rate = UART_FREQ/Divisor_Latch;


	unsigned long line_control = 0;

	sys_inb(base_addr + LCR, &line_control);//// LineControl


	unsigned long Interrupt_Enable;
	sys_inb(base_addr + IER, &Interrupt_Enable);


	printf("Bit-rate: %d\n", bit_rate);

	///LCR
	printf("Word length: ");
	int num = 5;
	if (BIT(0) & line_control) num += 1;
	if (BIT(1) & line_control) num += 2;
	printf("%d\n", num);

	if((line_control & BIT(2)) == 0)
	printf("N stop bits: %d\n", 1);
	else printf("N stop bits: %d\n", 2);

	printf("Parity: ");
	if ((line_control & BIT(3)) == 0)
		printf("None\n");
	else {
		if ((line_control & BIT(5)) == 0) {
			if ((line_control & BIT(4)) == 0)
				printf("Odd\n");
			else
				printf("Even\n");
		} else {
			if ((line_control & BIT(4)) == 0)
				printf("1\n");
			else printf("0\n");
		}
	}
	printf("Set Break Enable : %d\n",line_control & BIT(6));
	///IE

	if (Interrupt_Enable & BIT(0)) printf("Received Data Interrupt : Enabled\n");
	else printf("Received Data Interrupt : Disabled\n");

	if (Interrupt_Enable & BIT(1)) printf("Transmitter Empty Interrupt : Enabled\n");
	else printf("Transmitter Empty Interrupt : Disabled\n");

	if (Interrupt_Enable & BIT(2)) printf("Receiver Line Interrupt : Enabled\n");
	else printf("Receiver Line Interrupt : Disabled\n");

}

int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	           long parity, unsigned long rate) { 
	setPorta(base_addr,bits,stop, parity, rate);
}

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char c) {

	ser_test_set(base_addr,bits,stop,parity,rate);
	if(tx == 0)  ///receiver
	{

			char temp;
			getChar(base_addr,&temp);
			printf("%c",temp);

	}
	else{ /// sender

		unsigned long lsr;

		sys_inb(base_addr + SER_LSR, &lsr);

		while( !(lsr & SER_TX_RDY) ) {
		sys_inb(base_addr + SER_LSR, &lsr);
		}

		sys_outb(base_addr+SER_DATA, c);
	}


}

int ser_test_int(/* details to be provided */) { 
    /* To be completed */
}

int ser_test_fifo(/* details to be provided */) {
    /* To be completed */
}
