#include <stdio.h>
#include <string.h>

int IngresarNombre(char nom[5][20], int n);
void LeerCadena(char cadena[], int n);

int main()
{
    char nombre[5][20];

    int cont = 0, n = 0;

    printf("Cuantos nombres desea ingresar? : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
         cont = IngresarNombre(nombre, cont);
    }

    for(int i = 0; i < cont; i++)
    {
        printf(" %s",nombre[i]);
    }

    return 0;
}

int IngresarNombre(char nom[5][20], int n)
{
    printf("Ingrese el nombre de la persona %d: ", n);
    LeerCadena(nom[n],20);
    return n + 1;
}

void LeerCadena(char cadena[], int n)
{
    int len;
    getchar();
    fgets(cadena,n,stdin);
    len = strlen(cadena) - 1;
    cadena[len] = '\0';
}