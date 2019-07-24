/**
 * @file	xsh_kmux.c
 *
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
#include <mutex.h>

/**
 * Shell command (split). Splits the screen and spawns a new shell window until ended
 * (via 'join' command).
 * @param nargs	 number of arguments in args array
 * @param args   array of arguments
 * @return OK at end
 */


#if FRAMEBUF	/* Don't include if no framebuffer exists */

/* kmux function prototypes */

void help(void);
void openPanes(int);
void closePanes(void);
void switchPanes(void);

/* testing */
void outproc0(char *text, int fg, int bg, int pane);
void outproc1(char *text, int fg, int bg, int pane);
void outproc2(char *text, int fg, int bg, int pane);
void soutproc0(void);
void soutproc1(void);
/* kmux built-in commands */
struct defaultcommand builtInCommands[] = {
	{"open", openPanes},
	{"close", closePanes},
	{"switch", switchPanes}
	
};

/* global variables used by xsh_kmux */
int panecnt = 0;	/* number of current panes */
mutex_t frame_lock;


shellcmd xsh_kmux(int nargs, char *args[])
{
	char buf[SHELL_BUFLEN];		/* line input buffer */
	short buflen;			/* length of line inpu */
	char tokbuf[SHELL_BUFLEN + SHELL_MAXTOK];	/* token value buffer */
	short ntok;			/* number of tokens */
	char *tok[SHELL_MAXTOK];	/* pointers to token values */	
	frame_lock = mutex_create();

	/* Constantly receive and process input */
	while(TRUE) {
		printf("\nkmux$ ");
		/* Receive and parse input */
		control(stdin, TTY_CTRL_CLR_IFLAG, TTY_IRAW, NULL);
		control(stdin, TTY_CTRL_SET_IFLAG, TTY_ECHO, NULL);
		buflen = read(stdin, buf, SHELL_BUFLEN - 1);
		if ( (SYSERR == (ntok = lexan(buf, buflen, &tokbuf[0], &tok[0]))) || ntok <= 0)
		{
			printf("Sorry, command not recognized.");
			continue;
		}
		//type end to break;
		if ( (0 == strcmp(tok[0], "end")) && (ntok == 1) ) break;
		
		//display a help menu that explains available commands
		else if ((0 == strcmp(tok[0], "help")) && (ntok == 1)) {
			printf("help goes here\n");
			continue;
		}

		//split command
		else if ((0 == strcmp(tok[0], "open")) && (ntok == 2)) {
			openPanes(atoi(tok[1]));
		//	printf("Opened %d windows\n", atoi(tok[1]));
			return OK;
		}

		//join command 
		else if ((0 == strcmp(tok[0], "close")) && (ntok == 1)) {
			closePanes();
			return OK;
		}
		
		//switch command
		else if ((0 == strcmp(tok[0], "switch")) && (ntok == 1)) {
			switchp();
			return OK;
		}

		//status command
		else if ((0 == strcmp(tok[0], "status")) && (ntok == 1)) {
			int i;
			printf("ID\t|\tNAME\t|\tSTATE\n");
			for (i = 0; i < MAXPANES; i++) {
				printf("%d\t\t%s\t\t%d\n", panetab[i].id, panetab[i].name, panetab[i].state);
			}
			continue;
		}

		/* If we made it here, we have no idea what this command is. */
		printf("Sorry, %s is not a recognized command. Check syntax and help menu.", tok[0]);

		continue;

	}
}


void switchPanes(void) {
	switchp();

}

void outproc0(char *text, int fg, int bg, int pane) {
	char str[256];
	int count = 0;
	int fd = 0;
	
//	enable();

//	str = (char *)memget(256);

	if ((char *)SYSERR == str)
		return;



//	drawRect(TWO_PANE0_ULCOL, TWO_PANE0_ULROW, TWO_PANE0_LRCOL, TWO_PANE0_LRROW, LEAFGREEN);	
		
//	open(TTY1, PANE0);
	open(PANE0, THR_PANE0_ULROW, THR_PANE0_ULCOL, THR_PANE0_LRROW, THR_PANE0_LRCOL, fg, bg);
//	ready(create(shell, INITSTK, INITPRIO, "PSHELL0", 3, TTY1, TTY1, TTY1), RESCHED_NO);
//	sleep(10);
	while(1) {
		sprintf(str, "Process %d says %s for the %d time\n", gettid(), text, count++);
//		mutex_acquire(frame_lock);
		write(PANE0, str, strlen(str));
//		mutex_release(frame_lock);
		sleep(10);		
	}

}

void outproc1(char *text, int fg, int bg, int pane) {
	char str[256];
	int count = 0; 
//	enable();

//	str = (char *)memget(256);

	if ((char *)SYSERR == str)
		return;


		
//	drawRect(TWO_PANE1_ULCOL, TWO_PANE1_ULROW, TWO_PANE1_LRCOL, TWO_PANE1_LRROW, LEAFGREEN); 

//	open(TTY1, PANE1);
	open(PANE1, THR_PANE1_ULROW, THR_PANE1_ULCOL, THR_PANE1_LRROW, THR_PANE1_LRCOL, fg, bg);
//	ready(create(shell, INITSTK, INITPRIO, "PSHELL1", 3, TTY1, TTY1, TTY1), RESCHED_NO);
//	sleep(10);
	while(1)  {
	
		sprintf(str, "Process %d says %s for the %d time\n", gettid(), text, count++);
//		mutex_acquire(frame_lock);
		write(PANE1, str, strlen(str));
//		mutex_release(frame_lock);
//		sleep(10);
		sleep(10);
		
	}
}

void outproc2(char *text, int fg, int bg, int pane) {
	char str[256];
	int count = 0; 


//	enable();



	if ((char *)SYSERR == str)
		return;


		
//	drawRect(TWO_PANE1_ULCOL, TWO_PANE1_ULROW, TWO_PANE1_LRCOL, TWO_PANE1_LRROW, LEAFGREEN); 
	
//	open(TTY1, PANE1);
	open(PANE2, THR_PANE2_ULROW, THR_PANE2_ULCOL, THR_PANE2_LRROW, THR_PANE2_LRCOL, fg, bg);
//	ready(create(shell, INITSTK, INITPRIO, "PSHELL1", 3, TTY1, TTY1, TTY1), RESCHED_NO);

//	write(PANE2, SHELL_BANNER_PI3_NONVT100, strlen(SHELL_BANNER_PI3_NONVT100));
//	sleep(10);
	while(1)  {
		sprintf(str, "Process %d says %s for the %d time\n", gettid(), text, count++);
//		mutex_acquire(frame_lock);
		write(PANE2, str, strlen(str));
//		mutex_release(frame_lock);
//		sleep(10);
		sleep(10);
		
	}
}



void soutproc0() {
	open(PANE0, TWO_PANE0_ULROW, TWO_PANE0_ULCOL, TWO_PANE0_LRROW, TWO_PANE0_LRCOL, WHITE, BLACK);
	open(TTY2, KBDMON0);

/*	
	char shell0[7] = "SHELL0";
	char shell1[7] = "SHELL1";
	int i;
	struct thrent *thrptr;
	
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if ((strcmp(thrptr->name, shell0) == 0) || (strcmp(thrptr->name, shell1) == 0)) {
			suspend(i);
			kprintf("suspended: %d\r\n");
		}
	}

	*/
		

//	ready(create((void *)shell, INITSTK, INITPRIO, "PSHELL0", 3, PANE0, PANE0, TTY2), RESCHED_NO);
	int shell_id;
	shell_id = create((void *)shell, INITSTK, INITPRIO, "PSHELL0", 3, PANE0, PANE0, TTY2);
	sprintf(&thrtab[shell_id].name, "%s", "PSHELL0");
	ready(shell_id, RESCHED_NO);




//	while (1) {
//		write(PANE0, buf, strlen(buf));
//	}

//	open(TTY1, PANE0);
//	open(TTY1, KBDMON0);
//	ready(create((void *)shell, INITSTK, INITPRIO, "PSHELL0", 3, TTY1, TTY1, TTY1), RESCHED_NO);
	
		
	//ready(create((void *)shell, INITSTK, INITPRIO, "PSHELL0", 3, PANE0, PANE0, PANE0), RESCHED_NO);
}

void soutproc1() {
	open(PANE1, TWO_PANE1_ULROW, TWO_PANE1_ULCOL, TWO_PANE1_LRROW, TWO_PANE1_LRCOL, WHITE, BLACK);
	open(TTY3, KBDMON0);

//	control(stdin, PANE_CTRL_CLR_IFLAG, PANE_IRAW, NULL);
//
//	control(stdin, PANE_CTRL_SET_IFLAG, PANE_ECHO, NULL);
//	((thrtab[thrcurrent[getcpuid()]]).fdesc[0]) = DEVNULL;	
//	((thrtab[thrcurrent[getcpuid()]]).fdesc[1]) = DEVNULL;	

/*
	char shell0[7] = "SHELL0";
	char shell1[7] = "SHELL1";
*/
	int i;
	struct thrent *thrptr;
/*
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if ((strcmp(thrptr->name, shell0) == 0)) {
			suspend(i);
			kprintf("suspended: %d\r\n", i);
		}
		if ((strcmp(thrptr->name, shell1) == 0)) {
			suspend(i);
		}
	}
*/
	int shell_id;
	shell_id = create((void *)shell, INITSTK, INITPRIO, "PSHELL1", 3, PANE1, PANE1, TTY3);
	sprintf(&thrtab[shell_id].name, "%s", "PSHELL1");
	ready(shell_id, RESCHED_NO);

/*
	udelay(1000);
		
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if ((strcmp(thrptr->name, "PSHELL1") == 0)) {
			suspend(i);
		}	
	}
*/

//	ready(create((void *)shell, INITSTK, INITPRIO, "PSHELL1", 3, TTY3, PANE1, TTY3), RESCHED_NO);
//	control(stdin, PANE_CTRL_CLR_IFLAG, PANE_IRAW, NULL);
//	control(stdin, PANE_CTRL_SET_IFLAG, PANE_ECHO, NULL);

//	sleep(20000);

}



void openPanes(int num) {
	if (num == 2) {

		int switch_id;

/*
		drawRect(TWO_PANE0_ULCOL, TWO_PANE0_ULROW, TWO_PANE0_LRCOL, TWO_PANE0_LRROW, LEAFGREEN);	
		drawRect(TWO_PANE1_ULCOL, TWO_PANE1_ULROW, TWO_PANE1_LRCOL, TWO_PANE1_LRROW, LEAFGREEN); 
		if (OK == open(CONSOLE, TTY1)) {
			
			ready(create(shell, INITSTK, INITPRIO, "PANE1", 3, TTY1, TTY1, TTY1), RESCHED_NO);
			
		}

		if (OK == open(PANE2, CONSOLE2)) {
			
			ready(create(shell, INITSTK, INITPRIO, "PANE2", 3, TTY1, TTY1, TTY1), RESCHED_NO);
			
		}
*/
/*
		kill(26);

		screenClear(BLACK);	
		ready(create((void *)outproc0, INITSTK, 20, "outproc0", 3, "AAA", MAGENTA, 1), RESCHED_YES);
		ready(create((void *)outproc1, INITSTK, 20, "outproc1", 3, "BBB", CYAN, 1), RESCHED_YES);

*/		
//		open(PANE0, TWO_PANE0_ULROW, TWO_PANE0_ULCOL, TWO_PANE0_LRROW, TWO_PANE0_LRCOL, CYAN, BLACK);	
//		open(PANE1, TWO_PANE1_ULROW, TWO_PANE1_ULCOL, TWO_PANE1_LRROW, TWO_PANE1_LRCOL, MAGENTA, BLACK);
	
//		ready(create((void *)switchpane, INITSTK, 20, "switchpane", 0), RESCHED_YES);
		
		char shell0[7] = "SHELL0";
		char shell1[7] = "SHELL1";
		int i;
		struct thrent *thrptr;

		for (i = 0; i < NTHREAD; i++) {
			thrptr = &thrtab[i];
			if ((strcmp(thrptr->name, shell0) == 0)) {
			//	thrptr->prio = suspend(i);
				suspend(i);
				kprintf("suspended: %d\r\n", i);
				continue;
			}
			if ((strcmp(thrptr->name, shell1) == 0)) {
			//	thrptr->prio = suspend(i);
				suspend(i);
				kprintf("suspended: %d\r\n", i);
			}
		}

		ready(create((void *)soutproc0, INITSTK, 20, "soutproc0", 0), RESCHED_YES);
		ready(create((void *)soutproc1, INITSTK, 20, "soutproc1", 0), RESCHED_YES);
		udelay(1000);
		ready(create((void *)setupPanes, INITSTK, 20, "setupPanes", 0), RESCHED_YES);

//		switch_id = create((void *)switchpane, INITSTK, 20, "switchpane", 0);
//		ready(switch_id, RESCHED_YES);		

//		ready(create((void *)soutproc0, INITSTK, 20, "outproc0", 0), RESCHED_YES);
//		ready(create((void *)soutproc1, INITSTK, 20, "outproc1", 0), RESCHED_YES);
		
		

	}

	else if (num == 3) {
/*
		drawRect(THR_PANE0_ULCOL, THR_PANE0_ULROW, THR_PANE0_LRCOL, THR_PANE0_LRROW, LEAFGREEN);
		drawRect(THR_PANE1_ULCOL, THR_PANE1_ULROW, THR_PANE1_LRCOL, THR_PANE1_LRROW, LEAFGREEN);
		drawRect(THR_PANE2_ULCOL, THR_PANE2_ULROW, THR_PANE2_LRCOL, THR_PANE2_LRROW, LEAFGREEN);
*/
//		kill(26);

		ready(create((void *)outproc0, INITSTK, 10, "outproc0", 3, "AAA", BLUE, 1), RESCHED_YES);
//		sleep(10);
		ready(create((void *)outproc1, INITSTK, 10, "outproc1", 3, "BBB", GREEN, 1), RESCHED_YES);
//		sleep(10);
		ready(create((void *)outproc2, INITSTK, 10, "outproc2", 3, "CCC", ORANGE, 1), RESCHED_YES);
	
	}
	else if (num == 4) {				
		drawRect(FOR_PANE0_ULCOL, FOR_PANE0_ULROW, FOR_PANE0_LRCOL, FOR_PANE0_LRROW, LEAFGREEN);
		drawRect(FOR_PANE1_ULCOL, FOR_PANE1_ULROW, FOR_PANE1_LRCOL, FOR_PANE1_LRROW, LEAFGREEN);
		drawRect(FOR_PANE2_ULCOL, FOR_PANE2_ULROW, FOR_PANE2_LRCOL, FOR_PANE2_LRROW, LEAFGREEN);
		drawRect(FOR_PANE3_ULCOL, FOR_PANE3_ULROW, FOR_PANE3_LRCOL, FOR_PANE3_LRROW, LEAFGREEN);
	}
}
	
void closePanes() {
	int i, j;
	struct thrent *thrptr;
	char name[8] = "PSHELL";
	char proc[10] = "*PANEOUT*";

	for (i = 0; i < MAXPANES; i++) {
		if (panetab[i].state == PANE_USED) {
			for (j = 0; j < NTHREAD; j++) {
				thrptr = &thrtab[j];
				if (strcmp(thrptr->name, name) == 0) {
					kill(j);
				}
			}
		}
	}
}




#endif /* FRAMEBUF */
