#include <stdio.h>
#include "funciones.h"

int main()
{
     int cantidad[5] = {0}, recurso[5] = {0}, cont = 0;
    char nombreProducto[5][20];
    int opc = 0;
    float tiempo[5] = {0};

    do
    {
        opc = menu();

        switch (opc)
        {
        case 1:
            cont = registrarProducto(nombreProducto, cantidad, tiempo, recurso, cont);
            break;

        case 2:
            editarProducto(nombreProducto, cantidad, tiempo, recurso);
            printf("\nProducto editado correctamente.\n");
            break;
        case 3:
            eliminarProducto(nombreProducto, cantidad, tiempo, recurso);
            break;

        case 4:
          verificarProduccion(cantidad, tiempo, recurso);
          break;

        case 5:
            printf("Salida exitosa.\n");
            return 0;

        default:
            printf("Opcion no existente.\n");
        }

    } while (opc != 5);
}