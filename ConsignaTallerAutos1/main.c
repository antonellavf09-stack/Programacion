// ============================================================
// main.c
// Programa principal - Sistema de Gestion de Inventario
// y Clientes (SGIC) - Concesionaria "Ruedas de Oro"
//
// Estructura del programa:
//   - Al iniciar: carga datos desde archivos binarios
//   - En ejecucion: navega por menus para operar el sistema
//   - Al modificar datos: guarda automaticamente en archivos
// ============================================================

#include <stdio.h>
#include "funciones.h"

int main(int argc, char const *argv[])
{
    // --------------------------------------------------------
    // Declaracion de arreglos en memoria para los tres modulos
    // --------------------------------------------------------
    Vehiculo vehiculos[MAX_VEHICULOS];
    Cliente  clientes[MAX_CLIENTES];
    Venta    ventas[MAX_VENTAS];

    int nVehiculos = 0; // Contador de vehiculos cargados
    int nClientes  = 0; // Contador de clientes cargados
    int nVentas    = 0; // Contador de ventas cargadas

    // --------------------------------------------------------
    // Cargar datos desde archivos al iniciar el programa
    // Si los archivos no existen se inicia con arreglos vacios
    // --------------------------------------------------------
    cargarVehiculos(vehiculos, &nVehiculos);
    cargarClientes(clientes, &nClientes);
    cargarVentas(ventas, &nVentas);

    printf("  Sistema iniciado: %d vehiculo(s), %d cliente(s), %d venta(s) cargados.\n",
           nVehiculos, nClientes, nVentas);

    // --------------------------------------------------------
    // Bucle principal del programa
    // --------------------------------------------------------
    int opcPrincipal = 0;
    int continuar    = 1;

    do
    {
        opcPrincipal = menuPrincipal();

        switch (opcPrincipal)
        {
        // ---- MODULO VEHICULOS --------------------------------
        case 1:
        {
            int opcVeh = 0;
            do
            {
                opcVeh = menuVehiculos();
                switch (opcVeh)
                {
                case 1:
                    registrarVehiculo(vehiculos, &nVehiculos);
                    break;
                case 2:
                    mostrarVehiculos(vehiculos, nVehiculos);
                    break;
                case 3:
                    buscarVehiculo(vehiculos, nVehiculos);
                    break;
                case 4:
                    buscarPorPreferencias(vehiculos, nVehiculos);
                    break;
                case 5:
                    actualizarVehiculo(vehiculos, nVehiculos);
                    break;
                case 6:
                    eliminarVehiculo(vehiculos, &nVehiculos);
                    break;
                case 7:
                    printf("  Regresando al menu principal...\n");
                    break;
                }
            } while (opcVeh != 7);
            break;
        }

        // ---- MODULO CLIENTES ---------------------------------
        case 2:
        {
            int opcCli = 0;
            do
            {
                opcCli = menuClientes();
                switch (opcCli)
                {
                case 1:
                    registrarCliente(clientes, &nClientes);
                    break;
                case 2:
                    mostrarClientes(clientes, nClientes);
                    break;
                case 3:
                    buscarCliente(clientes, nClientes);
                    break;
                case 4:
                    printf("  Regresando al menu principal...\n");
                    break;
                }
            } while (opcCli != 4);
            break;
        }

        // ---- MODULO VENTAS -----------------------------------
        case 3:
            registrarVenta(vehiculos, nVehiculos,
                           clientes,  nClientes,
                           ventas,    &nVentas);
            break;

        // ---- HISTORIAL DE VENTAS ----------------------------
        case 4:
            mostrarVentas(ventas, nVentas,
                          vehiculos, nVehiculos,
                          clientes,  nClientes);
            break;

        // ---- SALIR ------------------------------------------
        case 5:
            continuar = 0;
            break;
        }

    } while (continuar == 1);

    // --------------------------------------------------------
    // Resumen al salir del programa
    // --------------------------------------------------------
    printf("\n  Saliendo del sistema...\n");
    printf("  Resumen final:\n");
    printf("    Vehiculos en inventario : %d\n", nVehiculos);
    printf("    Clientes registrados    : %d\n", nClientes);
    printf("    Ventas realizadas       : %d\n", nVentas);
    printf("  Todos los datos estan guardados. Hasta pronto.\n");

    return 0;
}