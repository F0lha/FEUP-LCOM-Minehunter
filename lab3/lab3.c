#include <minix/syslib.h>
#include <minix/drivers.h>

#include "keyboard.h"
#include "test3.h"
#include "i8254.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);



int main(int argc, char **argv){
	/* Initialize service */

	sys_enable_iop(SELF);
	sef_startup();

	printf("lab3: The PC's Keyboard  \n");
	if ( argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}

}

static void print_usage(char *argv[]) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"kbd_test_scan<unsigned short asm>\" \n"
			"\t service run %s -args \"kbd_test_leds<unsigned short *toggle> \n"
			"\t service run %s -args \"kbd_test_timed_scan<unsigned short n>\" \n"
			,argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	char *str;
	long num;
	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "kbd_test_scan <unsigned ass> ", strlen("kbd_test_scan")) == 0) {
		if( argc != 3 ) {
			printf("Timer:wrong no of arguments for test of timer_test_config() \n");
			return 1;
		}
		unsigned long ass = parse_ulong(argv[2], 10);
		kbd_test_scan(ass);
		return 0;
	}
	else if (strncmp(argv[1], "kbd_test_leds<unsigned short *leds> ", strlen("kbd_test_leds")) == 0) {
		if( argc < 3 ) {
			printf("Keyboard:wrong no of arguments for test of kbd_test_leds() \n");
			return 1;
		}
		int x = sizeof(unsigned short);
		unsigned short *cenas =  malloc((argc-2)*x);
		//unsigned short *cenas_temp = cenas;
		int i = 0;
		for(i;i<argc-2;i++){

			unsigned short valor = parse_ulong(argv[2+i], 10);
			cenas[i] = valor;
		}
		kbd_test_leds(argc-2,cenas);
		return 0;
	}
	else if (strncmp(argv[1], "kbd_test_timed_scan<unsigned short n>", strlen("kbd_test_timed_scan")) == 0) {
			if( argc != 3 ) {
				printf("Timer:wrong no of arguments for test of timer_test_config() \n");
				return 1;
			}
			unsigned long n = parse_ulong(argv[2], 10);
			kbd_test_timed_scan(n);
			return 0;
		}
	else {
		printf("Keyboard:non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX )
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("Keyboard: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
