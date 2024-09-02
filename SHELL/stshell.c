#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_CMD_LEN 1024
#define MAX_ARG_NUM 16

int cd( char *argv[]){

    if (chdir(argv[1]) != 0) {
        perror("chdir");
        return 1;
    }

    return 0;
}

int main(void)
{
	char cmd[MAX_CMD_LEN] = {0};
	char *argv[MAX_ARG_NUM] = {NULL};
	signal(SIGINT, SIG_IGN);
	while (true)
	{
		printf("stshell> ");
		fflush(stdout);
		fgets(cmd, MAX_CMD_LEN, stdin);
		cmd[strlen(cmd) - 1] = '\0';
		int i = 0;
		char *token = strtok(cmd, " ");
		while (token != NULL)
		{
			argv[i++] = token;
			token = strtok(NULL, " ");
		}
		argv[i] = NULL;
		if (argv[0] == NULL)
			continue;
		if (strcmp(argv[0], "exit") == 0)
			break;
		if(strcmp(argv[0], "cd") == 0){
			if(i>=2){
				cd(argv);
				continue;
			}
		}
		int pipe_number = 0, redirect_status = 0;
		int pipe_location[2] = {0};
		int fd_loc;
		for (int i = 0; i < MAX_ARG_NUM; i++)
		{
			if (argv[i] == NULL)
				break;
			if (strcmp(argv[i], "|") == 0)
			{
				if (pipe_number < 2)
				{
					pipe_location[pipe_number] = i;
					pipe_number++;
					argv[i] = NULL;
				}
				else
				{
					fprintf(stderr, "pipe number is over 2\n");
					return EXIT_FAILURE;
				}
			}
			else if (strcmp(argv[i], ">") == 0)
			{
				redirect_status = 1;
				fd_loc = i;
				argv[i] = NULL;
				break;
			}
			else if (strcmp(argv[i], ">>") == 0)
			{
				redirect_status = 2;
				fd_loc = i;
				argv[i] = NULL;
				break;
			}
			else if (strcmp(argv[i], "<") == 0)
			{
				redirect_status = 3;
				fd_loc = i;
				argv[i] = NULL;
				break;
			}
		}
		pid_t pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			if (!redirect_status && pipe_number == 0)
			{
				execvp(argv[0], argv);
				perror("execvp");
				exit(1);
			}
			else if (redirect_status)
			{
				int fd = 0;
				switch (redirect_status)
				{
				case 1:
				{
					fd = open(argv[fd_loc + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
					break;
				}
				case 2:
				{
					fd = open(argv[fd_loc + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
					break;
				}
				case 3:
				{
					fd = open(argv[fd_loc + 1], O_RDONLY);
					break;
				}
				default:
				{
					printf("redirect status error\n");
					return EXIT_FAILURE;
				}
				}
				if (fd == -1)
				{
					perror("open");
					return EXIT_FAILURE;
				}
				if (dup2(fd, redirect_status == 3 ? STDIN_FILENO : STDOUT_FILENO) == -1)
				{
					perror("dup2");
					return EXIT_FAILURE;
				}
				close(fd);
				execvp(argv[0], argv);
				perror("execvp");
				exit(1);
			}
			else if (pipe_number == 1)
			{
				int pipe_fd[2];
				if (pipe(pipe_fd) == -1)
				{
					perror("pipe");
					return EXIT_FAILURE;
				}
				pid_t pid2 = fork();
				if (pid2 == 0)
				{
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[0]);
					close(pipe_fd[1]);
					execvp(argv[0], argv);
					perror("execvp");
					exit(1);
				}
				else if (pid2 > 0)
				{
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
					close(pipe_fd[1]);
					execvp(argv[pipe_location[0] + 1], (argv + pipe_location[0] + 1));
					perror("execvp");
					exit(1);
				}
				else
				{
					perror("fork");
					return EXIT_FAILURE;
				}
			}
			else if (pipe_number == 2)
			{
				int pipe_fd[4];
				if (pipe(pipe_fd) == -1 || pipe(pipe_fd + 2) == -1)
				{
					perror("pipe");
					return EXIT_FAILURE;
				}
				pid_t pid2 = fork();
				if (pid2 == 0)
				{
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[0]);
					close(pipe_fd[1]);
					close(pipe_fd[2]);
					close(pipe_fd[3]);
					execvp(argv[0], argv);
					perror("execvp");
					exit(1);
				}
				else if (pid2 < 0)
				{
					perror("fork");
					return EXIT_FAILURE;
				}
				pid_t pid3 = fork();
				if (pid3 == 0)
				{
					dup2(pipe_fd[0], STDIN_FILENO);
					dup2(pipe_fd[3], STDOUT_FILENO);
					close(pipe_fd[0]);
					close(pipe_fd[1]);
					close(pipe_fd[2]);
					close(pipe_fd[3]);
					execvp(argv[pipe_location[0] + 1], (argv + pipe_location[0] + 1));
					perror("execvp");
					exit(1);
				}
				else if (pid3 > 0)
				{
					dup2(pipe_fd[2], STDIN_FILENO);
					close(pipe_fd[0]);
					close(pipe_fd[1]);
					close(pipe_fd[2]);
					close(pipe_fd[3]);
					execvp(argv[pipe_location[1] + 1], (argv + pipe_location[1] + 1));
					perror("execvp");
					exit(1);
				}
				else
				{
					perror("fork");
					return EXIT_FAILURE;
				}
			}
			else if (pipe_number > 2)
			{
				fprintf(stderr, "pipe number is over 2\n");
				return EXIT_FAILURE;
			}
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
		}
		else
		{
			perror("fork");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}