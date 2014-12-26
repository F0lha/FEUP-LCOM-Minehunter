#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>

#include "test7.h"
#include "ModPortaSerie.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();

	printf("lab7: Porta \n");
	if ( argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}

}

static void print_usage(char *argv[]) {
printf("Usage: one of the following:\n"
"\t service run %s -args \"teste <unsigned char tx>\"\n"
"\t service run %s -args \"timer_test_square <unsigned long>\n"
"\t service run %s -args \"timer_test_int <unsigned long>\" \n"
,argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	char *str;
	long num;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "timer_test_config", strlen("timer_test_config")) == 0) {
		if( argc != 3 ) {
			printf("Timer:wrong no of arguments for test of timer_test_config() \n");
			return 1;
		}
		unsigned long tx = parse_ulong(argv[2], 10);
		char *cenas = "oil tudo bem.";
		ser_test_poll(COM1_ADDR,tx,8,1,-1,9600,12,&cenas);
		return 0;
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
		printf("video_txt: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
