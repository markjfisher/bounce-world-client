#ifndef BWC_COMPAT_H
#define BWC_COMPAT_H

// Character codes taken from apple2enh we need in the lowercase enabled platforms

#undef CH_HLINE
#undef CH_VLINE
#undef CH_ULCORNER
#undef CH_URCORNER
#undef CH_LLCORNER
#undef CH_LRCORNER
#undef CH_TTEE
#undef CH_BTEE
#undef CH_LTEE
#undef CH_RTEE
#undef CH_CROSS
#undef CH_DEL
#undef CH_CURS_UP
#undef CH_CURS_DOWN

#define CH_HLINE        0x5F
#define CH_VLINE        0xDF
#define CH_ULCORNER     0x5F
#define CH_URCORNER     0x20
#define CH_LLCORNER     0xD4
#define CH_LRCORNER     0xDF
#define CH_TTEE         0x5F
#define CH_BTEE         0xD4
#define CH_LTEE         0xD4
#define CH_RTEE         0xDF
#define CH_CROSS        0xD4

#define CH_DEL          0x7F
#define CH_CURS_UP      0x0B
#define CH_CURS_DOWN    0x0A

#endif // BWC_COMPAT_H