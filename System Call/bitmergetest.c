#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>

void toBi(unsigned long n)
{
	int i;
	int a[64];

	for ( i = 0; n > 0; i++) {
		a[i] = n % 2;
		n = n / 2;
	}
	for ( i = i - 1; i >= 0; i--){
		printf("%d", a[i]);
	}
	printf("\n");
}


int main()
{
	int errno;
	unsigned long ret1;
	unsigned int high_bits1 = pow(2,31);
	unsigned int low_bits1 = 0b101;

	printf("Test 1\n");
	if ((ret1 = syscall(548, high_bits1, low_bits1)) == -1) {
		printf("The value (decimal) is %lu\n", ret1);
		printf("The value (hex) is %lx\n", ret1);
		printf("The value (binary) is ");
		toBi(ret1);
		printf("errno: %d\n", errno);
		perror("Error ");
		printf("TEST FAILED\n");
	}
	else {
		printf("The value (decimal) is %lu\n", ret1);
		printf("The value (hex) is %lx\n", ret1);
		printf("The value (binary) is ");
		toBi(ret1);
	}
	printf("----------------------------------------------\n");
	unsigned long ret2;
	unsigned int high_bits2 = 0x12345678;
	unsigned int low_bits2 = 0x87654321;

	printf("Test 2\n");
	if ((ret2 = syscall(548, high_bits2, low_bits2)) == -1) {
		printf("The value (decimal) is %lu\n", ret2);
		printf("The value (hex) is %lx\n", ret2);
		printf("The value (binary) is ");
		printf("errno: %d\n", errno);
		perror("Error ");
		printf("TEST FAILED\n");
	}
	else {
		printf("The value (decimal) is %lu\n", ret2);
		printf("The value (hex) is %lx\n", ret2);
		printf("The value (binary) is ");
		toBi(ret2);
		printf("TEST PASSED\n");
	}
	printf("------------------------------------------------\n");
	unsigned long ret3;
	unsigned int high_bits3 = 0x87654321;
	unsigned int low_bits3 = 0x10050103;

	printf("Test 3\n");
	if ((ret3 = syscall(548, high_bits3, low_bits3)) == -1) {
		printf("The value (decimal) is %lu\n", ret3);
		printf("The value (hex) is %lx\n", ret3);
		printf("The value (binary) is ");
		toBi(ret3);
		printf("errno: %d\n", errno);
		perror("Error ");
		printf("TEST FAILED\n");
	}
	else {
		printf("The value (decimal) is %lu\n", ret3);
		printf("The value (hex) is %lx\n", ret3);
		printf("The value (binary) is ");
		toBi(ret3);
		printf("TEST PASSED\n");
	}

	return 0;

}
