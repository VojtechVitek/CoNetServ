#include "conetserv.h"
#include "conetserv_posix.h"

char buffer[BUFFER_LENGTH];

int main()
{
	startCommand(0, "www.fres-solutions.com");

	for(;;)
		readCommand(0, buffer);

	return 0;
}
