#include "shell.h"

/**
  * get_input - gets the user input command and save it in buff
  * @buff: the buffer
  * @args: the arg vector
  * Return: number of character entered
  */
ssize_t get_input(char **buff, char **args)
{
	size_t n;
	ssize_t m;

	n = 0;
	m = getline(buff, &n, stdin);

	/*if nothing is entered and 'enter' button pressed. Restart*/
	if (m == 1)
		execvp(args[0], args);
	/* if the input is piped or EOF, the next iteration, m is 0 */
	else if (m < 1)
	{
		free(*buff);
		/*if input is from the terminal, restart*/
		if (isatty(STDIN_FILENO))
			execvp(args[0], args);
		/*if it's piped, exit*/
		exit(0);
	}
	/**
	  * this is to handle EOF when ctrl-D is pressed
	  * after input since there'll be no newline
	 */
	if (m > 1 && *(*buff + m - 1) != '\n')
		*(*buff + m) =  '\0';
	else
	/*this overrides the newline at the end from getline()*/
		*(*buff + m - 1) = '\0';

	return (m);
}

/**
  * check_separator - checks for the presence of the command separator (;)
  * @buff: the input buffer containing the command string
  * Description: uses a static buffer to hold the initial command string
  * and then search for ; and uses strtok to return the next command found.
  * finally returns NULL if there are no ; left
  * Return: pointer to the next command or NULL
  */
char *check_separator(char *buff)
{
	static char *buff_hold, *save_ptr;
	static int counter;
	char *curr_comm, *curr_comm_cpy, *curr_comm_cpy2;

	if (buff != NULL && buff_hold == NULL)
	{
		buff_hold = str_dup(buff);
		free(buff);
	}

	if (buff_hold == NULL)
		return (NULL);

	if (counter == 0)
		curr_comm = strtok_r(buff_hold, ";", &save_ptr);
	else
		curr_comm = strtok_r(NULL, ";", &save_ptr);

	counter++;
	if (curr_comm == NULL)
	{
		free(buff_hold);
		buff_hold = save_ptr = NULL;
		counter = 0;

		return (NULL);
	}
	else
	{
		curr_comm_cpy = str_dup(curr_comm);
		curr_comm_cpy2 = str_dup(curr_comm);
		if (str_cmp(strtok(curr_comm_cpy, " "), "exit") == 0)
			free(buff_hold);
		free(curr_comm_cpy);

		return (curr_comm_cpy2);
	}
}

/**
  * free_args - frees the arg vector created by 'parse_command'
  * @args: the arg vector generated from user input
  * Return: nothing
  */
void free_args(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}

	free(args);
}

/**
  * free_args_exit - frees the arg vector and exit the shell
  * @args: the args vector
  * Return: nothing
  */
void free_args_exit(char **args)
{
	free_args(args);

	exit(0);
}
