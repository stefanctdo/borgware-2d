/**
 * Conways Game of life 
 * Author: Daniel Otte
 * License: GPLv3
 * 
 * 
 */

#include <stdint.h>
#ifdef AVR
	#include <util/delay.h>
	#include <avr/sfr_defs.h> /* for debugging */
#endif
#include "../config.h"
#include "../random/prng.h"
#include "../pixel.h"
#include "../util.h"

/******************************************************************************/ 

#undef DEBUG
 
#define XSIZE UNUM_COLS
#define YSIZE UNUM_ROWS

// optimizing for 8 bit archs while retaining compatibility with dimensions >255
#if UNUM_COLS < 128 && UNUM_ROWS < 128
typedef uint8_t coord_t;
#else
typedef unsigned int coord_t;
#endif

/* 
 *  last line is for debug information
 */
#ifdef DEBUG 
 #undef YSIZE
 #define YSIZE (UNUM_ROWS-1)
 #define DEBUG_ROW (UNUM_ROWS-1)
 #define DEBUG_BIT(pos, val) \
  setpixel((pixel){(pos)%XSIZE,DEBUG_ROW+(pos)/XSIZE},(val)?3:0)
 #define DEBUG_BYTE(s,v) \
  DEBUG_BIT((s)*8+0, (v)&(1<<0)); \
  DEBUG_BIT((s)*8+1, (v)&(1<<1)); \
  DEBUG_BIT((s)*8+2, (v)&(1<<2)); \
  DEBUG_BIT((s)*8+3, (v)&(1<<3)); \
  DEBUG_BIT((s)*8+4, (v)&(1<<4)); \
  DEBUG_BIT((s)*8+5, (v)&(1<<5)); \
  DEBUG_BIT((s)*8+6, (v)&(1<<6)); \
  DEBUG_BIT((s)*8+7, (v)&(1<<7))   
#else
 #define DEBUG_BIT(s,v)
 #define DEBUG_BYTE(s,v)
#endif

//#define GLIDER_TEST

#define BITSTUFFED
#define LOOP_DETECT_BUFFER_SIZE 8U

#ifndef GOL_DELAY
 #define GOL_DELAY 1 /* milliseconds */
#endif

#ifndef GOL_CYCLES
 #define GOL_CYCLES (2*60*3)
#endif

/******************************************************************************/
/******************************************************************************/

enum cell_e {dead=0, alive=1};
#ifdef NDEBUG
	typedef uint8_t cell_t;
#else
	typedef enum cell_e cell_t;
#endif

#ifndef BITSTUFFED

#define FIELD_XSIZE XSIZE
#define FIELD_YSIZE YSIZE

typedef cell_t field_t[FIELD_XSIZE][FIELD_YSIZE];

/******************************************************************************/

void setcell(field_t  pf, coord_t x, coord_t y, cell_t value){
	pf[(x+FIELD_XSIZE)%FIELD_XSIZE][(y+FIELD_YSIZE)%FIELD_YSIZE] = value;
}

/******************************************************************************/

cell_t getcell(field_t pf, coord_t x, coord_t y){
	return pf[(x+FIELD_XSIZE)%FIELD_XSIZE][(y+FIELD_YSIZE)%FIELD_YSIZE];
}

#else /* BITSTUFFED */

#define FIELD_XSIZE ((XSIZE+7)/8)
#define FIELD_YSIZE YSIZE

typedef uint8_t field_t[FIELD_XSIZE][FIELD_YSIZE];

/******************************************************************************/

void setcell(field_t pf, coord_t x, coord_t y, cell_t value){
	uint8_t t;

	t = pf[x/8][y];
	if(value==alive){
		t |= 1<<(x&7);
	} else {
		t &= ~(1<<(x&7));
	}
	pf[x/8][y] = t;
}

/******************************************************************************/

static cell_t getcell(field_t pf, coord_t x, coord_t y){
	return ((pf[x/8][y])&(1<<(x&7)))?alive:dead;
}
#endif

/******************************************************************************/

uint8_t countsurroundingalive(field_t pf, coord_t x, coord_t y){

	static int8_t const offset[] = {-1, -1, 0, +1, +1, +1, 0, -1, -1, -1};
	x += XSIZE;
	y += YSIZE;
	uint8_t i, ret=0;
	for (i = 8; i--;)
	{
		// getcell(...) returns either 0 or 1
		ret += getcell(pf,(x+offset[i+2])%XSIZE, (y+offset[i])%YSIZE);
	}

	return ret;
}

/******************************************************************************/

void nextiteration(field_t dest, field_t src){
	coord_t x,y;
	uint8_t tc;
	for(y=0; y<YSIZE; ++y){
		for(x=0; x<XSIZE; ++x){
			tc=countsurroundingalive(src,x,y);
			switch(tc){
	//			case 0:
	//			case 1:
	//				/* dead */
	//				setcell(dest, x,y, dead);
				case 2:
					/* keep */
					setcell(dest, x,y, getcell(src,x,y));
					break;
				case 3:
					/* alive */
					setcell(dest, x,y, alive);
					break;
				default:
					/* dead */
					setcell(dest, x,y, dead);
					break;
			}
		}
	}
} 

/******************************************************************************/

void printpf(field_t pf){
	coord_t x,y;
	for(y=YSIZE; y--;){
		for(x=XSIZE; x--;){
			setpixel((pixel){x,y},getcell(pf,x,y)*3);
		}
	}
}

/******************************************************************************/

void pfcopy(field_t dest, field_t src){
	coord_t x,y;
	for(y=YSIZE; y--;){
		for(x=XSIZE; x--;){
			setcell(dest,x,y,getcell(src,x,y));
		}
	}
}

/******************************************************************************/
#ifndef BITSTUFFED
uint8_t pfcmp(field_t dest, field_t src){
	coord_t x,y;
	for(y=YSIZE; y--;){
		for(x=XSIZE; x--;){
			if (getcell(src,x,y) != getcell(dest,x,y))
				return 1;
		}
	}
	return 0;
}

/******************************************************************************/

uint8_t pfempty(field_t src){
	coord_t x,y;
	for(y=YSIZE; y--;){
		for(x=XSIZE; x--;){
			if (getcell(src,x,y)==alive)
				return 0;
		}
	}
	return 1;
}

#else

uint8_t pfcmp(field_t dest, field_t src){
	coord_t x,y;
	for(y=0; y<FIELD_YSIZE; ++y){
		for(x=0; x<FIELD_XSIZE; ++x){
			if (src[x][y] != dest[x][y])
				return 1;
		}
	}
	return 0;
}

/******************************************************************************/

uint8_t pfempty(field_t src){	
	coord_t x,y;
	for(y=0; y<FIELD_YSIZE; ++y){
		for(x=0; x<FIELD_XSIZE; ++x){
			if (src[x][y]!=0)
				return 0;
		}
	}
	return 1;
}

#endif
/******************************************************************************/

void insertglider(field_t pf){
	/*
	 *  #
	 *   #
	 * ###
	 */
		                      setcell(pf, 1, 0, alive);
	                                                    setcell(pf, 2, 1, alive);
	setcell(pf, 0, 2, alive); setcell(pf, 1, 2, alive); setcell(pf, 2, 2, alive);
}

/******************************************************************************/

int gameoflife(){
	DEBUG_BYTE(0,0); // set debug bytes to zero
	DEBUG_BYTE(1,0);
	field_t pf1,pf2;
	field_t ldbuf[LOOP_DETECT_BUFFER_SIZE]={{{0}}}; // loop detect buffer
	uint8_t ldbuf_idx=0;
	coord_t x,y;
	uint16_t cycle;
	
//start:	
	/* initalise the field with random */
	for(y=YSIZE; y--;){
		for(x=XSIZE; x--;){
			setcell(pf1,x,y,(random8()&1)?alive:dead);
		}
	}
#ifdef GLIDER_TEST	
	/* initialise with glider */
	for(y=YSIZE; y--;){
		for(x=XSIZE; x--;){
			setcell(pf1,x,y,dead);
		}
	}
	insertglider(pf1);	 
#endif	
	
	/* the main part */
	printpf(pf1);
	for(cycle=1; cycle<GOL_CYCLES; ++cycle){
		DEBUG_BYTE(0,(uint8_t)(GOL_CYCLES-cycle)&0xff);
		DEBUG_BYTE(1, SREG);
		wait(GOL_DELAY);
		pfcopy(pf2,pf1);
		nextiteration(pf1,pf2);
		printpf(pf1);
	/* loop detection */
		if(!pfcmp(pf1, pf2)){
			insertglider(pf1);
			cycle=1;
		}
		if(pfempty(pf1)){
			/* kill game */
			return 0;
		}
	/* */
		uint8_t i;
		for(i=0; i<LOOP_DETECT_BUFFER_SIZE; ++i){
			if(!pfcmp(pf1, ldbuf[i])){
				insertglider(pf1);
				cycle=1;
			}
		}
		pfcopy(ldbuf[ldbuf_idx], pf1);
		ldbuf_idx = (ldbuf_idx+1)%LOOP_DETECT_BUFFER_SIZE;
		
	}
	
	return 0;
}

