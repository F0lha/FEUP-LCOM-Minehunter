#include <minix/drivers.h>
#include <minix/syslib.h>
#include "timer.h"
#include "i8254.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);



int main(int argc, char **argv){
/* Initialize service */

sef_startup();

printf("lab4: The PS/2 Mouse  \n");

if ( argc == 1 ) {
print_usage(argv);
return 0;
} else {
proc_args(argc, argv);
}

}

static void print_usage(char *argv[]) {
printf("Usage: one of the following:\n"
"\t service run %s -args \"timer_test_config <unsigned long>\" \n"
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
printf("timer_test_config()\n"); /* Actually, it was already invoked */
unsigned long timer = parse_ulong(argv[2], 10);
timer_test_config(timer);
return 0;
}
else if (strncmp(argv[1], "timer_test_square", strlen("timer_test_square")) == 0) {
if( argc != 3 ) {
printf("Timer:wrong no of arguments for test of timer_get_config() \n");
return 1;
}
unsigned long freq = parse_ulong(argv[2], 10);
printf("RAN  %lu\n",freq);
timer_test_square(freq);
return 0;
}
else if (strncmp(argv[1], "timer_test_int", strlen("timer_test_int")) == 0) {
if( argc != 3 ) {
printf("Timer:wrong no of arguments for test of timer_test_int() \n");
return 1;
}
unsigned long time = parse_ulong(argv[2], 10);
timer_test_int(time);
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
