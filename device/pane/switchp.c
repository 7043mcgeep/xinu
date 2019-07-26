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
 #include <shell.h>

int switchp() {
	int activeCount = 0;
	int i, j;
	irqmask im;

	/* Detecting active PANES */
	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_USED) {
			activeCount++;
		}
	}

	struct thrent *thrptr;
	char name[8] = "PSHELL";



	for (i = 0; i < activeCount; i++) {
		if (panetab[i].hasFocus) {
			sprintf(name, "%s%d", name, i);
			
			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];

				if (strcmp(thrptr->name, name) == 0) {

					im = disable();
					panetab[i].canRead = FALSE;

					restore(im);
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
					im = disable();
	
					switch (index) {
						case 0:
							//ready(create((void *)shell, INITSTK, INITPRIO, "PSHELL0", PANE0, PANE0, TTY2), RESCHED_YES);
							panetab[index].canRead = TRUE;
							restore(im);
							break;

						case 1:
						//ready(create((void *)shell, INITSTK, INITPRIO, "PSHELL1", PANE1, PANE1, TTY3), RESCHED_YES);
							panetab[index].canRead = TRUE;
							restore(im);
							break;

						case 2:
						restore(im);
							break;

						case 3:
						restore(im);
							break;
					}
					
				}
			}
		}
	}

	return OK;
}
