/**
 * @file	pane.h
 * Definitions, prototypes, and structures for a windowed pane system
 */

#ifndef _PANE_H
#define _PANE_H	

#define MAXPANES 4	/* maximum of 4 panes in the panebox */

int pInit(device *devptr);
devcall pOpen(device *devptr);
devcall pClose(device *devptr);
devcall pWrite(device *devptr);
devcall pPutc(device *devptr, char ch);

void spawnPane(void);							/* Create a pane and place it into the panetab */
void killPane(char *name);							/* Remove a pane from the panetab */
void spawnFrame(void);							/* Spawn the frame shell process */
void drawPane(int id, int width, int height, int posx, int posy);	/* Draw a pane on the screen */	
void drawPanelName(char *name, int length);

struct pane {
	int id;		/* ID of pane in panebox*/
	int state;	/* PANE_* below */
	int init;	/* initalized T/F */
	char name[11];	/* name of pane */
	int xpos;	/* Upper left corner x position of pane */
	int ypos;	/* Upper left corner y position of pane */
};

/* PANE states */

#define PANE_FREE	0
#define	PANE_USED	1
#define PANE_NULL	2	/* Used when creating the NULL pane for default use of fbPutc when kmux is not */
				/* being used */
				/* Number of used panes */
extern struct pane panetab[MAXPANES];
extern int panecount;
//struct pane panetab[MAXPANES];	/* Table keeping track of panes */
#endif /* _PANE_H */
