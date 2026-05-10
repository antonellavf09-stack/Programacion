#include <stdio.h>
#include "funciones.h"

int main(int arg, char *argv[])
{
    int opc = 0;
    int opc2 = 0;
    float res = 0, n1 = 0, n2 = 0;

    opc = menu();

    do
    {
        switch (opc)
        {
        case 1:

            printf("Ingrese el numero 1: ");
            n1 = validarEnteroconRango(0,10);
            printf("Ingrese el numero 2: ");
            n2 = validarEnteroconRango(0,10);
            res = suma(n1, n2);
            printf("El resultado es: %.2f\n", res);
            break;
        case 2:
            printf("Ingrese el numero 1: ");
            scanf("%f", &n1);
            printf("Ingrese el numero 2: ");
            scanf("%f", &n2);
            res = resta(n1, n2);
            printf("El resultado es: %.2f\n", res);
            break;
        case 3:

            printf("Ingrese el numero 1: ");
            scanf("%f", &n1);
            printf("Ingrese el numero 2: ");
            scanf("%f", &n2);
            res = multiplicacion(n1, n2);
            printf("El resultado es: %.2f\n", res);
            break;

        case 4:

            printf("Ingrese el numero 1: ");
            scanf("%f", &n1);
            printf("Ingrese el numero 2: ");
            scanf("%f", &n2);

            if (n2 == 0)
            {
                printf("No se puede dividir para 0\n");
                scanf("%f", &n2);
            }
            res = division(n1, n2);
            printf("El resultado es: %.2f\n", res);
            break;

        case 5:

            printf("Ingrese el numero 1: ");
            scanf("%f", &n1);
            printf("Ingrese el numero 2: ");
            scanf("%f", &n2);
            res = potenciacion(n1, n2);
            printf("El resultado es: %.2f\n", res);
            break;

        case 6:

            printf("Ingrese el numero 1: ");
            scanf("%f", &n1);
            res = RaizCuadrada(n1);
            printf("El resultado es: %.2f\n", res);
            break;
        default:
            break;
        }
        printf("Seleccione una opcion 1. Si/ 2. No:\n");
        printf(">> ");
        scanf("%d", &opc2);
    } while (opc2 = 1);
    return 0;
}