#include "main.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void _printf(const char *text)
{
	write(STDOUT_FILENO, text, strlen(text));
}
