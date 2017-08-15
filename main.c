#include <stdio.h>
#include "token.h"
#include "mygod.h"

int main()
{
	char buff[BUFF_MAX_SIZE] = {0};

	while(NULL != fgets(buff, BUFF_MAX_SIZE, stdin)) {
		YU_parse_line(buff);
	}
	
	return 0;
}
