#include<stdio.h>

int main(int argc, char * argv[]){

    float alumno1[3];
    float alumno2[3];
    float promedio1, promedio2;

    printf("Ingrese las calificaciones del alumno 1:\n");
    printf("Nota 1: ");
    scanf("%f",&alumno1[0]);
    printf("Nota 2: ");
    scanf("%f",&alumno1[1]);
    printf("Nota 3: ");
    scanf("%f",&alumno1[2]);

    printf("Ingrese las calificaciones del alumno 2:\n");
    printf("Nota 1: ");
    scanf("%f",&alumno2[0]);
    printf("Nota 2: ");
    scanf("%f",&alumno2[1]);
    printf("Nota 3: ");
    scanf("%f",&alumno2[2]);

    promedio1 = (alumno1[0] + alumno1[1] + alumno1[2])/3;
    promedio2 = (alumno2[0] + alumno2[1] + alumno2[2])/3;

    printf("El promedio del alumno 1 es: %.2f\n",promedio1);
     printf("El promedio del alumno 2 es: %.2f",promedio2);
}