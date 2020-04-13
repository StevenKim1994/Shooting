	#include <stdio.h>
    #include <stdlib.h>


int main()
{
 
    int* a = (int*)malloc(sizeof(int) * 4);
	int* b = (int*)malloc(sizeof(int) * 16);
	
	for (int i = 0; i < 16; i++)
	{
		b[i] = 15-i;
		printf("%d ", b[i]);

	}
	printf("\n");

	int p = 2, q = 1, w = 2, h = 2;
	
	for (int i = 0; i < h; i++)
		for(int j = 0; j<w; j++)
				a[w * j + i] = b[4 * (q + j) + p + i];//memcpy(&a[w*j+i], &b[4 *  (q+j) + p+i], sizeof(int)*2);
	

	for (int i = 0; i < 4; i++)
		printf("%d", a[i]);
	

	getchar();
	free(a);
	free(b);

    return 0;
}