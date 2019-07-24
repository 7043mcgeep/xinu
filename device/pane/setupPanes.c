#include <xinu.h>
#include <stdio.h>
#include <stdarg.h>
#include <framebuffer.h>
#include <device.h>
#include <thread.h>
#include <pane.h>


int setupPanes() {
	int activeCount = 0;
	int i, j;

	struct thrent *thrptr;
	char name[8] = "PSHELL";


	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_USED) {
			activeCount++;
		}
	}

	/* PANE0 has focus by default */
	panetab[0].hasFocus = TRUE;


	
	/* suspending all panes not in focus */
	for (i = 0; i < activeCount; i++) {
		if (panetab[i].hasFocus == FALSE) {
			sprintf(name, "%s%d", name, i);

			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];
				if (strcmp(thrptr->name, name) == 0) {
				//	thrptr->fdesc[0] = NULL;
					suspend(j);	
					break;
				}
			}
			panetab[i].hasFocus = FALSE;
		} 
	}
	kprintf("setup done!\r\n");
	return OK;
	
}
