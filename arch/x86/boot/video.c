/**
 * A simple video driver
 * Author: Renê de Souza Pinto
 */

#include "video.h"


static unsigned char *videomem = (unsigned char *)VIDEO_MEM_ADDR;
static unsigned char px        = 0;
static unsigned char py        = 0;
static unsigned char c_attr    = LIGHT_GRAY;


/**
 * clrscr
 * Clear screen
 */
void clrscr(void)
{
	unsigned int i;

	for(i=0; i<VIDEO_MEM_SIZE; i+=2) {
		videomem[i]   = ' ';
		videomem[i+1] = BLACK;
	}
}


/**
 * gotoxy
 * Move the cursor to x and y position
 *
 * @param x Column cursor position
 * @param y Line cursor position
 */
void gotoxy(unsigned char x, unsigned char y)
{
	if(x <= VIDEO_COLS && y <= VIDEO_ROWS) {
		px = x;
		py = y;
	}
	return;
}


/**
 * setattr
 * Set character attribute
 *
 * @param attr Character attribute (see video.h)
 */
void setattr(unsigned char attr)
{
	c_attr = attr;
	return;
}


/**
 * writechar
 * Write a character on screen
 *
 * @param ch Character
 * @param attr Character attribute (color)
 */
void writechar(char ch, unsigned char attr)
{
	unsigned int pos;

	pos = ((VIDEO_COLS * py) + px) * 2;

	/* Update cursor position */
	if(px >= VIDEO_COLS) {
		px = 0;
		py++;

		if(py >= VIDEO_ROWS) {
			scroll_screen();
			py = VIDEO_ROWS-1;
		}
	} else {
		px++;
	}

	videomem[pos]   = ch;
	videomem[pos+1] = attr;
}


/**
 * writechar_xy
 * Write a character on screen in x, y position
 *
 * @param ch Character
 * @param attr Character attribute (color)
 * @param x Column cursor position
 * @param y Line cursor position
 */
void writechar_xy(char ch, unsigned char attr, unsigned char x, unsigned char y)
{
	gotoxy(x, y);
	writechar(ch, attr);
}


/**
 * kprint
 * Write a string on screen
 *
 * @param str String
 */
void kprint(char *str)
{
	unsigned int i = 0;
	char c;

	while((c = str[i]) != '\0') {
		switch(c) {
			case '\n':
						if(py <= VIDEO_ROWS) {
								py++;
								if(py >= VIDEO_ROWS) {
									scroll_screen();
									py = VIDEO_ROWS-1;
								}
						}
						px = 0;
						break;

			case '\r':
						if(px > 0) px--;
						break;

			default:
						writechar(str[i], c_attr);
		}
		i++;
	}
	return;
}


/**
 * scroll_screen
 */
void scroll_screen(void)
{
	int i, j, oldpos, newpos;
	unsigned char tmp1, tmp2;

	for(i=1; i<=VIDEO_ROWS; i++) {
		oldpos = i * VIDEO_COLS * 2;
		newpos = (i-1) * VIDEO_COLS * 2;

		for(j=0; j<VIDEO_COLS*2; j+=2) {

			tmp1 = videomem[oldpos + j];
			tmp2 = videomem[oldpos + j + 1];

			videomem[newpos + j]     = tmp1;
			videomem[newpos + j + 1] = tmp2;
		}
	}
}

