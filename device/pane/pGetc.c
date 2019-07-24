/**
 * @file pGetc.c
 *
 */

#include <stddef.h>
#include <ctype.h>
#include <device.h>
#include <thread.h>
#include <pane.h>

devcall pGetc(device *devptr) {
	char ch;
	int result = NULL;
	struct pane *ppane = &devtab[devptr->minor];

	result = pRead(ppane, &ch, 1);


	if (1 != result) {
		return result;
	}

	if (ch == '~') {
		int i, j;
		int activeCount = 0;
		struct thrent *thrptr;
		char name[8] = "PSHELL";

	
		for (i = 0; i < MAXPANES; i++) {
			if (panetab[i].state == PANE_USED) {
				activeCount++;
			}
		}


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
	return ch;

}
