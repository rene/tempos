#include "minilibc.h"

int main(void)
{
	char wmsg[]   = "Welcome to Echo program (written in C)!\n";
	char prompt[] = "> ";
	char token[512];
	char key[2];
	int i, p;

	_printf(wmsg);
	_printf(prompt);
	p = 0;
	while (1) {
		/* Read a key */
		if (read(0, key, 1) > 0) {
			switch (key[0]) {
				case '\r':
				case '\n':
					token[p] = '\0';
					_printf("Echo: %s\n", token);
					_printf(prompt);
					p = 0;
					break;

				default:
					token[p++] = key[0];
			}
		}
	}

	for(;;);
	return 0;
}

