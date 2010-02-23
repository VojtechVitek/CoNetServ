#include <stdio.h>
#include "conetserv.h"
#include "conetserv_posix.h"

char buffer[BUFFER_LENGTH];

int main()
{
	startCommand(0, "www.fres-solutions.com");

	int len;

	for(;;) {
		if ((len = readCommand(0, buffer)) > 0)
			printf("%s", buffer);
	}

	return 0;
}
