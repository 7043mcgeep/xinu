/**
 * @file	pane.h
 * Definitions, prototypes, and structures for a windowed pane system
 */

#ifndef _PANE_H
#define _PANE_H	
#include <semaphore.h>


#define MAXPANES 4	/* maximum of 4 panes in the panebox */
#define PANEPRIO 100	/* priority of poutproc */

int pInit(device *devptr);
//int pFree(struct pane *ppane);
devcall pOpen(device *devptr, va_list ap);
devcall pClose(device *devptr);
devcall pWrite(device *devptr, unsigned char *buf, int len);
devcall pPutc(device *devptr, unsigned char ch);
devcall pControl(device *devptr, int func, char *arg1, char *arg2);

void spawnPane(void);							/* Create a pane and place it into the panetab */
void killPane(char *name);							/* Remove a pane from the panetab */
void spawnFrame(void);							/* Spawn the frame shell process */
void drawPane(int id, int width, int height, int posx, int posy);	/* Draw a pane on the screen */	
void drawPanelName(char *name, int length);

struct pane {
	device *devptr;		/* pane dev structure 		*/
	int id;			/* ID of pane in panebox	*/
	int state;		/* PANE_* below 		*/
	int init;		/* initalized T/F 		*/
	char name[11];		/* name of pane 		*/

	int ul_row;		/* upper left absolute row 	*/
	int ul_col;		/* upper left absolute col 	*/
	int lr_row;		/* lower right absolute row 	*/
	int lr_col;		/* lower right absolute col 	*/

	int rows;		/* effective pane height 	*/
	int cols;		/* effective pane width 	*/
	int cursrow;		/* current relative cursor 	*/
	int curscol;		/* location in rows and cols 	*/

	int fg;			/* pane foreground color 	*/
	int bg;			/* pane background color 	*/

	/* output fields */
	semaphore p_outsem;	/* output buffer space semaphor */
	int p_ohead;		/* index of first character	*/
	int p_otail;		/* index of last character 	*/
	int p_ocount;		/* # characters in output buffer*/
	int p_olimit;		/* max size of output buffer 	*/
	char* p_outbuf;		/* buffer (dynamically alloted) */
	int p_olowat;		/* buffer low water mark (delay)*/
	int p_odsend;		/* # sends delayed for space 	*/
	int outprocid; 		/* process id of output process	*/
};

/* PANE states */

#define PANE_FREE	0
#define	PANE_USED	1
#define PANE_NULL	2	/* Used when creating the NULL pane for default use of fbPutc when kmux is not */
				/* being used */
				/* Number of used panes */
int pFree(struct pane *ppane);

extern struct pane panetab[MAXPANES];
extern int panecount;

/* PANE locations */

/* 2 PANE configuration */
#define TWO_PANE0_ULROW		0
#define TWO_PANE0_ULCOL		0
#define TWO_PANE0_LRROW		DEFAULT_HEIGHT - 1
#define TWO_PANE0_LRCOL		DEFAULT_WIDTH / 2

#define TWO_PANE1_ULROW		0
#define TWO_PANE1_ULCOL		DEFAULT_WIDTH / 2
#define TWO_PANE1_LRROW		DEFAULT_HEIGHT - 1
#define TWO_PANE1_LRCOL		DEFAULT_WIDTH - 1

/* 3 PANE configuration */
#define THR_PANE0_ULROW		0
#define THR_PANE0_ULCOL		0
#define THR_PANE0_LRROW		DEFAULT_HEIGHT / 2
#define THR_PANE0_LRCOL		DEFAULT_WIDTH / 2 

#define THR_PANE1_ULROW		DEFAULT_HEIGHT / 2
#define THR_PANE1_ULCOL		0
#define THR_PANE1_LRROW		DEFAULT_HEIGHT - 1
#define THR_PANE1_LRCOL		DEFAULT_WIDTH / 2

#define THR_PANE2_ULROW		0
#define THR_PANE2_ULCOL 	DEFAULT_WIDTH / 2
#define THR_PANE2_LRROW		DEFAULT_HEIGHT - 1
#define THR_PANE2_LRCOL		DEFAULT_WIDTH - 1

/* 4 PANE configuration */
#define FOR_PANE0_ULROW		0
#define FOR_PANE0_ULCOL		0
#define FOR_PANE0_LRROW		DEFAULT_HEIGHT / 2
#define FOR_PANE0_LRCOL		DEFAULT_WIDTH / 2

#define FOR_PANE1_ULROW		0
#define FOR_PANE1_ULCOL		DEFAULT_WIDTH / 2
#define FOR_PANE1_LRROW		DEFAULT_HEIGHT - 1
#define FOR_PANE1_LRCOL		DEFAULT_WIDTH - 1

#define FOR_PANE2_ULROW		DEFAULT_HEIGHT / 2
#define FOR_PANE2_ULCOL		DEFAULT_WIDTH / 2
#define FOR_PANE2_LRROW		DEFAULT_HEIGHT - 1
#define FOR_PANE2_LRCOL		DEFAULT_WIDTH - 1

#define FOR_PANE3_ULROW		DEFAULT_HEIGHT / 2
#define FOR_PANE3_ULCOL		0
#define FOR_PANE3_LRROW		DEFAULT_HEIGHT - 1
#define FOR_PANE3_LRCOL		DEFAULT_WIDTH / 2


#endif /* _PANE_H */