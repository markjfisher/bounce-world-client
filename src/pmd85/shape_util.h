#ifndef SHAPE_UTIL_H
#define SHAPE_UTIL_H

extern void __CALLEE__  blit_shape(char *shape_data, char w, char h, char stn);
extern void __CALLEE__  clear_shape(char *shape_data, char w, char h, char stn);

extern void __CALLEE__  clear_rect_tb(char w, char h);
extern void __CALLEE__  print_shape_tb(char *shape_data, char w, char h, char stn);

extern void __CALLEE__ shape_to_buffer(uint8_t shape_id, int8_t center_x, int8_t center_y);
extern char __FASTCALL__ *get_shape_data(void);

#endif // SHAPE_UTIL_H