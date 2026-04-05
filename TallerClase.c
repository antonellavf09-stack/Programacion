// Taller de Programacion - Ejercicio 1
// Programa para gestionar el inventario y ventas de un producto en una tienda
#include <stdio.h>

int main()
{
    // Declaracion de variables
    char ID[10];
    char nombre[30], continuar, aplicar, op;
    int stock, opc, cantidad, relleno;
    int registro = 0;
    float precio, totalprecio = 0, descuento, subtotal, montodescuento, total;

    // Menu de opciones
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

        // Validacion de opciones

        if (opc < 1 || opc > 6)
        {
            printf("No hay opcion valida. Ingrese nuevamente: ");
            scanf("%d", &opc);
        }

        // Switch para cada opcion del menu
        switch (opc)
        {
            // Registro de producto
        case 1:

            printf("Ingrese el ID del producto: ");
            scanf("%s", ID);

            printf("Ingrese el nombre del producto: ");
            getchar();
            fgets(nombre, sizeof(nombre), stdin);

            printf("Ingrese el stock del producto: ");
            scanf("%d", &stock);

            // Validacion de stock negativo

            if (stock < 0)
            {
                printf("No hay stock negativo. Ingrese de nuevo: ");
                scanf("%d", &stock);
            }

            printf("Ingrese el precio del producto: ");
            scanf("%f", &precio);

            // Validacion de precio negativo

            if (precio < 0)
            {
                printf("Precio negativo. Ingrese nuevamente: ");
                scanf("%f", &precio);
            }
            registro = 1;
            break;

            // Consulta de informacion del producto
        case 4:

            // Validacion de registro de producto

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
            // Mostrar informacion del producto
            else
            {
                printf("Producto:\n");
                printf("\nID:\t\t%s", ID);
                printf("\nNombre:\t\t%s", nombre);
                printf("\nStock:\t\t%d", stock);
                printf("\nPrecio:\t\t%.2f\n", precio);
            }

            break;
            // Venta de producto
        case 2:
            // Validacion de registro de producto

            if (registro == 0)
            {
                printf("No se puede vender.\n");
                printf("Primero ingrese el producto\n");
                printf("Redirigiendo al Menu...\n");
                break;
            }
            // Proceso de venta
            do
            {
                // Validacion de cantidad a vender

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

                // Aplicar descuento

                printf("Desea agregar un descuento a la venta del producto? (s/n)\n");
                scanf(" %c", &aplicar);

                // Validacion de aplicacion de descuento

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
                    // Si no se aplica descuento, el total es igual al subtotal
                    total = subtotal;
                    printf("Total con sin descuento: %.2f\n", subtotal);
                }

                // Actualizar total de ganancias y stock

                totalprecio += total;

                stock -= cantidad;

                printf("Venta realizada.\n");
                printf("El stock restante es: %d\n", stock);

                // Validacion de continuar con otra venta

                printf("Desea realizar otra venta? (s/n): \n");
                scanf(" %c", &continuar);

            } while (continuar == 's' || continuar == 'S');

            break;
            // Reabastecer producto
        case 3:

            // Validacion de registro de producto

            if (registro == 0)
            {
                printf("No se puede reabaster sin ingresar primero el producto.\n");
                printf("Redirigiendo al Menu...\n");
                break;
            }

            // Proceso de reabastecimiento

            printf("Ingrese la cantidad que desea reabastecer: ");
            scanf("%d", &relleno);

            // Validacion de cantidad a reabastecer

            if (relleno < 0)
            {
                printf("No se aceptan negativos. Ingrese de nuevo: ");
                scanf("%d", &relleno);
            }

            // Actualizar stock con la cantidad reabastecida

            stock += relleno;

            printf("Su stock ha sido reabastecido.\n");
            printf("El stock ahora cuenta con: %d\n", stock);

            break;
            // Consulta de ganancias
        case 5:

            // Validacion de registro de producto
            if (totalprecio == 0)
            {
                // Si no se han realizado ventas, el total de ganancias es 0

                printf("Aun no se han realizado ventas.\n");
            }
            else
            {
                // Mostrar total de ganancias

                printf("Su total de ganancia es %.2f\n", totalprecio);
            }

            break;
            // Salir del programa
        case 6:

            // Mensaje de despedida

            printf("Gracias por usar el programa.");
            break;

        default:

            // Si se ingresa una opcion no valida, se muestra un mensaje de error y se solicita ingresar nuevamente

            printf("No existe la opcion.\n");
            printf(">> ");
            scanf("%d", &opc);
            break;
        }
    } while (opc < 6);
    return 0;
}