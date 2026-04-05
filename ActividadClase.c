#include <stdio.h>
#define Max_Vehiculos 5

float tiempos[Max_Vehiculos];
int puestos[Max_Vehiculos];
int total;

int main()
{

    printf("Ingrese el total de vehiculos de la carrera (maximo %d): ", Max_Vehiculos);
    scanf("%i", &total);

    if (total > Max_Vehiculos) {
        total = Max_Vehiculos;
    }

    for (int i = 0; i < total; i++){
        
        printf("Ingrese el puesto de salida del vehiculo %d: ",i + 1);
        scanf("%i", &puestos[i]);
        
        printf("Ingrese el tiempo de llegada %d: ", i + 1);
        scanf("%f", &tiempos[i]);
    }
    for (int i = 0; i < total - 1; i++ ){
        for (int j = 0; j < total - i - 1; j++){
            if (tiempos[j] > tiempos[j + 1]){
                
                float Temptime = tiempos[j];
                tiempos[j] = tiempos[j + 1];
                tiempos[j + 1] = Temptime;

                int TempPuesto = puestos[j];
                puestos[j] = puestos[j + 1];
                puestos[j + 1] = TempPuesto;

            }
        }
    }
    printf ("\nLos tres primeros lugares son:\n");
    
    for (int i = 0; i < 3 && i < total; i++){
        
        printf("Puesto %d con tiempo %.2f\n", puestos[i],tiempos[i]);
    }
    return 0;
}
    