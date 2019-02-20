// '.' == 46
// '#' == 35               (46 - c) / 10 -->  # becomes 1 and . becomes 0

#include <stdlib.h>
#include <unistd.h> // for system call write
#include <fcntl.h> // for system call open
#include <string.h> // for memmove and strlen
#include <stdio.h> // for debug printf
#include <stdbool.h> // to use bool type

void	display_error(char *s, int fd) // display error
{
	write(fd, s, strlen(s));
}

#define BUFFER_SIZE 1024
char	*read_file(char *av)
{
	char	buf[BUFFER_SIZE];
	int		fd;
	int		ret;
	char	*result;

	if (((fd = open(av, O_RDONLY)) < 0) \
        || ((ret = read(fd, &buf, BUFFER_SIZE)) < 0) \
        || !(result =  calloc(0, sizeof(char) * (BUFFER_SIZE)))) // calloc = malloc + bzero
		return (NULL);
	buf[ret] = '\0';
	memmove(result, buf, ret);
	close(fd);
	return (result);
}

#define T_SIZE 4*5

void show_tetriminos(short tetriminos_nb, char tetriminos[tetriminos_nb][16]) {
	short count;
	short i;

	count = 0;
	while (count < tetriminos_nb)
	{
		i = 0;
		while (i < 16)
		{
			write(1, &tetriminos[count][i], 1);
			if (i++ % 4 == 3)
				write(1, "\n", 1);
		}
		write(1, "\n", 1);
		++count;
	}
}

static	bool	check_tetrimino_is_valid(char tetrimino[16])
{
	short	i;
	short	links_bn;
	short	tetrimino_blocks;
	bool	all_blocks_connected;

	i = 0;
	tetrimino_blocks = 0;
	while (i < 16)
	{
		if (tetrimino[i] == '#')
		{
			links_nb = 0; // reset links_nb to 0
			links_nb += (i % 4 != 0 && (tetrimino[i - 1] == '#')) // if not first column
			links_nb += (i % 4 != 3 && (tetrimino[i + 1] == '#') // if not last column
			links_nb += (i / 4 != 0 && (tetrimino[i - 4] == '#')) // if not first row
			links_nb += (i / 4 != 3 && (tetrimino[i + 4] == '#')) // if not last row
			if (links_nb == 0)
				return false;
			if (links_nb > 1) // this condition is to unvalidate if we have two separate blocks like .##. .... .... and .##. further
				all_blocks_connected = true;
			++tetrimino_blocks;
		}
		++i;
	}
	return (tetrimino_blocks == 4 && all_blocks_connected); // Tetriminos should contain exactly 4 '#'
}

bool	are_all_tetriminos_valid(short tetriminos_nb, char tetriminos[tetriminos_nb][16]) {
	short 	count;

	count = 0;
	while (count < tetriminos_nb) // iterate through all tetriminos
	{
		if (check_tetrimino_is_valid(tetriminos[count++]) == false) // if one of them if invalid display error message
		{
			display_error("Tetrimino is invalid\n", 2);
			return false;
		}
	}
	return true;
}

void transform_input_into_tetriminos(short tetriminos_nb, char *str) {
	short 	i;
	short 	count;
	char	tetriminos[tetriminos_nb][16];
	int		input_block;

	count = 0;
	while (count < tetriminos_nb) // iterate through all tetriminos
	{
		input_block = count * 21; // each tetrimino is 21 characters in the input, due to '\n'
		i = 0;
		while (i < 20)
		{
			while (i % 5 < 4) // 4 first characters are not '\n'
			{
				c = ; // str[0] then str[21] etc for very first character of each tetri
				if (str[i + input_block] != '#' && str[i + input_block] != '.') // these 4 first characters should be '#' or '.'
					display_error("Invalid character in Tetriminos\n", 2);
				++i;
			}
			strncpy((char *)tetriminos + count * 16 + (i / 5) * 4, str + input_block + i - 4, 4); // we copy 4 characters from char *str to char *tetriminos[]
			if (str[i++ + input_block] != '\n') //5th character is a '\n'
				display_error("Missing Line Break between rows\n", 2);
		}
		if (++count < tetriminos_nb && str[20 + input_block] != '\n')
			display_error("Missing Line Break at the end of Tetrimino\n", 2);

	}
	show_tetriminos(tetriminos_nb, tetriminos);
	bool check = are_all_tetriminos_valid(tetriminos_nb, tetriminos);
	printf("valid ? %d\n", check);
}

int main(int argc, char **argv)
{
    char		*input;
    short       i;

    if (argc == 2)
    {
		if (!(input = read_file(argv[1]))) // error, invalid file
		      display_error("Could not read file\n", 2);
        else if (((i = (strlen(input) + 1) / (T_SIZE + 1)) == 0) ||
			((strlen(input) + 1) % (T_SIZE + 1)) != 0)
			display_error("Invalid file\n", 2);
		transform_input_into_tetriminos(i, input);
    }
	else
		display_error("Please submit a file\n", 2);

	return 0;
}
