#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>


#include "vbe.h"
#include "test5.h"
#include "video_gr.h"
#include "lmlib.h"



static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {
	/* Initialize service */

	sef_startup();

	printf("lab5: The PC's Video Card in Graphics Mode   \n");

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}


}

static void print_usage(char *argv[]) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_init<unsigned short mode, unsigned short delay>\" \n"
			"\t service run %s -args \"test_square<unsigned short x, unsigned short y, unsigned short size, unsigned long color>\n"
			"\t service run %s -args \"test_config<void>\" \n"
			"\t service run %s -args \"test_gesture<short length, unsigned short tolerance>\" \n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	char *str;
	long num;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("Mouse:wrong no of arguments for test of test_init() \n");
			return 1;
		}
		printf("test_init\n"); /* Actually, it was already invoked */
		unsigned short mode = parse_ulong(argv[2], 10);
		unsigned short delay = parse_ulong(argv[3], 10);
		test_init(mode,delay);
		return 0;
	} else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {
		if (argc != 6) {
			printf(
					"Mouse:wrong no of arguments for test of test_square() \n");
			return 1;
		}
		unsigned short x,y,size;
		unsigned long color;
		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		size = parse_ulong(argv[4], 10);
		color = parse_ulong(argv[5], 10);
		test_square(x,y,size,color);
		return 0;
	} else if (strncmp(argv[1], "test_config", strlen("test_config"))
			== 0) {
		if (argc != 2) {
			printf(
					"Mouse:wrong no of arguments for test of test_config() \n");
			return 1;
		}
		//test_config();
		return 0;
	} else if (strncmp(argv[1], "test_gesture", strlen("test_gesture"))
			== 0) {
		if (argc != 4) {
			printf(
					"Mouse:wrong no of arguments for test of test_gesture() \n");
			return 1;
		}
		unsigned long length = parse_ulong(argv[2], 10);
		unsigned long tolerance = parse_ulong(argv[3], 10);
		//test_gesture(length,tolerance);
		return 0;
	}
	else {
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
