#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

// Funcion Convertir el estado del libro a mayuscula

void ConvertirEstado(char *estado)
{
    if (*estado == 's')
    {
        *estado = 'S';
    }
    else if (*estado == 'p')
    {
        *estado = 'P';
    }
}

// Funcion para mostrar el menu de opciones
int menu()
{
    printf("\tSeleccione una opcion:\n");
    printf("1. Registrar libro\n");
    printf("2. Mostrar libros\n");
    printf("3. Buscar libro por Titulo o ID\n");
    printf("4. Actualizar estado del libro\n");
    printf("5. Eliminar libro\n");
    printf(">> ");
    int opc = validarIntRango(1, 5);
    return opc;
}

// Funcion para crear un libro
void CrearLibro(Libro *libros, int *n)
{
    // Restriccion de 10 libros
    if (*n >= 10)
    {
        printf("Error: No se pueden registrar mas de 10 libros\n");
        return;
    }
    printf("Ingrese el ID del libro %d: ", *n);
    libros[*n].ID = validarIntRango(1, 999999);

    // Validar que el ID del libro no se repita
    for (int i = 0; i < *n; i++)
    {
        if (libros[i].ID == libros[*n].ID)
        {
            printf("Error: El ID del libro ya existe\n");
            return;
        }
    }

    printf("Ingrese el titulo del libro %d: ", *n);
    leerCadena(libros[*n].titulo, 100);

    printf("Ingrese el autor del libro %d: ", *n);
    leerCadena(libros[*n].autor, 50);

    printf("Ingrese el anio del libro %d: ", *n);
    libros[*n].anio = validarIntRango(1000, 2026);

    printf("Ingrese el estado del libro %d (S para disponible, P para prestado): ", *n);
    scanf(" %c", &libros[*n].estado);

    // Limpiar el buffer de entrada
    while ((getchar()) != '\n')
        ;

    // Funcion Validacion
    ConvertirEstado(&libros[*n].estado);

    // Validar que el estado del libro sea correcto
    if (libros[*n].estado != 'S' && libros[*n].estado != 'P')
    {
        printf("Error: El estado del libro es incorrecto\n");
        return;
    }

    (*n)++;

    // Mensaje de libro registrado exitosamente
    printf("Libro registrado exitosamente\n");

    // Preguntar si desea registrar otro libro
    printf("Desea registrar otro libro? 1. si/ 2. no: ");
    int opc = validarIntRango(1, 2);
                                
    // Si el usuario desea registrar otro libro, llamar a la funcion CrearLibro nuevamente
    if (opc == 1)
    {
        CrearLibro(libros, n);
    }
    else if (opc == 2)
    {
        printf("Regresando al menu principal...\n");
        return;
    }
}

// Funcion para mostrar los libros registrados
void mostrarLibros(Libro *libros, int *n)
{
    // Si no hay libros registrados, mostrar un mensaje de error y regresar
    if (*n == 0)
    {
        printf("La biblioteca esta vacia\n");
        return;
    }

    printf("#\t\tID\t\tTitulo\t\tAutor\t\tAnio\t\tEstado\n");

    // Recorrer el arreglo de libros y mostrar los datos de cada libro
    for (int i = 0; i < *n; i++)
    {
        printf("%d\t\t%d\t\t%s\t\t%s\t\t%d\t\t%c\n", i + 1, libros[i].ID, libros[i].titulo, libros[i].autor, libros[i].anio, libros[i].estado);
    }

    // Preguntar si desea ver el detalle de un libro
    printf("Desea ver el detalle de un libro? 1. si/ 2. no: ");
    int opc = validarIntRango(1, 2);

    // Si el usuario desea ver el detalle de un libro, pedir el numero del libro y mostrar los datos del libro seleccionado
    if (opc == 1)
    {
        // Pedir el numero del libro que desea ver
        printf("Ingrese el numero del libro que desea ver: ");
        // Validar que el numero del libro sea correcto
        int num = validarIntRango(1, *n);

        // Mostrar los datos del libro seleccionado
        printf("ID: %d\n", libros[num - 1].ID);
        printf("Titulo: %s\n", libros[num - 1].titulo);
        printf("Autor: %s\n", libros[num - 1].autor);
        printf("Anio: %d\n", libros[num - 1].anio);
        printf("Estado: %c\n", libros[num - 1].estado);
    }
}

//Funcion para buscar un libro por titulo o ID
int buscarLibro(Libro *libros, int *n)
{
    // Si no hay libros registrados, mostrar un mensaje de error y regresar -1
    if (*n == 0)
    {
        printf("La biblioteca esta vacia\n");
        return -1;
    }

    // Pedir al usuario que ingrese el titulo o ID del libro que desea buscar
    char busqueda[100];
    int index = 0, f = 0;
    printf("Ingrese el titulo o ID del libro que desea buscar: ");
    leerCadena(busqueda, 100);

    // Recorrer el arreglo de libros y buscar el libro que coincida con el titulo o ID ingresado por el usuario
    for (int i = 0; i < *n; i++)
    {
        // Si el libro coincide con el titulo o ID ingresado por el usuario, guardar el indice del libro encontrado y salir del ciclo
        if (strcmp(busqueda, libros[i].titulo) == 0 || atoi(busqueda) == libros[i].ID)
        {
            index = i;
            f = 1;
            break;
        }
    }
    // Si no se encuentra el libro, mostrar un mensaje de error y regresar -1
    if (f == 0)
    {
        printf("Libro no encontrado\n");
        return -1;
    }

    // Si se encuentra el libro, mostrar los datos del libro encontrado y regresar el indice del libro encontrado
    printf("\t----Libro encontrado----\n");
    printf("ID: %d | Titulo: %s | Autor: %s | Anio: %d | Estado: %c\n", libros[index].ID, libros[index].titulo, libros[index].autor, libros[index].anio, libros[index].estado);

    return index;
}

// Funcion para actualizar el estado de un libro
void actualizarEstado(Libro *libros, int *n)
{
    // Buscar el libro que desea actualizar el estado
    int index = buscarLibro(libros, n);

    // Si no se encuentra el libro, regresar
    if (index != -1)
    {
        printf("Ingrese el nuevo estado del libro (S para disponible, P para prestado): ");
        scanf(" %c", &libros[index].estado);

        // Limpiar el buffer de entrada
        while ((getchar()) != '\n')
            ;
        
        // Funcion Validacion
        ConvertirEstado(&libros[index].estado);
         // Validar que el estado del libro sea correcto
        if (libros[index].estado != 'S' && libros[index].estado != 'P')
        {
            printf("Error: El estado del libro es incorrecto\n");
            return;
        }
        
        printf("Estado del libro actualizado exitosamente\n");
    }
}

// Funcion para eliminar un libro
void eliminarLibro(Libro *libro, int *n)
{
    // Buscar el libro que desea eliminar
    int index = buscarLibro(libro, n);

    // Si no se encuentra el libro, regresar
    if (index != -1)
    {
        // Mover los libros posteriores al libro eliminado una posicion hacia adelante
        for (int i = index; i < *n - 1; i++)
        {
            libro[i] = libro[i + 1];
        }
        (*n)--;
        printf("Libro eliminado exitosamente\n");
    }
}

int validarIntRango(int a, int b)
{
    int n;
    int aux;
    do
    {
        aux = scanf("%d", &n);
        while ((getchar()) != '\n')
            ;
        if (aux != 1 || n < a || n > b)
        {
            printf("Error: El valor ingresado es incorrecto\n");
            printf("Vuelva a ingresarlo: ");
        }
    } while (aux != 1 || n < a || n > b);
    return n;
}

void leerCadena(char *cadena, int n)
{
    int len;
    fgets(cadena, n, stdin);
    len = strlen(cadena) - 1;
    cadena[len] = '\0';
}