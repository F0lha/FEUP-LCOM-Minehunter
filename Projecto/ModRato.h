#ifndef __MODRATO_H
#define __MODRATO_H

#define BIT(n) (0x01<<(n))

#define STATUS_PORT 		0x64
#define OUT_BUF 			0x60
#define IN_BUF 				0x60
#define DELAY_US 			20000
#define TWO_BYTE_CODE 		0xE0
#define BREAK_CODE_ESC 		0x81
#define STAT_REG 			0x64
#define KBC_CMD_REG 		0x64
#define KBC_CMD_MOUSE		0xD4
#define ENABLE_PACKETS	 	0xF4
#define DSM 				0xF5
#define ENABLE_MOUSE		0xA8
#define STATUS_REQUEST		0XE9
#define ACK					0xFA
#define PAR_ERR         	BIT(7)
#define TO_ERR              BIT(6)
#define IBF                 BIT(1)
#define OBF                 BIT(0)
#define RESEND              0xFE
#define ERROR               0xFC
#define MOUSE_IRQ 			12
#define MOUSE_HOOK_ID		2

#define MOUSE_MULTIPLIER	1


typedef struct{
	int x,y;

	unsigned long packets[3];

    int leftButtonDown;

    int rightButtonDown;

    int leftButtonReleased;

    int rightButtonReleased;

}Rato;

static Rato* rato = 0;


Rato* getRato();

Rato* newRato();

int mouse_subscribe_int();

int mouse_unsubscribe_int();

int mouse_int_handler();

void updateMouse();

int first_byte(unsigned long mouse_byte);

void drawRato(int cor);

int enable_packets();







#endif
