#include <stdio.h>
#include <string.h>
#include "funciones.h"

//  MENU 
int menu(){
    int opc;

    printf("Seleccione una opcion\n");
    printf("1. Registrar producto\n");
    printf("2. Ver inventario\n");
    printf("3. Encontrar el producto mas caro y mas barato\n");
    printf("4. Buscar producto por nombre\n");
    printf("0. Salir\n");
    printf(">> ");

    opc = ValidarEnteroconRango(0,4);

    return opc;
}

//  OPCION 1 
int registrarProducto(char nombre[10][20], float precio[10], int stock[10], int cont)
{
    printf("Ingrese el nombre del producto: ");
    leerCadena(nombre[cont],20);

    printf("Ingrese el precio del producto: ");
    precio[cont] = ValidarFlotanteconRango(0,10000);

    printf("Ingrese el stock del producto: ");
    stock[cont] = ValidarEnteroconRango(0,10000);

    return cont + 1;
}

//  OPCION 2 
void VerInventario(char nombre[10][20], float precio[10], int stock[10], int cont)
{
    float totalProducto = 0;
    float totalInventario = 0;
    float precioPromedio = 0;

    printf("#\tNombre\t\tStock\t\tPrecio\t\tTotal\n");

    for(int i = 0; i < cont; i++)
    {
        totalProducto = stock[i] * precio[i];
        totalInventario += totalProducto;
        precioPromedio += precio[i];

        printf("%d\t%s\t\t%d\t\t%.2f\t\t%.2f\n",
        i+1, nombre[i], stock[i], precio[i], totalProducto);
    }

    if(cont > 0)
    {
        precioPromedio = precioPromedio / cont;

        printf("Total inventario: %.2f\n", totalInventario);
        printf("Precio promedio: %.2f\n", precioPromedio);
    }
}

//  OPCION 3 
void productoCaroBarato(char nombre[10][20], float precio[10], int cont)
{
    int posCaro = 0;
    int posBarato = 0;

    if(cont == 0)
    {
        printf("No hay productos registrados\n");
        return;
    }

    for(int i = 1; i < cont; i++)
    {
        if(precio[i] > precio[posCaro])
        {
            posCaro = i;
        }

        if(precio[i] < precio[posBarato])
        {
            posBarato = i;
        }
    }

    printf("Producto mas caro: %s %.2f\n", nombre[posCaro], precio[posCaro]);
    printf("Producto mas barato: %s %.2f\n", nombre[posBarato], precio[posBarato]);
}

//  OPCION 4 
void buscarProducto(char nombre[10][20], float precio[10], int cont)
{
    char buscado[20];

    printf("Ingrese el nombre del producto a buscar: ");
    leerCadena(buscado,20);

    for(int i = 0; i < cont; i++)
    {
        if(strcmp(nombre[i], buscado) == 0)
        {
            printf("Producto encontrado\n");
            printf("Nombre: %s\n", nombre[i]);
            printf("Precio: %.2f\n", precio[i]);
            return;
        }
    }

    printf("Producto no encontrado\n");
}

//  FUNCIONES AUXILIARES 
void leerCadena(char cadena[], int n)
{
    fgets(cadena, n, stdin);
    cadena[strcspn(cadena,"\n")] = '\0';
}

int ValidarEnteroconRango(int a, int b)
{
    int n;
    int aux;

    do
    {
        aux = scanf("%d", &n);
        while(getchar() != '\n');

        if(aux != 1 || n < a || n > b)
        {
            printf("El valor ingresado es incorrecto\n");
            printf(">> ");
        }

    }while(aux != 1 || n < a || n > b);

    return n;
}

float ValidarFlotanteconRango(float a, float b)
{
    float n;
    int aux;

    do
    {
        aux = scanf("%f", &n);
        while(getchar() != '\n');

        if(aux != 1 || n < a || n > b)
        {
            printf("El valor ingresado es incorrecto\n");
            printf(">> ");
        }

    }while(aux != 1 || n < a || n > b);

    return n;
}