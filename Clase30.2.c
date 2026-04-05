#include <stdio.h>

int main(int argc, char *argv[])
{

    float alumno[3][2];
    float prom[2] = {0.0};

    for (int i = 0; i < 2; i++)
    {

        printf("Ingrese las calificaciones del alumno:\n");
        printf("Nota 1: ");
        scanf("%f", &alumno[0][i]);
        printf("Nota 2: ");
        scanf("%f", &alumno[1][i]);
        printf("Nota 3: ");
        scanf("%f", &alumno[2][i]);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {

            prom[j] += alumno[i][j];
        }
    }
    prom[0] /= 3;
    prom[1] /= 3;

    printf("El promedio del alumno 1 es: %.2f\n", prom[0]);
    printf("El promedio del alumno 2 es: %.2f", prom[1]);
}