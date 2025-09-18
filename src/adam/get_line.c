#include "get_line.h"
#include <eos.h>
#include "cursor.h"
#include "conio_helpers.h"


#define KEY_BACKSPACE    0x08
#define KEY_TAB          0x09
#define KEY_RETURN       0x0D
#define KEY_ESCAPE       0x1B
#define KEY_SPACE        0x20

void initPlatformKeyboardInput(void)
{
    eos_start_read_keyboard();
}

int getPlatformKey()
{
    int ch;
    ch = eos_end_read_keyboard();
    if (ch>1) {
      eos_start_read_keyboard();
    }
    else //don't worry about negative returns, set to zero
        ch=0;

    return ch;

}

void get_line(char* buf, uint8_t max_len)
{
    int pos = 0;
    int key;
    int x;
    int y;

    x = wherex();
    y = wherey();

    //get a line of input from the console.
    cursor(true);

    gotoxy(x,y);
    cursor_pos(x,y);

    while (true)
    {
        key = getPlatformKey();
        //   smartkeys_sound_play(SOUND_KEY_PRESS);
        if (key == KEY_RETURN)
        {
            cursor(false);
            break;
        }
        else if (key == KEY_BACKSPACE)
        {
            if (pos > 0)
            {
                pos--;
                x--;
                buf--;
                *buf=0x00;
                putch(KEY_BACKSPACE);
                putch(KEY_SPACE);
                putch(KEY_BACKSPACE);
                cursor_pos(x,y);
            }
        }
        else if (key > 0x1F && key < 0x7F) // Printable characters
        {
            if (pos < max_len)
            {
                pos++;
                x++;
                *buf=key;
                buf++;
                putch(key);
                cursor_pos(x,y);
            }
        }
    }
}

