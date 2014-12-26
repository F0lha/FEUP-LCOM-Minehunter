#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>

#include "test7.h"
#include "ModPortaSerie.h"

int main(int argc, char **argv) {

	sef_startup();

	ser_test_set(COM1_ADDR, 7, 2,  -1, 50);

	ser_test_conf(COM1_ADDR);
}
