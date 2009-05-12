/**
 * A simple video driver
 * Author: Renê de Souza Pinto
 */

#ifndef VIDEO_H_

	#include <x86/karch.h>

	#define VIDEO_H_ 1

	#define VIDEO_MEM_ADDR 0xB8000
	#define VIDEO_COLS     80
	#define VIDEO_ROWS     25
	#define VIDEO_MEM_SIZE VIDEO_COLS * VIDEO_ROWS * 2

	/**
	 * Video color attributes
	 */
	#define BLACK          0x00
	#define BLUE           0x01
	#define GREEN          0x02
	#define CYAN           0x03
	#define RED            0x04
	#define MAGENTA        0x05
	#define BROWN          0x06
	#define LIGHT_GRAY     0x07
	#define DARK_GRAY      0x08
	#define LIGHT_BLUE     0x09
	#define LIGHT_GREEN    0x0A
	#define LIGHT_CYAN     0x0B
	#define LIGHT_RED      0x0C
	#define LIGHT_MAGENTA  0x0D
	#define YELLOW         0x0E
	#define WHITE          0x0F

	/* Prototypes */
	void clrscr(void);
	void gotoxy(unsigned char x, unsigned char y);
	void setattr(unsigned char attr);
	void writechar(char ch, unsigned char attr);
	void writechar_xy(char ch, unsigned char attr, unsigned char x, unsigned char y);
	void kprint(char *str);
	void scroll_screen(void);

#endif /* VIDEO_H_ */

