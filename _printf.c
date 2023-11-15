#include "main.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 *_printf - function to write to standard output
 *@text: the text to be written
 *Return: nothing
*/

void _printf(const char *text)
{
	write(STDOUT_FILENO, text, strlen(text));
}
