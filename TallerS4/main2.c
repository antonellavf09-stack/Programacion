#include <stdio.h>
#include "funciones2.h"

int main()
{
    char nombre[10][20];
    float precio[10];
    int stock[10];

    int opc2 = 0; // contador de productos
    int opc1 = 0; // opcion del menu

    do
    {
        opc1 = menu();

        switch(opc1)
        {
            //  OPCION 1 
            case 1:
                if(opc2 < 10)
                {
                    opc2 = registrarProducto(nombre, precio, stock, opc2);
                }
                else
                {
                    printf("Inventario lleno\n");
                }
                break;

            //  OPCION 2 
            case 2:
                VerInventario(nombre, precio, stock, opc2);
                break;

            //   OPCION 3 
            case 3:
                productoCaroBarato(nombre, precio, opc2);
                break;

            //  OPCION 4 
            case 4:
                buscarProducto(nombre, precio, opc2);
                break;
        }

    }while(opc1 != 0);

    return 0;
}