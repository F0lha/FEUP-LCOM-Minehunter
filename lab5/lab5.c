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
			"\t service run %s -args \"test_line<unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color>\" \n"
			"\t service run %s -args \"test_xpm<unsigned short xi, unsigned short yi, char *xpm[]>\" \n",
			"\t service run %s -args \"int test_move<unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time>\" \n",
			argv[0], argv[0], argv[0], argv[0], argv[0]);
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
		unsigned short mode = parse_ulong(argv[2], 16);
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
	} else if (strncmp(argv[1], "test_line", strlen("test_line"))
			== 0) {
		if (argc != 7) {
			printf(
					"Mouse:wrong no of arguments for test of test_line() \n");
			return 1;
		}
		unsigned short x,y,xf,yf;
		unsigned long color;
		x = parse_ulong(argv[2], 10);
		y = parse_ulong(argv[3], 10);
		xf = parse_ulong(argv[4], 10);
		yf = parse_ulong(argv[5], 10);
		color = parse_ulong(argv[6], 10);
		test_line(x,y,xf,yf,color);
		return 0;
	} else if (strncmp(argv[1], "test_xpm", strlen("test_xpm"))
			== 0) {
		if (argc != 5) {
			printf(
					"Mouse:wrong no of arguments for test of test_gesture() \n");
			return 1;
		}
		unsigned long xi = parse_ulong(argv[2], 10);
		unsigned long yi = parse_ulong(argv[3], 10);
		char *cenas = argv[4];
		test_xpm(xi,yi,retXPM(cenas));
		return 0;
	}
	else if (strncmp(argv[1], "test_move", strlen("test_move"))
				== 0) {
			if (argc != 8) {
				printf(
						"Mouse:wrong no of arguments for test of test_move() \n");
				return 1;
			}
			unsigned long xi = parse_ulong(argv[2], 10);
			unsigned long yi = parse_ulong(argv[3], 10);
			char *cenas = argv[4];
			unsigned long hor = parse_ulong(argv[5], 10);
			unsigned long delta = parse_ulong(argv[6], 10);
			unsigned long time = parse_ulong(argv[7], 10);
			test_move(xi,yi,retXPM(cenas),hor,delta,time);
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
