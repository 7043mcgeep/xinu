/**
 * @file	pInit.c
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
#include <pane.h>

int pInit(device *devptr) {
/*	int i;
	for (i = 0; i < MAXPANES; i++) {
		if (strcmp(panetab[i].name, devptr->name) == 0) {
			panetab[i].state = PANE_FREE;
			return OK;
		}
	}
	return OK;
*/
//	devptr = (char *)0; 	/* in case Xinu restarts.*/
	panetab[devptr->minor].state = PANE_FREE;
	return OK;	
}
