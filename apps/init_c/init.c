long write(int fd, void *buf, unsigned long count);

int main(void)
{
	char msg[] = "Hello World from C language!\n";

	write(1, msg, sizeof(msg));

	return 0;
}

