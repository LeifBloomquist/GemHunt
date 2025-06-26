// Screen constants and defines

#ifndef SCREEN_H
#define SCREEN_H

#if defined(__VIC20__)
#define WINDOW_LINE1 0x1E8A
#define WINDOW_LINE2 (WINDOW_LINE1 + 22 )
#define WINDOW_LINE3 (WINDOW_LINE1 + 44 )
#define WINDOW_LINE4 (WINDOW_LINE1 + 66 )
#define WINDOW_LINE5 (WINDOW_LINE1 + 88 )
#define WINDOW_LINE6 (WINDOW_LINE1 + 110)
#define WINDOW_LINE7 (WINDOW_LINE1 + 132)
#define WINDOW_LINE8 (WINDOW_LINE1 + 154)
#define WINDOW_LINE9 (WINDOW_LINE1 + 176)
#define NETWORK_CHAR 0x1FF9
#define NETWORK_CHAR_COLOUR 0x97F9
#define SCREEN_WIDTH  22
#define SCREEN_HEIGHT 23
#define COLOR_OFFSET (COLOR_RAM-0x1E00)
#endif

#if defined(__C64__)
#define WINDOW_LINE1 0x0400
#define WINDOW_LINE2 (WINDOW_LINE1 + 40 )
#define WINDOW_LINE3 (WINDOW_LINE1 + 80 )
#define WINDOW_LINE4 (WINDOW_LINE1 + 120)
#define WINDOW_LINE5 (WINDOW_LINE1 + 160)
#define WINDOW_LINE6 (WINDOW_LINE1 + 200)
#define WINDOW_LINE7 (WINDOW_LINE1 + 240)
#define WINDOW_LINE8 (WINDOW_LINE1 + 280)
#define WINDOW_LINE9 (WINDOW_LINE1 + 320)
#define NETWORK_CHAR 0x07E7
#define NETWORK_CHAR_COLOUR 0xDBE7
#define SCREEN_WIDTH  40
#define SCREEN_HEIGHT 25
#define COLOR_OFFSET (COLOR_RAM-0x0400)
#endif



#define CHAR_STATE1 126 // top-left
#define CHAR_STATE2 124 // top-right
#define CHAR_STATE3 108 // bottom-right
#define CHAR_STATE4 123 // bottom-left
#define CHAR_BORDER 160 // block
#define CHAR_DIAGSE 205 // "\"
#define CHAR_DIAGNE 206 // "/"

// Prototypes
void prepare_screen();

#endif /* SCREEN_H */
