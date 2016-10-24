#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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
			close (pipefd [0]);
			if (dup2 (pipefd [1], 1) != 1) {
				fprintf (stderr, "dup of ls stdout failed\n");
				exit (3);
			}
			execl ("/bin/ls", "ls", "-l", NULL);
			fprintf (stderr, "ls execl failed\n");
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
			execl ("/usr/bin/wc", "wc", NULL);
			fprintf (stderr, "wc execl faile\n");
			exit (6);
	}
	wait (NULL);
	wait (NULL);
	exit (0);
}
