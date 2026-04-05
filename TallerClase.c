#include <stdio.h>

int main()
{
    char ID[10];
    char nombre[30], continuar, aplicar, op;
    int stock, opc, cantidad, relleno;
    int registro = 0;
    float precio, totalprecio = 0, descuento, subtotal, montodescuento, total;
    do
    {
        printf("-------------------Menu---------------------\n");
        printf("Seleccione una opcion:\n");
        printf("1. Registar producto:\n");
        printf("2. Vender Producto:\n");
        printf("3. Reabastecer producto:\n");
        printf("4. Consultar informacion:\n");
        printf("5. Consultar ganancias:\n");
        printf("6. Salir programa\n");
        printf(">> ");
        scanf("%d", &opc);

        if (opc < 1 || opc > 6)
        {
            printf("No hay opcion valida. Ingrese nuevamente: ");
            scanf("%d", &opc);
        }

        switch (opc)
        {
        case 1:

            printf("Ingrese el ID del producto: ");
            scanf("%s", ID);

            printf("Ingrese el nombre del producto: ");
            getchar();
            fgets(nombre, sizeof(nombre), stdin);

            printf("Ingrese el stock del producto: ");
            scanf("%d", &stock);

            if (stock < 0)
            {
                printf("No hay stock negativo. Ingrese de nuevo: ");
                scanf("%d", &stock);
            }

            printf("Ingrese el precio del producto: ");
            scanf("%f", &precio);

            if (precio < 0)
            {
                printf("Precio negativo. Ingrese nuevamente: ");
                scanf("%f", &precio);
            }
            registro = 1;
            break;

        case 4:

            if (registro == 0)
            {
                printf("No se han registrado producto.\n");
                printf("Desea ingresar ahora? (s/n): ");
                scanf(" %c", &op);
                if (op == 's' || op == 'S')
                {
                    opc = 1;
                }
            }
            else
            {
                printf("Producto:\n");
                printf("\nID:\t\t%s", ID);
                printf("\nNombre:\t\t%s", nombre);
                printf("\nStock:\t\t%d", stock);
                printf("\nPrecio:\t\t%.2f\n", precio);
            }

            break;

        case 2:

            if (registro == 0)
            {
                printf("No se puede vender.\n");
                printf("Primero ingrese el producto\n");
                printf("Redirigiendo al Menu...\n");
                break;
            }

            do
            {
                do
                {
                    printf("Ingrese la cantidad que desea vender: ");
                    scanf("%d", &cantidad);

                    if (cantidad <= 0)
                    {
                        printf("Error: Cantidad debe ser mayor a 0.\n");
                    }
                    else if (cantidad > stock)
                    {
                        printf("No hay suficiente stock para vender esa cantidad de producto.\n");
                        printf("Stock disponible: %d\n", stock);
                    }
                } while (cantidad <= 0 || cantidad > stock);

                subtotal = cantidad * precio;

                printf("Desea agregar un descuento a la venta del producto? (s/n)\n");
                scanf(" %c", &aplicar);

                if (aplicar == 's' || aplicar == 'S')
                {
                    printf("Ingrese en porcentaje ej: 20\n");
                    printf(">> ");
                    scanf("%f", &descuento);
                    montodescuento = subtotal * (descuento / 100);
                    total = subtotal - montodescuento;

                    printf("Total con sin descuento: %.2f\n", subtotal);
                    printf("Total con descuento: %.2f\n", total);
                }
                else
                {
                    total = subtotal;
                    printf("Total con sin descuento: %.2f\n", subtotal);
                }

                totalprecio += total;

                stock -= cantidad;

                printf("Venta realizada.\n");
                printf("El stock restante es: %d\n", stock);

                printf("Desea realizar otra venta? (s/n): \n");
                scanf(" %c", &continuar);

            } while (continuar == 's' || continuar == 'S');

            break;
        case 3:

            if (registro == 0)
            {
                printf("No se puede reabaster sin ingresar primero el producto.\n");
                printf("Redirigiendo al Menu...\n");
                break;
            }

            printf("Ingrese la cantidad que desea reabastecer: ");
            scanf("%d", &relleno);

            if (relleno < 0)
            {
                printf("No se aceptan negativos. Ingrese de nuevo: ");
                scanf("%d", &relleno);
            }

            stock += relleno;

            printf("Su stock ha sido reabastecido.\n");
            printf("El stock ahora cuenta con: %d\n", stock);

            break;
        case 5:

            if (totalprecio == 0)
            {
                printf("Aun no se han realizado ventas.\n");
            }
            else
            {
                printf("Su total de ganancia es %.2f\n", totalprecio);
            }

            break;

        case 6:

            printf("Gracias por usar el programa.");
            break;

        default:
            printf("No existe la opcion.\n");
            printf(">> ");
            scanf("%d", &opc);
            break;
        }
    } while (opc < 6);
    return 0;
}