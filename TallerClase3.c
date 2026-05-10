#include <stdio.h>
#define Num_Alumno 5
#define Num_Notas 3

float calificacion[5][3][3];
float nombres[5][20];
char asignaturas[3][20] = ("Matematicas","Fisica", "Quimica");

int opc = 0, opc2 = 2;
int contAlumnos = 0;

int main()
{

    do
    {
        
        printf("\tSellecione una opcion:\n");
        printf ("\t1. Registrar alumno\n");
        printf ("\t2. Registar calificaciones\n");
        printf("\n3. Promedio por alumno\n");
        printf("\t4. Promedio por asignacion\n");
        printf("\t5. Calificacion mas alta y baja por estudiante\n");
        printf("\t6. Calificacion mas alta y baja por estudiante\n");
        printf("\t7. Estudiantes aprobados y reprobados por asignatura\n");
        printf(">> ");
        scanf("%d", &opc);

        switch (opc)
        {
            case 1:

            printf("Ingrese el nombre del alumno %d: ", contAlumnos + 1);
            getchar();// Limpiar el buffer de entrada
            fgets(nombres[contAlumnos], 20, stdin);
            contAlumnos++;
            
            
            break;
        }

    } while (opc2 == 1);

 return 0;
}