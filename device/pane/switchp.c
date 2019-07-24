/**
 * @file switchp.c
 *
 * kmux shell command that allows the user to switch from one pane to another and type in the appropriate shel
 */

 #include <xinu.h>
 #include <stdio.h>
 #include <stdarg.h>
 #include <framebuffer.h> 
 #include <device.h>
 #include <thread.h>
 #include <pane.h>


int switchp() {
	int activeCount = 0;
	int i, j;


	/* Detecting active PANES */
	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_USED) {
			activeCount++;
		}
	}

	/* PANE0 has focus by default */
//	panetab[0].hasFocus = TRUE;

	struct thrent *thrptr;
	char name[8] = "PSHELL";

/*
	for (i = 0; i < activeCount; i++) {
		if (panetab[i].hasFocus == FALSE) {
			sprintf(name, "%s%d", name, i);
			kprint("%s\r\n", name);
			
			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];
				ow
				if (strcmp(thrptr->name, name) == 0) {
					suspend(j);
					thrptr->fdesc[0] = DEVNULL;
					thrptr->fdesc[1] = DEVNULL;
					thrptr->fdesc[2] = DEVNULL;
				}
			}
		}
	}
*/


	for (i = 0; i < activeCount; i++) {
		if (panetab[i].hasFocus) {
			sprintf(name, "%s%d", name, i);
			
			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];
				if (strcmp(thrptr->name, name) == 0) {
				//	thrptr->fdesc[0] = NULL;
					suspend(j);
				//	thrptr->fdesc[0] = NULL;  
					kprintf("%s has lost focus\r\n", thrptr->name);
				}
			}
			panetab[i].hasFocus = FALSE;
			continue;
		} 
		else {
			int index = i % activeCount;
			panetab[index].hasFocus = TRUE;
			strcpy(name, "PSHELL");
			sprintf(name, "%s%d", name, index);
			kprintf("name: %s\r\n", name);
			kprintf("index: %d\r\n", index);
			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];
				
				if (strcmp(thrptr->name, name) == 0) {
					switch (index) {
						case 0:
							
						//	thrptr->fdesc[0] = PANE0;
						//	thrptr->fdesc[1] = PANE0;
						//	thrptr->fdesc[2] = TTY2;
							resume(j);
							break;

						case 1:
						//	thrptr->fdesc[0] = PANE1;
						//	thrptr->fdesc[1] = PANE1;
						//	thrptr->fdesc[2] = TTY3;
							resume(j);
							kprintf("%s has gained focus\r\n", name);
							break;

						case 2:
						//	thrptr->fdesc[0] = PANE2;
						//	thrptr->fdesc[1] = PANE2;
						//	thrptr->fdesc[2] = TTY4;
							resume(j);
							break;

						case 3:
							thrptr->fdesc[0] = PANE3;
						//	thrptr->fdesc[1] = PANE3;
						//	thrptr->fdesc[2] = TTY5;
							resume(j);
							break;
					}
				}
			}
		}
	}
	
	return OK;
}
