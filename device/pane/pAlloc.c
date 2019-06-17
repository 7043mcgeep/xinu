/**
 * @file	palloc.c
 * 
 * Allocate and initialize a new pane structure
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

struct pane *palloc(void) {
	struct pane newpane;
	int i;

	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_FREE) {
			char name[6] = "PANE";
			sprintf(name, "%s%d", name, i);
			strcpy(newpane.name, name);
			newpane.id = i;
			newpane.state = PANE_USED;
			panetab[i] = newpane;
			return (&panetab[i]);
		}
	}
	return (struct pane *)SYSERR;
}
