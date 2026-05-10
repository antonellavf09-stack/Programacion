int registrarProducto(char nombre[5][20], int cantidades[5], float tiempo[5], int recurso[5], int cont);
float validarFloatConRango(float a, float b);
int validarEnteroConRango(int a, int b);
void leerCadena(char cadena[20]);
int menu();
void editarProducto(char nombres[5][20], int cantidades[], float tiempos[], int recursos[]);
void eliminarProducto(char nombres[5][20], int cantidades[], float tiempos[], int recursos[]);
void calcularTotales(int cantidades[], float tiempos[], int recursos[], float *tiempoTotal, int *recursosTotales);
void verificarProduccion(int cantidades[5], float tiempo[5], int recurso[5]);