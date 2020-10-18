#include "minilibc.h"
long write(int fd, void *buf, unsigned long count);
int main(void)
{
	char wmsg[]   = "Welcome to Echo program (written in C)!\n";
	char prompt[] = "> ";
	char token[512];
	char key[2];
	int i, p;
	write(1,wmsg,sizeof(wmsg));
	write(1,prompt,sizeof(prompt));
	p = 0;
	while (1) {
		/* Read a key */
		if(read(0,key,1)>0){
			switch (key[0]) {
				case '\r':
				case '\n':
					token[p] = '\0';
					int j = 0;
					while (token[j] != '\0' && j < p) {
						write(1, &token[j], 1);
						j++;
					}
					write(1,&"\n",1);
					write(1,prompt,sizeof(prompt));
					p = 0;
					break;
				default:
					token[p++] = key[0];
			}
		}
	}

	return 0;
}

