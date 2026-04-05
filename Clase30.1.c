#include<stdio.h>

int main(int argc, char * argv[]){

    float alumno1[3];
    float alumno2[3];
    float prom[2]={0.0};

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

   for (int i = 0; i < 3; i++){
    
    prom[0] += alumno1[i];
     prom[1] += alumno2[i];
   }
   prom[0]/=3;
   prom[1]/=3;
    printf("El promedio del alumno 1 es: %.2f\n",prom[0]);
    printf("El promedio del alumno 2 es: %.2f",prom[1]);
}