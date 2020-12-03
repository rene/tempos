#include <stdio.h>
#include "minilibc.h"

#define outByte(value,port) \
__asm__ ("outb %%al,%%dx\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:"::"a" (value),"d" (port))

#define inByte(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:":"=a" (_v):"d" (port)); \
_v; \
})

#define CMOS_READ(addr) ({ \
    outByte(addr,0x70); \
    inByte(0x71); \
})

int bcd_to_bin(int unit, int key);
int convert_24h(int hour);

void main() {
	// reading values from associated CMOS register number containing respective data
	
	int hour;
    int minute; 
    int second;
    int day;
    int month;
    int year;

	second = CMOS_READ(0x00);
    second=bcd_to_bin(second,0);

	minute = CMOS_READ(0x02);
    minute=bcd_to_bin(minute,0);

    hour = CMOS_READ(0x04);
    hour=bcd_to_bin(hour, 1);

	day = CMOS_READ(0x07);
    day=bcd_to_bin(day, 0);

	month = CMOS_READ(0x08);
    month=bcd_to_bin(month, 0);
    
	year = CMOS_READ(0x09);
    year=bcd_to_bin(year, 0);
    /*
    if(val==1){
        hour= convert_24h(hour);
    }*/

	printf("%03i-%03i-'%03i %03i:%03i:%03i UTC\n", day, month, year, hour, minute, second);
}

int bcd_to_bin(int unit, int key){
    // Converting BCD to binary values
    unit = ((unit & 0x0F) + ((unit / 16) * 10));
    if(key==0)
	    return unit;
    return ((unit) | (unit & 0x80));
}

int convert_24h(int hour){
	// Converting Hours into 24 hour format
	if (!(CMOS_READ(0x0B) & 0x02) && (hour & 0x80)) {
		return ((hour & 0x7F) + 12) % 24;
	}
}
