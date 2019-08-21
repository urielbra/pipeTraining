/**
 * Example program demonstrating UNIX pipes.
 *
 * Figures 3.25 & 3.26
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int conta(char x, char write_msg[BUFFER_SIZE]){
	int i;
	int resposta = 0;
	for (i=0 ; i < BUFFER_SIZE; i++){
		if(write_msg[i] == x){
			resposta++;
		}
	}
	// printf("Achei %d ocorrências de %c\n",resposta,x);
	return resposta;
}

int main(void)
{	
	char write_msg[BUFFER_SIZE] = "028371938561523591863574";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int pipeNumbers[2];
	int ansPipeNumbers[2];

	/* create the pipe */
	if (pipe(pipeNumbers) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	/* pipe to return values*/ 
	if (pipe(ansPipeNumbers) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	/* now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}
	if (pid > 0) {  /* parent process */
		printf("Hello, parent here!\n");
		// printf("Parent: %d\n",getpid());
		/* close the unused end of the pipe */
		close(pipeNumbers[READ_END]);
		close(ansPipeNumbers[WRITE_END]);
		sleep(5);
		/* write to the pipe */
		printf("Parent writing to the pipe: %s\n",write_msg);
		write(pipeNumbers[WRITE_END], write_msg, strlen(write_msg)+1); 
		// sleep(10);
		// printf("Parent closing the writing end of the pipe.\n");
		/* close the write end of the pipe */
		close(pipeNumbers[WRITE_END]);
		int finalResult;
		read(ansPipeNumbers[READ_END], &finalResult, sizeof(int));
		close(ansPipeNumbers[READ_END]);
		printf("Parent recieved result from pipe: %d\n", finalResult);

	}
	else { /* child process */

		printf("Hello, son here!\n");
		// printf("Child: %d\n",getpid());
		/* close the unused end of the pipe */
		close(pipeNumbers[WRITE_END]);
		close(ansPipeNumbers[READ_END]);

		/* read from the pipe */
		// printf("Child (%d) waiting data from the pipe\n",getpid());
		//read blocks client as long as there is a writer.
		printf("Child waiting data from pipe\n");
		//Child waits to read on Pipe
		read(pipeNumbers[READ_END], read_msg, BUFFER_SIZE);
		// sleep(8);
		// printf("My read_string: %s\n", read_msg);
		printf("Which number do you want to count 0-9?\n");
		int count;
		// getchar();
		scanf(" %d",&count);
		printf("\n\nChild will search %d on this string: %s\n",count,read_msg);
		printf("Parent now waiting for the result\n");
		int answer =  conta(('0' + count) , read_msg);
		printf("Child finish to count %c on %s.\n",('0' + count),read_msg);
		printf("Child found %d occourrences.\n",answer);
		close(pipeNumbers[READ_END]);

		//Child starts to return value
		write(ansPipeNumbers[WRITE_END], &answer, sizeof(int));	
		close(ansPipeNumbers[WRITE_END]);
	}

	return 0;
}
