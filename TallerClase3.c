#include <stdio.h>
#define Num_Alumno 5
#define Num_Notas 3

float alumno[Num_Alumno][Num_Notas];
float prom[Num_Alumno]={0.0};

int main(){

   for(int i = 0; i < Num_Alumno; i++ ){
    for(int j = 0; j < Num_Notas; j++){

        printf("Estudiante %d:\n",i + 1);
        printf("Ingrese la nota del estudiante %d:", j + 1);
        scanf("%f", &alumno[j][i]);
    }
   }
   for (int i = 0; i < Num_Notas; i++){
        
       for (j = 0; j < Num_Alumno; j++){

        prom[j] += alumno[i][j];
    }
}
    for(int i = 0; i < Num_Alumno; i++){
        
        prom[i]/=Num_Notas;
        printf("El promedio del alumno %d es: %.2f\n", i + 1, prom[i]);
    }
   }
    
 return 0;
}