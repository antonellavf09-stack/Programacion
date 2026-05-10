//Taller de Programacion - Ejercicio 2
#include <stdio.h>

int main(int argc, char const *argv[])
{
    // Arreglo de calificaciones: [alumno][asignatura][nota]                                  
    float calificacion[5][3][3] = {0.0};

    // Nombres de hasta 5 alumnos
    char nombres[5][20];

    // Asignaturas manejadas
    char asignaturas[3][20] = {"Matematicas", "Fisica", "Quimica"};

    // Promedios por alumno y asignatura
    float promedios[5][3];

    // Bandera para saber si ya se registró al menos un estudiante
    int registro = 0;

    int opc = 0, opc2 = 0; // Opción del menú y control de repetición
    int contAlumnos = 0; // Contador de alumnos registrados

    do
    {
        // Mostrar el menú principal de opciones
        printf("\tSelecione una opcion:\n");
        printf("\t1. Registrar alumno\n");
        printf("\t2. Registar calificaciones\n");
        printf("\t3. Promedio por alumno\n");
        printf("\t4. Promedio por asignacion\n");
        printf("\t5. Calificacion mas alta y baja por asignatura\n");
        printf("\t6. Calificacion mas alta y baja por estudiante\n");
        printf("\t7. Estudiantes aprobados y reprobados por asignatura\n");
        printf(">> ");
        scanf("%d", &opc);
        printf("\n");

        while (opc < 0 || opc > 7)
        {
            printf("No puede ingresar negativos.");
            printf(">> ");
            printf("\n");
            scanf("%d", &opc);
        }

        switch (opc)
        {
        case 1:
            // Registrar nombre de un nuevo alumno
            printf("Ingrese el nombre del alumno %d: ", contAlumnos + 1);
            getchar(); // Limpiar el buffer de entrada
            fgets(nombres[contAlumnos], 20, stdin);
            contAlumnos++;
            registro = 1; // Ya hay al menos un alumno registrado
            break;

        case 2:
            // Registrar calificaciones para un alumno existente
            if (registro == 0)
            {
                printf("\tPrimero debe registar estudiantes. Seleccione 1 para registro.\n");
                break;
            }

            int selA = 0;
            printf("Seleccione el alumno que desee registrar las calificaciones:\n");
            for (int i = 0; i < contAlumnos; i++)
            {
                printf("%d.%s", i, nombres[i]);
            }
            printf(">> ");
            scanf("%d", &selA);

            for (int i = 0; i < 3; i++)
            {
                printf("Ingrese las notas de la materia %s\n", asignaturas[i]);
                promedios[selA][i] = 0;

                for (int j = 0; j < 3; j++)
                {
                    printf("Nota %d: ", j + 1);
                    scanf("%f", &calificacion[selA][i][j]);

                    while (calificacion[selA][i][j] < 0 || calificacion[selA][i][j] > 10)
                    {
                        printf("No se pueden ingresar negativos ni mayores a 10. Ingrese de nuevo:\n");
                        printf(">> ");
                        scanf("%f", &calificacion[selA][i][j]);
                    }
                    promedios[selA][i] += calificacion[selA][i][j];
                }
                promedios[selA][i] /= 3;
            }
            break;
        case 3:
            // Mostrar los promedios por alumno
            if (registro == 0)
            {
                printf("\tPrimero debe registar estudiantes. Seleccione 1 para registro.\n");
                break;
            }

            printf("\t----------------Calificaciones por alumno----------------\n");
            printf("%s\t\t%s\t\t%s\t\tNombre\n", asignaturas[0], asignaturas[1], asignaturas[2]);

            for (int i = 0; i < contAlumnos; i++)
            {
                printf("%.2f\t\t%.2f\t\t%.2f\t\t%s\n", promedios[i][0], promedios[i][1], promedios[i][2], nombres[i]);
            }
            break;
        case 4:
            // Calcular promedio general por cada asignatura
            if (registro == 0)
            {
                printf("\tPrimero debe registar estudiantes. Seleccione 1 para registro.\n");
                break;
            }

            printf("\t----------------Calificaciones por asignatura----------------\n");
            printf("Nombre\t\t%s\t\t%s\t\t%s\n", asignaturas[0], asignaturas[1], asignaturas[2]);

            for (int i = 0; i < 3; i++)
            {
                float promedioAsignatura = 0;
                for (int j = 0; j < contAlumnos; j++)
                {
                    promedioAsignatura += promedios[j][i];
                }
                promedioAsignatura /= contAlumnos;
                printf("%s: %.2f\n", asignaturas[i], promedioAsignatura);
            }

            break;
        case 5:

            if (registro == 0)
            {
                printf("\tPrimero debe registar estudiantes. Seleccione 1 para registro.\n");
                break;
            }

            for (int i = 0; i < 3; i++)
            {
                float mayor = promedios[0][i];
                float menor = promedios[0][i];

                for (int j = 1; j < contAlumnos; j++)
                {
                    if (promedios[j][i] > mayor)
                    {
                        mayor = promedios[j][i];
                    }

                    if (promedios[j][i] < menor)
                    {
                        menor = promedios[j][i];
                    }
                }
                printf("%s -> Mayor: %.2f      |    %s -> Menor: %.2f", asignaturas[i], mayor, menor);
            }
            break;
        case 6:
            // Calcular la mayor y menor nota para cada estudiante
            if (registro == 0)
            {
                printf("\tPrimero debe registar estudiantes. Seleccione 1 para registro.\n");
                break;
            }

            for (int i = 0; i < contAlumnos; i++)
            {
                float mayor = promedios[i][0];
                float menor = promedios[i][0];

                for (int j = 1; j < 3; j++)
                {
                    if (promedios[i][j] > mayor)
                    {
                        mayor = promedios[i][j];
                    }

                    if (promedios[i][j] < menor)
                    {
                        menor = promedios[i][j];
                    }
                }
                printf("%s -> Mayor: %.2f  |   %s -> Menor: %.2f", nombres[i], mayor, menor);
            }

            break;
        case 7:
            // Contar aprobados y reprobados por asignatura
            if (registro == 0)
            {
                printf("\tPrimero debe registar estudiantes. Seleccione 1 para registro.\n");
                break;
            }

            for (int i = 0; i < 3; i++)
            {
                int aprobados = 0;
                int reprobados = 0;
                for (int j = 0; j < contAlumnos; j++)
                {
                    if (promedios[j][i] >= 6)
                    {
                        aprobados++;
                    }
                    else
                    {
                        reprobados++;
                    }
                }
                printf("%s -> Aprobados: %d   |  %s -> Reprobados: %d\n", asignaturas[i], aprobados, reprobados);
            }

            break;

        default:
            // Manejar opción de menú inválida
            printf("Opcion no valida. Ingrese de nuevo.");
            printf(">> ");
            printf("\n");

            break;
        }

        printf("\nDesea seleccionar otra opcion? (1. Si / 2. No):\n");
        printf(">> ");
        scanf("%d", &opc2);

    } while (opc2 == 1);

    return 0;
}