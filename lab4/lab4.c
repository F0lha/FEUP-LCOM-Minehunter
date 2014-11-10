#include <minix/drivers.h>
#include <minix/syslib.h>
#include "test4.h"
#include "test4.h"
#include "mouse.h"
#include "i8254.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {
	/* Initialize service */

	sef_startup();

	printf("lab4: The PS/2 Mouse  \n");

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}

}

static void print_usage(char *argv[]) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_packet<unsigned short cnt>\" \n"
			"\t service run %s -args \"test_async<unsigned short idle_time>\n"
			"\t service run %s -args \"test_config<void>\" \n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	char *str;
	long num;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("Mouse:wrong no of arguments for test of test_packet() \n");
			return 1;
		}
		printf("test_packet\n"); /* Actually, it was already invoked */
		unsigned short cnt = parse_ulong(argv[2], 10);
		test_packet(cnt);
		return 0;
	} else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf(
					"Mouse:wrong no of arguments for test of test_async() \n");
			return 1;
		}
		unsigned long time = parse_ulong(argv[2], 10);
		test_async(time);
		return 0;
	} else if (strncmp(argv[1], "test_config", strlen("test_config"))
			== 0) {
		if (argc != 2) {
			printf(
					"Mouse:wrong no of arguments for test of test_config() \n");
			return 1;
		}
		test_config();
		return 0;
	} else {
		printf("Timer:non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
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
