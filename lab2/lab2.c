#include <minix/drivers.h>

#include "timer.h"
#include "i8254.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {
/* Initialize service */

sef_startup();

printf("lab2: Contador e Altifalante do PC \n");

if ( argc == 1 ) {
print_usage(argv);
return 0;
} else {
proc_args(argc, argv);
}
return 0;

}

static void print_usage(char *argv[]) {
printf("Usage: one of the following:\n"
"\t service run %s -args \"timer_test_config <unsigned long>\" \n"
"\t service run %s -args \"timer_get_config <unsigned long> <unsigned char> \n"
"\t service run %s -args \"timer_display_conf <unsigned char>\" \n"
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
unsigned long  time = *argv[2];
timer_test_config(time);
return 0;
} else if (strncmp(argv[1], "2 aula", strlen("timer_get_config")) == 0) {
if( argc != 4 ) {
printf("Timer:wrong no of arguments for test of timer_get_config() \n");
return 1;
}

} else if (strncmp(argv[1], "2 aula", strlen("timer_display_conf")) == 0) {
if( argc != 3 ) {
printf("Timer:wrong no of arguments for test of timer_display_conf() \n");
return 1;
}
}
else {
printf("Timer:non valid function \"%s\" to test\n", argv[1]);
return 1;
}
}
