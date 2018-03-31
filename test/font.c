#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL,"");
	wchar_t c = 0x2566;
	wchar_t *str = L"\u2566\u2566";
	wchar_t d = L'\u0040';
	/*wprintf(L"%c\n", L'\u0040');*/
	/*for (int i=0;i<1000;i++)*/
	char block1[] = { 0xe2, 0x94, 0x80, '\0' };
    char block2[] = { 0xe2, 0x94, 0x81, '\0' };
    printf("%s%s\n", block1, block2);
    return 0;
}

