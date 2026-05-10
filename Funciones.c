#include <stdio.h>
#include <math.h>

int menu();
float suma(float a, float b);
float resta(float a, float b);
float multiplicacion(float a, float b);
float division(float a, float b);
float potenciacion(float a, float b);
float RaizCuadrada(float a);

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

int menu()
{

    int opc;

    printf("\tSeleccione una opcion:\n");
    printf("\t1. Suma\n");
    printf("\t2. Resta\n");
    printf("\t3. Multiplicacion\n");
    printf("\t4. Division\n");
    printf("\t5. Potencia\n");
    printf("\t6. Raiz Cuadrada\n");
    printf("\t>> ");
    scanf("%d", &opc);
    return opc;
}

float suma(float a, float b)
{
    float res;
    res = a + b;
    return res;
}

float resta(float a, float b)
{
    float res;
    res = a - b;
    return res;
}

float multiplicacion(float a, float b)
{
    float res;
    res = a * b;
    return res;
}

float division(float a, float b)
{
    float res;
    res = a / b;

    if (b == 0)
    {
        printf("No se puede dividir para 0\n");
        res = 0;
    }
    else
    {
        res = a / b;
    }
    return res;
}

float potenciacion(float a, float b)
{
    float res;

    if (a == 0 && b == 0)
    {
        printf("Indeterminacion.\n");
        res = 0;
    }
    else
    {
        res = pow(a, b);
    }
    return res;
}

float RaizCuadrada(float a)
{
    float res;
    res = sqrt(a);
    return res;
}

float validarFlotante()
{
    float n = 0;
    float aux = 0;

    do
    {
        getchar(); //Limpia el buffer de entrada para evitar problemas con scanf
        aux = scanf("%f", &n);
        if (aux =! 1)
        {
            prinft("El valor ingresado no es correcto.\n");
        }
    }while(aux =! 1);
    return n;
}

float validarEnteroconRango(int a, int b)
{
    int n = 0;
    float aux = 0;

    do
    {
        getchar(); //Limpia el buffer de entrada para evitar problemas con scanf
        aux = scanf("%d", &n);
        if (aux =! 1 || (n < a && n > b))
        {
            prinft("El valor ingresado no es correcto.\n");
        }
    }while(aux =! 1 || (n >= a && n <= b));
    return n;
}