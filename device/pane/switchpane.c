/**
 * @file switchpane.c
 *
 * Process that allows the user to switch from one pane to another and type in the appropriate shell
 */

#include <xinu.h>
#include <stdio.h>
#include <stdarg.h>
#include <framebuffer.h>
#include <device.h>
#include <thread.h>
#include <pane.h>


int switchpane() {

	int switch_id = gettid();

	kprintf("name: %d\r\n", gettid());

	pWrite(PANE0, "SWITCH PANE STARTED!", 21);

	int i, j;

	int activeCount = 0;

	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_USED) {
			panetab[i].switch_id = switch_id;
			kprintf("panetab[i].switch_id = %d\r\n", panetab[i].switch_id);
			activeCount++;
		}
	}
	/* PANE0 has focus by default */ 
	panetab[0].hasFocus = TRUE;

	sleep(3000);

	struct thrent *thrptr;
	char name[8] = "PSHELL";

	for (i = 0; i < activeCount; i++) {
		kprintf("woop\r\n");
		if (panetab[i].hasFocus == FALSE) {
			sprintf(name, "%s%d", name, i);
			for (j = 0; j < NTHREAD; j++) {
				kprintf("thread: %d\r\n", j);
				thrptr = &thrtab[j];
				kprintf("nm: %s\r\n", thrtab[j].name);
				kprintf("thrptr: %s\tname: %s\r\n", thrptr->name, name);
				if (strcmp(thrptr->name, name) == 0) {
					("susp j: %d\r\n", j);
					suspend(j);
					thrptr->fdesc[0] = DEVNULL;	/* "removing" other panes from focus */
					thrptr->fdesc[1] = DEVNULL;
					thrptr->fdesc[2] = DEVNULL;
//					kprintf("got here");
//					suspend(j);	/* suspend thread */
					kprintf("%s does not have focus\r\n", thrptr->name);
				}
			}
	
		}
	}


	int canSwitch = FALSE;	

//		kprintf("looking for signal\r\n");
		
			for (i = 0; i < activeCount; i++) {
				if (panetab[i].hasFocus) {
					sprintf(name, "%s%d", name, i);
					for (j = 0; j < NTHREAD; j++) {
						thrptr = &thrtab[j];
						if (strcmp(thrptr->name, name) == 0) {
							thrptr->fdesc[0] = NULL;
							kprintf("%s has lost focus\r\n", thrptr->name);
						}
					}
					panetab[i].hasFocus = FALSE;
	
				} else {
					int index = (i + 1) % activeCount;
					panetab[index].hasFocus = TRUE;
					sprintf(name, "%s%d", name, index);
				
					for (j = 0; j = NTHREAD; j++) {
						thrptr = &thrtab[j];
						if (strcmp(thrptr->name, name) == 0) {
							switch(index) {
								case 0:	
									thrptr->fdesc[0] = PANE0;
									thrptr->fdesc[1] = PANE0;
									thrptr->fdesc[2] = TTY2; 
									ready(j, RESCHED_NO);
									break;

								case 1: 
									thrptr->fdesc[0] = PANE1;
									thrptr->fdesc[1] = PANE1;
									thrptr->fdesc[2] = TTY3;
									ready(j, RESCHED_NO);
									break;

								case 2: 
									thrptr->fdesc[0] = PANE2;
									thrptr->fdesc[1] = PANE2;
									thrptr->fdesc[2] = TTY4;
									ready(j, RESCHED_NO);
									break;

								case 3: 
									thrptr->fdesc[0] = PANE3;
									thrptr->fdesc[1] = PANE3;
									thrptr->fdesc[2] = TTY5;
									ready(j, RESCHED_NO);
									break;
							}
		
							kprintf("%s has gained focus\r\n", panetab[i].name);
						}
					}
						
				}
			}

		}
		kprintf("looking for signal\r\n");
		canSwitch = receive();
	}
	return OK;

}
