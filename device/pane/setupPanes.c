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
	char shell0[7] = "SHELL0";
	char shell1[7] = "SHELL1";
	irqmask im;


	im = disable();


	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if ((strcmp(thrptr->name, "SHELL0") == 0)) {
			thrptr->fdesc[0] = NULL;
			thrptr->fdesc[1] = NULL;
			thrptr->fdesc[2] = NULL;
			kprintf("suspended: %d\r\n", i);
		}
	
		if ((strcmp(thrptr->name, "SHELL1") == 0)) {
			thrptr->fdesc[0] = NULL;
			thrptr->fdesc[1] = NULL;
			thrptr->fdesc[2] = NULL;
		
			kprintf("suspended: %d\r\n", i);
		}
	
	}

	restore(im);





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
					panetab[i].canRead = FALSE;
					restore(im);
					break;
				}
			}
			panetab[i].hasFocus = FALSE;
		} 
	}
	kprintf("setup done!\r\n");

}
