/* 
    MIT License
    (c) Antonio de Haro, 2020

    See attached LICENSE file for more info.

    drivers/screen.c:
    Defines a way for the kernel to more easily print to the screen 

*/

#include <drivers/ports.h>
#include <drivers/screen.h>

/*
_________________________

    Private functions
_________________________

*/

/* Simple arithmetic calculations to convert 1d arrays to 2d */
#define OFFSET(col, row) (2 * (row * MAX_COLS + col))
#define OFFSET_ROW(offset) (offset / (2 * MAX_COLS))
#define OFFSET_COL(offset) ((offset - (OFFSET_ROW(offset) * 2 * MAX_COLS)) / 2)

/* The currrent color used to write to the screen */
static char current_color = COLOR(WHITE, BLACK);

/* Gets the current cursor's position */
static inline int get_cursor() {
    byte_out(VIDEO_CTRL, 14);
    int offset = byte_in(VIDEO_DATA) << 8;

    byte_out(VIDEO_CTRL, 15);
    offset += byte_in(VIDEO_DATA);

    return offset * 2;
}

/* Sets the current cursor's position */
static inline void set_cursor(int offset) {
    offset /= 2;

    byte_out(VIDEO_CTRL, 14);
    byte_out(VIDEO_DATA, (unsigned char)(offset >> 8));

    byte_out(VIDEO_CTRL, 15);
    byte_out(VIDEO_DATA, (unsigned char)(offset & 0xff));
}

/* Prints a single character and returns the offset to the next one */
static int charprint(char c, int col, int row) {
    unsigned char *video = (unsigned char *)VIDEO_ADDRESS;

    if (col >= MAX_COLS || row >= MAX_ROWS) {
        video[2 * (MAX_COLS) * (MAX_ROWS)-2] = 'E';
        video[2 * (MAX_COLS) * (MAX_ROWS)-1] = COLOR(RED, WHITE);
        return OFFSET(col, row);
    }

    int offset = OFFSET(col, row);

    if (c == '\n') {
        row = OFFSET_ROW(offset);
        offset = OFFSET(0, row + 1);
    } else {
        video[offset] = c;
        video[offset + 1] = current_color;
        offset += 2;
    }

    set_cursor(offset);
    return offset;
}

/*
_________________________

    Public Kernel API
_________________________

*/

/* Set the current color for writting to the screen */
void set_color(char c) {
    current_color = c;
}

/* Get the current color for writting to the screen */
char get_color() {
    return current_color;
}

/* Clears the screen with blank characters */
void clear() {
    char *screen = (char *)VIDEO_ADDRESS;

    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = COLOR(WHITE, BLACK);
    }

    set_cursor(OFFSET(0, 0));
}

/* 
   Prints a string on a specified location, if colum or
   row are negative, the character will be printed on 
   the cursor
*/
void print_at(char *message, int col, int row) {
    int offset;

    if (col >= 0 && row >= 0) {
        offset = OFFSET(col, row);
    } else {
        offset = get_cursor();
        row = OFFSET_ROW(offset);
        col = OFFSET_COL(offset);
    }

    for (int i = 0; message[i] != 0; i++) {
        offset = charprint(message[i], col, row);

        row = OFFSET_ROW(offset);
        col = OFFSET_COL(offset);
    }
}

/* Prints a string on the cursor's position */
void print(char *message) {
    print_at(message, -1, -1);
}
