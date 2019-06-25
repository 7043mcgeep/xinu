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

/**
 * Shell command (split). Splits the screen and spawns a new shell window until ended
 * (via 'join' command).
 * @param nargs	 number of arguments in args array
 * @param args   array of arguments
 * @return OK at end
 */


#if FRAMEBUF	/* Don't include if no framebuffer exists */

/* Fixed pane locations */
#define PANE1X	0
#define	PANE1Y	0
#define	PANE2X	(DEFAULT_WIDTH / 2)
#define PANE2Y	0

/* kmux function prototypes */
/* DEPRECATED */
void new(void);
void split(void);
void startShell(void);
void join(void);

void help(void);
void openPanes(int);
void closePanes(void);

/* testing */
void outproc0(void);
void outproc1(char *text, int fg, int pane);

/* kmux built-in commands */
struct defaultcommand builtInCommands[] = {
	{"open", openPanes},
	{"close", closePanes}
	
};

/* global variables used by xsh_kmux */
int panecnt = 0;	/* number of current panes */


shellcmd xsh_kmux(int nargs, char *args[])
{
	char buf[SHELL_BUFLEN];		/* line input buffer */
	short buflen;			/* length of line inpu */
	char tokbuf[SHELL_BUFLEN + SHELL_MAXTOK];	/* token value buffer */
	short ntok;			/* number of tokens */
	char *tok[SHELL_MAXTOK];	/* pointers to token values */	

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

		return OK;

	}
}



void startShell() {
	if (panecnt < MAXPANES) {
		switch(panecnt) {

			case 0:		ready(create(shell, INITSTK, INITPRIO, "PANE1", 3, TTY1, TTY1, TTY1), RESCHED_NO); 
				//	screenInit();
					panecnt++;
					while(1);
					break;
		
			case 1: 	ready(create(shell, INITSTK, INITPRIO, "PANE2", 3, TTY1, TTY1, TTY1), RESCHED_NO);
				//	screenInit();
					panecnt++;
					while(1);
					break;

			case 2: 	ready(create(shell, INITSTK, INITPRIO, "PANE3", 3, TTY1, TTY1, TTY1), RESCHED_NO);
				//	screenInit(); 
					panecnt++;
					while(1);
					break;

			case 3:		ready(create(shell, INITSTK, INITPRIO, "PANE4", 3, TTY1, TTY1, TTY1), RESCHED_NO);
				//	screenInit();
					panecnt++;
					while(1);
					break;
		}
	}
	else {
		printf("Maximum number of panes spawned!\n");
	}	



//	ready(create((void *)shell, INITSTK, INITPRIO, "SHELL2", 3, CONSOLE, CONSOLE, CONSOLE), RESCHED_YES);
//	open(TTY1, KBDMON0);
//	screenInit();
}


void join() {	/* new version needs paneid */
	struct thrent *thrptr;	/* pointer to thread entry */
	int i;
/*
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if (strcmp(thrptr->name, "SHELL2") == 0) {
			kill(i)
		}
	} */

/*
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if (strcmp(thrptr->name, "SHELL2") == 0) {
			kill(i);
			panecnt--;
			return;
		} else if (strcmp(thrptr->name, "SHELL3") == 0) {
			kill(i);
			panecnt--;
			return;
		} else if (strcmp(thrptr->name, "SHELL4") == 0) {
			kill(i);
			panecnt--;
			return;
		} else if (strcmp(thrptr->name, "SHELL5") == 0) {
			kill(i);
			panecnt--;
			return;
		}
	}
	printf("Unable to destroy SHELL process!\n");
*/
}


void outproc0() {
	enable();

//	drawRect(TWO_PANE0_ULCOL, TWO_PANE0_ULROW, TWO_PANE0_LRCOL, TWO_PANE0_LRROW, LEAFGREEN);	
	
//	open(TTY1, PANE0);
	open(PANE0, TWO_PANE0_ULROW, TWO_PANE0_ULCOL, TWO_PANE0_LRROW, TWO_PANE0_LRCOL, WHITE, MAGENTA);
//	ready(create(shell, INITSTK, INITPRIO, "PSHELL0", 3, TTY1, TTY1, TTY1), RESCHED_NO);

	while(1);

}

void outproc1(char *text, int fg, int pane) {
	char *str = NULL;
	int count = 0; 
	int fd = 0;

	enable();

	str = (char *)memget(256);

	if ((char *)SYSERR == str)
		return;

	fd = pane + 1;
		
//	drawRect(TWO_PANE1_ULCOL, TWO_PANE1_ULROW, TWO_PANE1_LRCOL, TWO_PANE1_LRROW, LEAFGREEN); 
	
//	open(TTY1, PANE1);
	open(fd, TWO_PANE1_ULROW, TWO_PANE1_ULCOL, TWO_PANE1_LRROW, TWO_PANE1_LRCOL, WHITE, CYAN);
//	ready(create(shell, INITSTK, INITPRIO, "PSHELL1", 3, TTY1, TTY1, TTY1), RESCHED_NO);

	while(1) {
		sprintf(str, "Process %d says %s\n", gettid(), text);
		write(fd, str, strlen(str));
		sleep(10 * pane);
	}
}


void openPanes(int num) {
	if (num == 2) {

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
		
		ready(create((void *)outproc0, INITSTK, 10, "outproc0", 3, "AAA", BLUE, 0), RESCHED_NO);
		
		ready(create((void *)outproc1, INITSTK, 10, "outproc1", 3, "BBB", GREEN, 0), RESCHED_NO);
	
		

		
		

	}

	else if (num == 3) {
		drawRect(THR_PANE0_ULCOL, THR_PANE0_ULROW, THR_PANE0_LRCOL, THR_PANE0_LRROW, LEAFGREEN);
		drawRect(THR_PANE1_ULCOL, THR_PANE1_ULROW, THR_PANE1_LRCOL, THR_PANE1_LRROW, LEAFGREEN);
		drawRect(THR_PANE2_ULCOL, THR_PANE2_ULROW, THR_PANE2_LRCOL, THR_PANE2_LRROW, LEAFGREEN);
	
	}
	else if (num == 4) {				
		drawRect(FOR_PANE0_ULCOL, FOR_PANE0_ULROW, FOR_PANE0_LRCOL, FOR_PANE0_LRROW, LEAFGREEN);
		drawRect(FOR_PANE1_ULCOL, FOR_PANE1_ULROW, FOR_PANE1_LRCOL, FOR_PANE1_LRROW, LEAFGREEN);
		drawRect(FOR_PANE2_ULCOL, FOR_PANE2_ULROW, FOR_PANE2_LRCOL, FOR_PANE2_LRROW, LEAFGREEN);
		drawRect(FOR_PANE3_ULCOL, FOR_PANE3_ULROW, FOR_PANE3_LRCOL, FOR_PANE3_LRROW, LEAFGREEN);
	}
}

void closePanes() {

}




#endif /* FRAMEBUF */
