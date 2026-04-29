#ifndef MSDOS_CONIO_H
#define MSDOS_CONIO_H

#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "chardef.h"

extern int getch(void);
extern int kbhit(void);
extern unsigned inp(unsigned port);
extern unsigned outp(unsigned port, unsigned value);

void gotoxy(int x, int y);
void gotox(int x);
void gotoy(int y);
int  wherex(void);
int  wherey(void);

int  cputs(const char *s);
void cputc(char c);
void cputcxy(int x, int y, char c);
void cputsxy(int x, int y, const char *s);
void chlinexy(int x, int y, int length);

void clrscr(void);
char cgetc(void);
void cursor(uint8_t on);
void revers(uint8_t on);

void fill_rows(uint8_t top_row, uint8_t bottom_row, uint8_t attr);

uint8_t bwc_normal_attr(void);
void    switch_colorset(void);

void init_msdos(void);
void cleanup_msdos(void);

uint8_t doesclrscrafterexit(void);

#endif // MSDOS_CONIO_H
