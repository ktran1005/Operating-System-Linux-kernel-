#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAXSIZE 1300

int main(void)
{
	char *val_id = "14377220";
	char *inv_id = "8675309";
	char *file = "/dev/fakedrive";
	char buf[MAXSIZE];
	int fd1, fd2, fd3;
	int ret_read;
	int ret_write1;
	int ret_write2;

	printf("Opening: %s\n", file);
	fd1 = open(file, O_RDONLY);
	if (fd1 == -1) {
		fprintf(stderr, "failed to open file\n");
		exit(EXIT_FAILURE);
	}

	printf("Reading from: %s\n", file);
	ret_read = read(fd1, buf, 1024);
	if (ret_read == -1)
		perror("Error: ");
	printf("Received %d bytes ", ret_read);
	printf("(raw): ");
	for (int i = 0; i < ret_read; i++)
		printf("%02x ", buf[i]);

	printf("\n");
	buf[ret_read] = '\0';
	printf("\t\t(ascii): %s\n", buf);
	printf("Closing: %s\n", file);
	printf("---------\n");
	printf("\n");
	close(fd1);
	printf("Opening: %s\n", file);
	fd2 = open(file, O_WRONLY);
	if (fd2 == -1) {
		fprintf(stderr, "failed to open file %s\n", file);
		exit(EXIT_FAILURE);
	}
	ret_write1 = write(fd2, inv_id, 8);
	if (ret_write1 == -1) {
		printf("errno value: %d\n", errno);
		printf("write \"%s\" to: %s\n", inv_id, file);
		perror("write() ");
		printf("Closing: %s\n", file);
		printf("---------\n");
		printf("\n");
	}
	close(fd2);
	printf("Opening: %s\n", file);
	fd3 = open(file, O_WRONLY);
	if (fd3 == -1) {
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	ret_write2 = write(fd3, val_id, 8);
	if (ret_write2 == -1) {
		printf("errno value: %d\n", errno);
		perror(" ");
		exit(EXIT_FAILURE);
	}
	printf("Writing \"%s\" to: %s\n", val_id, file);
	printf("Success!\n");
	printf("Closing: %s\n", file);
	printf("---------\n");
	close(fd3);

	return 0;
}
