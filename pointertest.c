#include <stdio.h>
#include <stdlib.h>
int main()
{
	printf("Pick a size\n"); //get a size
	int *z = NULL; //set pointer to null
	z = malloc(sizeof(*z));
	scanf("%d", &(*z)); //get input
	int* x = calloc(*z,sizeof(int)); //dynamic array creation
	for (int i = 0; i < *z; i++) //loop through new array add values 
	{
		x[i] = i;
	}
	for (int w = 0; w < *z; w++) //print array 
	{
		printf("Array: %d\n" ,x[w]);
	}
	free(z); //free memory 
	return 0;
}
