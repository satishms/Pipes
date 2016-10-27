#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void main () {
	int pipefd[2];
	if (pipe (pipefd)) {
		fprintf (stderr, "pipe failed\n");
		exit (1);
	}
	switch (fork ()) {
		case -1 :
			fprintf (stderr, "ls fork failed\n");
			exit (2);
		default : break;
		case 0 :
			if (-1 == close (pipefd [0]) {
				perror("close() failed: ");
				exit(-1);
			}
			if (dup2 (pipefd [1], 1) != 1) {
				fprintf (stderr, "dup2 of ls stdout failed\n");
				exit (3);
			}
			execl ("/bin/ls", "ls", "-l", (char*)NULL);
			fprintf (stderr, "ls execl failed.\n");
			/* Instead, do this */
			perror("ls execl failed: "); // Will print the english description of errno - set by the system call
			exit (4);
	}
	switch (fork ()) {
		case -1 :
			fprintf (stderr, "wc fork failed\n");
			exit (5);
		default : break;
		case 0 :
			close (pipefd [1]); 
			if (dup2 (pipefd[0], 0) != 0) {
				fprintf (stderr, "dup of wc stdin failed\n");
				exit (6);
			}
			execl ("/usr/bin/wc", "wc", (char*)NULL);
			fprintf (stderr, "wc execl faile\n");
			exit (6);
	}
	wait (NULL);
	wait (NULL);
	exit (0);
}
