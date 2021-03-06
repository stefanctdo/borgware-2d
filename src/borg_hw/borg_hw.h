#ifndef BORG_HW_H
#define BORG_HW_H

// LINEBYTES holds the number of bytes per line within the frame buffer (pixmap)
#define LINEBYTES (((NUM_COLS-1)/8)+1)


#ifndef BORG_FRAMEBUF
	extern unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];
#else /* BORG_FRAMEBUF */
	unsigned char framebuf[NUM_ROWS][NUM_COLS];
#endif /* BORG_FRAMEBUF */

void watchdog_enable();
void borg_hw_init();
void timer0_off();

#endif
