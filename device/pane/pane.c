/**
 * @file	pane.c
 */

#include <xinu.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <tty.h>
#include <string.h>
#include <shell.h>
#include <device.h>
#include <framebuffer.h>
#include <pane.h>

/* Global variables */
panecount = 0;

void drawPanelName(char *name, int length) {
	int i;
	/* Draw rectangle along bottom of screen */
	drawRect(0, DEFAULT_HEIGHT-15, DEFAULT_WIDTH-1, DEFAULT_HEIGHT-1, GREEN);
	
	/* Write pane name and id inside rectangle */
	struct thrent *thrptr;
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if (strcmp(thrptr->name, name) == 0) {
			int j;
			int x = 10;
			int y = DEFAULT_HEIGHT - CHAR_HEIGHT;
			for (j = 0; j < length; j++) {
				drawChar(name[j], x, y, CYAN);
				x += CHAR_WIDTH;
			}
		}
	}	
}


struct pane panetab[MAXPANES];

void spawnPane() {
	struct pane newpane;
	
	/* Search through panetab and find FREE pane */
	int i;
	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_FREE) {
			char name[6] = "PANE";
			sprintf(name, "%s%d", name, i);
			strcpy(newpane.name, name);
		//	newpane.name = name;
		//	printf("name: %s\n", name);
		//	strncat(name, i, 1);
		//	printf("name: %s\n", name);
			newpane.id = i;
			newpane.state = PANE_USED;
			panetab[i] = newpane;
			panecount++;
		//	screenInit();
		//	ready(create(shell, INITSTK, INITPRIO, name, 3, TTY1, TTY1, TTY1), RESCHED_NO);


	

			if (panecount <= 2) {
				if (panecount == 1) {
					
					screenInit();
					drawPane(1, DEFAULT_WIDTH/2, DEFAULT_HEIGHT-1, 0, 0);	/* Left pane */
					ready(create(shell, INITSTK, INITPRIO, name, 3, TTY1, TTY1, TTY1), RESCHED_NO);
					drawPanelName(name, sizeof(name)/sizeof(char));
				}
				if (panecount == 2) {	
				
					screenInit();
					drawPane(2, DEFAULT_WIDTH/2, DEFAULT_HEIGHT-1, DEFAULT_WIDTH-1, DEFAULT_WIDTH);
					ready(create(shell, INITSTK, INITPRIO, name, 3, TTY1, TTY1, TTY1), RESCHED_NO);
					drawPanelName(name, sizeof(name)/sizeof(char));
				}
			}

		//	drawPane(frame.id, DEFAULT_WIDTH-1, DEFAULT_HEIGHT-1, 0,0);				
		//	drawPanelName(name, sizeof(name)/sizeof(char));

			while(1);
		}
	}
//	printf("Maximum number of panes spawned!\n");
	printf("You're a pushy little bastard, ain't ya? But I like that, I like that!\n\t\t-Karate Kid Sensei\n");	
}

void killPane(char *name) {
	struct thrent *thrptr;				/* Pointer to thread entry*/
	int i, j;

/*
	for (i = 0; i < MAXPANES; i++) {
		if (0 == (strcmp(panetab[i].name, name))) {
			panetab[i].state = PANE_FREE;
			
			for (i 
		}
	}
*/



	for (i = 0; i < MAXPANES; i++) {
		if (strcmp(panetab[i].name, name) == 0) {
			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];
				if (strcmp(thrptr->name, name) == 0) {
					panetab[i].state = PANE_FREE;
					panecount--;
					kill(j);
					return;
				}
			}
		}
	}

//	for (i = 0; i < MAXPANES; i++) {		/* Look through panetab */
//		if (panetab[i].id == 0) {		/* Special case for frame */
//			panetab[0].state = PANE_FREE;
//			for (j = 0; j < NTHREAD; j++) {
//				thrptr = &thrtab[j];
//				if (strcmp(thrptr->name, "KMUXFRAME0") == 0) {
//					kill(j);
//					return;
//				}
//			}
//		
//		}
//		
//		else if (panetab[i].id == id) {		/* If paneid == the id we're looking for, */
//		//	strncat(name, i, 1);		/* set the name to the process's name */
//			sprintf(name, "%s%d", name, i);
//			printf(name);
//			panetab[i].state = PANE_FREE;	/* set the status back to FREE */
//			for (j = 0; j < NTHREAD; j++) {
//				thrptr = &thrtab[j];
//				if (strcmp(thrptr->name, name) == 0) {
//					kill(j);	/* kill process */
//					return;
//				}
//			}	
//		}
//	}
}

void spawnFrame() {

	int i;				
	for (i = 0; i < MAXPANES; i++) {	/* Initalize temporary panes with state FREE */
		struct pane temp;
		temp.id = i;
		temp.state = PANE_FREE;
		panetab[i] = temp;
	}	
	


	struct pane frame;
	frame.state = PANE_USED;
	
	char name[11] = "KMUXFRAME0";
	frame.id = 0;
	panetab[0] = frame;

	screenInit();
	ready(create(shell, INITSTK, INITPRIO, name, 3, TTY1, TTY1, TTY1), RESCHED_NO);

	drawPanelName(name, sizeof(name)/sizeof(char));

	while(1);

}

void drawPane(int id, int width, int height, int posx, int posy) {
	drawRect(posx, posy, posx + width, posy + height, LEAFGREEN);
}
