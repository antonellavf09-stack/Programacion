#include<stdio.h>
#include<string.h>


int validarEnteroConRango(int a, int b){
    int n;
    int aux;
    do
    {
        aux = scanf("%d",&n);
        while((getchar()) != '\n');
        if(aux != 1 || n < a || n > b){
            printf("El valor ingresado es incorrecto\n");
            printf(">> ");
        }
    } while (aux!=1 || n < a || n > b);
    return n;
}


float validarFloatConRango(float a, float b){
    float n;
    int aux;
    do
    {
        aux = scanf("%f",&n);
        while((getchar()) != '\n');
        if(aux != 1 || n < a || n > b){
            printf("El valor ingresado es incorrecto\n");
            printf(">> ");
        }
    } while (aux!=1 || n < a || n > b);
    return n;
}

int menu(){
    int opc=0;
    printf("");
    printf("\n\n\t----------Opciones-----------\n");
    printf("\t1. Agregar producto\n");
    printf("\t2. Editar producto\n");
    printf("\t3. Eliminar producto\n");
    printf("\t4. Calcular produccion\n");
    printf("\t5.Salir\n");
    printf(">> ");
    opc=validarEnteroConRango(1,5);

    return opc;
}

void leerCadena(char cadena[20]){
    int len;
    fgets(cadena,20,stdin);
    len = strlen(cadena) - 1;
    cadena[len] = '\0';
}

int registrarProducto(char nombre[5][20], int cantidades[5], float tiempo[5], int recurso[5], int cont){

    if(cont >= 5){
    printf("No se pueden registrar mas productos.\n");
    return cont;
    }

    printf("Ingrese el nombre del producto %d: ",cont+1);
    leerCadena(nombre[cont]);
    printf("Ingrese la cantidad del producto %d: ",cont+1);
    cantidades[cont] = validarEnteroConRango(0,10000);
    printf("Ingrese el tiempo de produccion %d: ",cont+1);
    tiempo[cont] = validarFloatConRango(0,1000);
    printf("Ingrese el numero de recursos necesarios  %d: ",cont+1);
    recurso [cont] = validarEnteroConRango(0,10000);
   
    return cont + 1;
}


int buscarProducto(char nombres[5][20], char nombreBuscado[]){
    for(int i = 0; i < 5; i++){
        if(strcmp(nombres[i], nombreBuscado) == 0){
            return i;
        }
    }
    return -1;
}

void editarProducto(char nombres[5][20], int cantidades[5], float tiempos[5], int recursos[5]) {
    char buscar[20];
    printf("Nombre del producto a editar: ");
    leerCadena(buscar);

    int pos = buscarProducto(nombres, buscar);
    if(pos == -1){
        printf("Producto no encontrado.\n");
        return;
    }

    char nuevo[20];

    while(1){
        printf("Nuevo nombre: ");
        leerCadena(nuevo);

        int existe = 0;
        for(int i = 0; i < 5; i++){
            if(i != pos && strcmp(nombres[i], nuevo) == 0){
                existe = 1;
                break;
            }
        }

        if(existe){
            printf("Error: ya existe un producto con ese nombre. Intente otro.\n");
        } else {
            break;
        }
    }

    strcpy(nombres[pos], nuevo);

    printf("Nueva cantidad: ");
    cantidades[pos] = validarEnteroConRango(1,10);

    printf("Nuevo tiempo: ");
    tiempos[pos] = validarFloatConRango(1,10);

    printf("Nuevo recurso: ");
    recursos[pos] = validarEnteroConRango(1,10);
}
void eliminarProducto(char nombres[5][20], int cantidades[5], float tiempos[5], int recursos[5]) {
    char buscar[20];
    printf("Nombre del producto a eliminar: ");
    leerCadena(buscar);

    int pos = buscarProducto(nombres, buscar);
    if(pos == -1){
        printf("Producto no encontrado.\n");
        return;
    }
    nombres[pos][0] = '\0';
    cantidades[pos] = 0;
    tiempos[pos] = 0;
    recursos[pos] = 0;

    printf("Producto eliminado.\n");
}

void calcularTotales(int cantidades[], float tiempos[], int recursos[], float *tiempoTotal, int *recursosTotales){
    *tiempoTotal = 0;
    *recursosTotales = 0;

    for(int i = 0; i < 5; i++){
        *tiempoTotal += cantidades[i] * tiempos[i];
        *recursosTotales += cantidades[i] * recursos[i];
    }
}

void verificarProduccion(int cantidades[5], float tiempo[5], int recurso[5])
{
    float tiempoDisponible;
    int recursosDisponibles;

    float tiempoTotal = 0;
    int recursosTotales = 0;

    printf("\n--- CALCULO DE PRODUCCION ---\n");

    printf("Ingrese el tiempo disponible de la fabrica: ");
    tiempoDisponible = validarFloatConRango(0,100000);

    printf("Ingrese los recursos disponibles: ");
    recursosDisponibles = validarEnteroConRango(0,100000);

    calcularTotales(cantidades, tiempo, recurso, &tiempoTotal, &recursosTotales);

    printf("\nTiempo total requerido: %.2f\n", tiempoTotal);
    printf("Recursos totales requeridos: %d\n", recursosTotales);

    if(tiempoTotal <= tiempoDisponible &&
       recursosTotales <= recursosDisponibles)
    {
        printf("\nLa fabrica SI puede cumplir con la demanda.\n");
    }
    else
    {
        printf("\nLa fabrica NO puede cumplir con la demanda.\n");
    }
}
