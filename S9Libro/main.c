#include<stdio.h>
#include"funciones.h"

int main(int argc, char const *argv[])
{
    Libro libros[10];
    int n = 0, opc = 0, continuar = 1;

    do
    {
        opc = menu();

        switch (opc)
        {
        case 1:
             CrearLibro(libros, &n);
            break;
        case 2:
            mostrarLibros(libros, &n);
            break;
        case 3:
            buscarLibro(libros, &n);
            break;
        case 4:
            actualizarEstado(libros, &n);
            break;
        case 5:
            eliminarLibro(libros, &n);
            break;
        default:
            printf("Opcion no valida\n");
            break;
        }
        printf("Desea escoger otra opcion? 1. si/ 2. no: ");
        continuar = validarIntRango(1, 2);

    } while (continuar == 1);
    if(continuar == 2)
    {
        printf("Saliendo del programa...\n");
        printf("Biblioteca registrada:\n");
        mostrarLibros(libros, &n);
        printf("\nGracias por usar el programa\n");
    }
 return 0;
}