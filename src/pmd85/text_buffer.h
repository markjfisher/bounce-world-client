#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

// TODO: asm version
// void text_buffer_init(void);
// void cputc_tb(char c);
// void cputs_tb(const char *s);


extern void __FASTCALL__ gotox_tb(unsigned char x);
extern void __FASTCALL__ gotoy_tb(unsigned char y);
extern void __CALLEE__   gotoxy_tb(unsigned char x, unsigned char y);
extern void __FASTCALL__ update_tb_ptr(void);

extern void __FASTCALL__ putc_tb(char c);
extern void __FASTCALL__ putc_upper_tb(char c);
extern void __FASTCALL__ puts_tb(const char *s);

extern void __FASTCALL__ show_text_buffer(void);

// debugging stuff
extern char __FASTCALL__ *get_tb_buffer(void);
extern char __FASTCALL__ *get_tb_row_flags(void);
// void debug_buffer(void);

#endif // TEXT_BUFFER_H
