#include <stdio.h>
#define Num_Alumno 2
#define Num_Calificaciones 3

int main(int argc, char *argv[])
{

    float alumno[Num_Calificaciones][Num_Alumno];
    float prom[Num_Alumno] = {0.0};

    for (int i = 0; i < Num_Alumno; i++)
    {

        printf("Ingrese las calificaciones del alumno %d:\n", i + 1);
    
        for (int j=0; j < Num_Calificaciones; j++);{
            
            printf("Nota %d: ",j + 1);
            scanf("%f", &alumno[j][i]);
        }
        
    }
    for (int i = 0; i < Num_Calificaciones; i++)
    {
        for (int j = 0; j < Num_Alumno; j++)
        {

            prom[j] += alumno[i][j];
        }
        for(int i=0; i < Num_Alumno; i++){
            
            prom[i]/=Num_Calificaciones;
            printf("El promedio del alumno %d es: %.2f\n", i + 1, prom[i]);
        }
    }
    return 0;
}