#include <stdio.h>
#include <stdarg.h>
void func(const char *str, ...)
{
	va_list ap;
	va_start(ap, str);
	vprintf(str, ap);
	va_end(ap);
}
int main(void)
{
	func("%d %c\n", 'a','a');	
}
