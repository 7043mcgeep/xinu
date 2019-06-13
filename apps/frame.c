/**
 * frame.c
 * Starts enviornment that panes attach to
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <tty.h>
#include <string.h>
#include <shell.h>
#include <framebuffer.h>
#include <device.h>
#include <pane.h>

/**
 * Spawns the base frame that panes will attach to
 * @returns paneid
 */

void spawnframe() {	

	
	struct pane frame;
	frame.state = PANE_USED;
	
	char name[11] = "KMUXFRAME0";
	frame.id = 0;
	strcpy(frame.name, name);
	panetab[0] = frame;
	ready(create(shell, INITSTK, INITPRIO, name, 3, TTY1, TTY1, TTY1), RESCHED_NO);

	/* Draw rectangle along bottom of screen */
	drawRect(0, DEFAULT_HEIGHT-15, DEFAULT_WIDTH, DEFAULT_HEIGHT, GREEN);
//	fillRect(0, DEFAULT_HEIGHT-15, DEFAULT_WIDTH, DEFAULT_HEIGHT, GREEN, 0);
	
	while(1);

}
