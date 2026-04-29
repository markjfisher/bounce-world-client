#include <graph.h>
#include <i86.h>
#include <dos.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <conio.h>
#include "screen.h"
#include "data.h"

#define VGA_COLS       40
#define VGA_ROWS       25

// Background colors we cycle through (CGA palette indexes 0..6, skipping 7
// which is light gray and would clash with the reverse-video background).
//   0 = black, 1 = blue, 2 = green, 4 = red, 5 = magenta, 6 = brown
static const uint8_t bg_palette[] = { 0x00, 0x01, 0x02, 0x04, 0x05, 0x06 };
#define BG_PALETTE_SIZE (sizeof(bg_palette) / sizeof(bg_palette[0]))

static uint8_t bg_index = 0;

static int     cursor_x      = 0;
static int     cursor_y      = 0;
static uint8_t current_attr  = 0x0F;

static uint8_t normal_attr(void)
{
    return (uint8_t)((bg_palette[bg_index] << 4) | 0x0F);
}

static uint8_t reverse_attr(void)
{
    // Bright-white background (0xF0) with the current bg color as the fg.
    // Requires that the high attribute bit be set to "intensity" rather
    // than "blink" — see disable_blink() called from init_msdos().
    return (uint8_t)(0xF0 | bg_palette[bg_index]);
}

static void disable_blink(void)
{
    union REGS r;
    r.h.ah = 0x10;
    r.h.al = 0x03;
    r.h.bl = 0x00;  // 0 = disable blink, enable bright-bg attribute
    r.h.bh = 0x00;
    int86(0x10, &r, &r);
}

uint8_t bwc_normal_attr(void) { return normal_attr(); }

static uint8_t far *vram(void)
{
    return (uint8_t far *)MK_FP(0xB800, 0);
}

void gotoxy(int x, int y)
{
    union REGS r;
    r.h.ah = 0x02;
    r.h.bh = 0x00;
    r.h.dh = (uint8_t)y;
    r.h.dl = (uint8_t)x;
    int86(0x10, &r, &r);
    cursor_x = x;
    cursor_y = y;
}

int wherex(void) { return cursor_x; }
int wherey(void) { return cursor_y; }

void gotox(int x) { gotoxy(x, cursor_y); }
void gotoy(int y) { gotoxy(cursor_x, y); }

static void put_char_with_attr(char c)
{
    uint8_t far *v = vram();
    int pos;
    if (cursor_x < 0 || cursor_x >= VGA_COLS || cursor_y < 0 || cursor_y >= VGA_ROWS) {
        return;
    }
    pos = (cursor_y * VGA_COLS + cursor_x) * 2;
    v[pos]     = (uint8_t)c;
    v[pos + 1] = current_attr;
    cursor_x++;
    if (cursor_x >= VGA_COLS) {
        cursor_x = 0;
        if (cursor_y < VGA_ROWS - 1) cursor_y++;
    }
}

void cputc(char c) { put_char_with_attr(c); }

int cputs(const char *s)
{
    int n = 0;
    while (*s) { put_char_with_attr(*s++); ++n; }
    return n;
}

void cputcxy(int x, int y, char c)         { gotoxy(x, y); put_char_with_attr(c); }
void cputsxy(int x, int y, const char *s)  { gotoxy(x, y); cputs(s); }

void chlinexy(int x, int y, int length)
{
    gotoxy(x, y);
    while (length-- > 0) put_char_with_attr((char)CH_HLINE);
}

void fill_rows(uint8_t top_row, uint8_t bottom_row, uint8_t attr)
{
    uint8_t far *v = vram();
    uint16_t start = (uint16_t)top_row * VGA_COLS * 2;
    uint16_t end   = ((uint16_t)bottom_row + 1) * VGA_COLS * 2;
    uint16_t pos;
    for (pos = start; pos < end; pos += 2) {
        v[pos]     = 0x20;
        v[pos + 1] = attr;
    }
}

static void fill_screen(uint8_t attr)
{
    fill_rows(0, (uint8_t)(VGA_ROWS - 1), attr);
}

void clrscr(void)
{
    fill_screen(normal_attr());
    gotoxy(0, 0);
}

char cgetc(void)
{
    // Blocking: matches cc65's cgetc() semantics so common code (get_line,
    // app_errors, press_key) works the same way. Callers that don't want
    // to block guard with kbhit() first.
    int c = getch();
    if (c == 0) {
        c = getch();
        switch (c) {
            case 0x48: return (char)KEY_UP_ARROW;
            case 0x50: return (char)KEY_DOWN_ARROW;
            case 0x4B: return (char)KEY_LEFT_ARROW;
            case 0x4D: return (char)KEY_RIGHT_ARROW;
            case 0x8D: return (char)KEY_SHIFT_UP_ARROW;
            case 0x91: return (char)KEY_SHIFT_DOWN_ARROW;
            case 0x73: return (char)KEY_SHIFT_LEFT_ARROW;
            case 0x74: return (char)KEY_SHIFT_RIGHT_ARROW;
            case 0x47: return (char)KEY_CLEAR;
            default:   return 0;
        }
    }
    return (char)c;
}

void cursor(uint8_t on)
{
    union REGS r;
    r.h.ah = 0x01;
    r.h.bh = 0x00;
    if (on) { r.h.ch = 0x06; r.h.cl = 0x07; }
    else    { r.h.ch = 0x20; r.h.cl = 0x00; }
    int86(0x10, &r, &r);
}

void revers(uint8_t on)
{
    current_attr = on ? reverse_attr() : normal_attr();
}

void switch_colorset(void)
{
    bg_index = (uint8_t)((bg_index + 1) % BG_PALETTE_SIZE);
    current_attr = normal_attr();
    clrscr();
    info_display_count = 0;
}


static uint8_t saved_video_mode = 0x03;
static uint8_t saved_active_page = 0x00;

void init_msdos(void)
{
    union REGS r;
    setvbuf(stdout, NULL, _IONBF, 0);

    r.h.ah = 0x0F;
    int86(0x10, &r, &r);
    saved_video_mode  = r.h.al;
    saved_active_page = r.h.bh;

    _setvideomode(_TEXTC40);
    disable_blink();
    cursor(0);
    bg_index = 0;
    current_attr = normal_attr();
    clrscr();
}

void cleanup_msdos(void)
{
    union REGS r;
    cursor(1);

    r.h.ah = 0x00;
    r.h.al = saved_video_mode;
    int86(0x10, &r, &r);

    if (saved_active_page != 0) {
        r.h.ah = 0x05;
        r.h.al = saved_active_page;
        int86(0x10, &r, &r);
    }
}

uint8_t doesclrscrafterexit(void) { return 1; }
