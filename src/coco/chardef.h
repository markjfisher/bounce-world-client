
#ifndef CHARDEF_H
#define CHARDEF_H

#ifdef COCO2_BUILD
#define CH_HLINE        131
#define CH_HLINE_T      131
#define CH_HLINE_B      140
#define CH_VLINE        133
#define CH_VLINE_L      133
#define CH_VLINE_R      138
#define CH_ULCORNER     129
#define CH_URCORNER     130
#define CH_LLCORNER     132
#define CH_LRCORNER     136
#define CH_RTEE         137
#define CH_LTEE         134
#define CH_TTEE         131
#define CH_BTEE         140
#define CH_CROSS        128
#else
#define CH_HLINE        61
#define CH_HLINE_T      61
#define CH_HLINE_B      61
#define CH_VLINE        58
#define CH_VLINE_L      58
#define CH_VLINE_R      58
#define CH_ULCORNER     43
#define CH_URCORNER     43
#define CH_LLCORNER     43
#define CH_LRCORNER     43
#define CH_RTEE         43
#define CH_LTEE         43
#define CH_TTEE         43
#define CH_BTEE         43
#define CH_CROSS        43
#endif

#define CH_CURS_LEFT    8
#define CH_DEL          127
#define CH_ENTER        13

#endif // CHARDEF_H
