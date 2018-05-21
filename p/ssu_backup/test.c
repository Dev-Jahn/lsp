#include <stdio.h>
#include "util.h"
#include <openssl/sha.h>

int main(void)
{
	int ret = cat("fuck.txt");
	printf("%d\n", ret);
}
