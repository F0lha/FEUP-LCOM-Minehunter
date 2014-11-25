# Makefile for the Final Project

COMPILER_TYPE= gnu

CC=gcc

PROG=	Projecto
SRCS=	Jogo.c ModGrafico.c ModMenu.c ModPortaSerie.c ModRato.c ModRTC.c ModTeclado.c ModTimer.c ModWindow.c 

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
