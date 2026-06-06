typedef struct
{
    int ID;
    char titulo[100];
    char autor[50];
    int anio;
    char estado; // 'S' para disponible, 'P' para no disponible
}Libro;

int menu();
void leerCadena(char *cadena, int n);
int validarIntRango(int a, int b);
void ConvertirEstado(char *estado);
void CrearLibro(Libro *libros, int *n);
void mostrarLibros(Libro *libros, int *n);
int buscarLibro(Libro *libros, int *n);
void actualizarEstado(Libro *libros, int *n);
void eliminarLibro(Libro *libro, int *n);
