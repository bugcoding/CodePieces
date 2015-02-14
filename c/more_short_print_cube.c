#include <stdio.h>
#include <stdlib.h>
int main(int len, char *argv[(scanf("%d", &len))])
{
	for (int i = -len, j = -len; j <= len && \
			((i == len + 1) && (j++, i = -len, putchar('\n')),\
				(abs(i) + abs(j) <= len) ? putchar('*') : putchar(' ')); i++);
	return 0;
}






