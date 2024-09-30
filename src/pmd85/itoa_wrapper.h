#ifndef PMD85_ITOA_H
#define PMD85_ITOA_H

#undef itoa
#define itoa(n ,s, b) uint8_to_a(n, s, b)

extern void __CALLEE__  uint8_to_a(unsigned char n, unsigned char *dst, unsigned char base);
extern void __CALLEE__  uint8_to_a10(unsigned char n, unsigned char *dst);
extern void __CALLEE__  uint8_to_a16(unsigned char n, unsigned char *dst);


#endif // PMD85_ITOA_H
