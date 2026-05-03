#define FUNCIONES_H

// MENU
int menu();

// OPCION 1
int registrarProducto(char nombre[10][20], float precio[10], int stock[10], int cont);

// OPCION 2
void VerInventario(char nombre[10][20], float precio[10], int stock[10], int cont);

// OPCION 3
void productoCaroBarato(char nombre[10][20], float precio[10], int cont);

// OPCION 4
void buscarProducto(char nombre[10][20], float precio[10], int cont);

// FUNCIONES AUXILIARES
void leerCadena(char cadena[], int n);
int ValidarEnteroconRango(int a, int b);
float ValidarFlotanteconRango(float a, float b);