/**
 * @file	pane.h
 * Definitions, prototypes, and structures for a windowed pane system
 */


#define MAXPANES 5	/* maximum of 4 panes in the panebox */

void spawnPane(void);							/* Create a pane and place it into the panetab */
void killPane(char *name);							/* Remove a pane from the panetab */
void spawnFrame(void);							/* Spawn the frame shell process */
void drawPane(int id, int width, int height, int posx, int posy);	/* Draw a pane on the screen */	
void drawPanelName(char *name, int length);
struct pane {
	int id;		/* ID of pane in panebox*/
	int state;	/* PANE_* below */
	char name[11];	/* name of pane */
};

/* PANE states */

#define PANE_FREE	0
#define	PANE_USED	1

				/* Number of used panes */
extern struct pane panetab[MAXPANES];
extern int panecount;
//struct pane panetab[MAXPANES];	/* Table keeping track of panes */
