#include<stdio.h>

int main()
{

    int arr[5];

    float a = 30;

    float *p;

    printf("El valor de a es: %.2f\n",a);

    p = &a;
    *p = 50;

    printf("---------------------\n");
    printf("El valor de a es: %.2f\n",a);
    
    int arr[5] = {37,50,5,0,1};

    int *q;

    for(int i = 0; i < 5; i++)
    {
        printf("%d\n",arr[i]);
    }

    q = arr;

    *(q + 1)  = 100;

}