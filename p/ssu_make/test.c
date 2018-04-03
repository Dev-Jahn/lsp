#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "struct.h"
#include "util.h"
#include "patterns.h"
#include "pp.h"
int main()
{
	char a[100] = "mkset/circular";
	syntax(a);
}
