// ============================================================
// main.c
// Programa principal - Sistema Integral de Gestion y
// Prediccion de Contaminacion del Aire (SIGPCAZU)
//
// Flujo de uso diario:
//   1. Al iniciar: carga zonas con historial de 30 dias
//   2. Opcion 2:   usuario ingresa medicion del dia de HOY
//   3. Opcion 3-3: el sistema predice la contaminacion de MANANA
//   4. Opcion 3-4: emite recomendaciones si se preveen excesos
//   5. Opcion 4:   exporta reporte al archivo reporte.txt
// ============================================================

#include <stdio.h>
#include "funciones.h"

int main(int argc, char const *argv[])
{
    // --------------------------------------------------------
    // Arreglos en memoria para zonas y predicciones
    // --------------------------------------------------------
    Zona       zonas[MAX_ZONAS];
    Prediccion predicciones[MAX_ZONAS];

    int nZonas        = 0; // Contador de zonas cargadas desde el archivo
    int nPredicciones = 0; // Contador de predicciones calculadas en la sesion

    // --------------------------------------------------------
    // Cargar datos desde zonas.dat
    // Cada zona trae su historial de 30 dias precargado
    // --------------------------------------------------------
    cargarZonas(zonas, &nZonas);

    printf("  =============================================\n");
    printf("   SIGPCAZU - Monitoreo de Calidad del Aire\n");
    printf("   Norma: TULSMA Libro VI Anexo 4 (Ecuador)\n");
    printf("  =============================================\n");
    printf("  Sistema iniciado: %d zona(s) cargada(s).\n\n", nZonas);

    if (nZonas > 0)
        printf("  PASO 1: Ingrese la medicion de HOY (opcion 2) antes de predecir.\n");

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
        // ---- MODULO GESTION DE ZONAS -------------------------
        case 1:
        {
            int opcZona = 0;
            do
            {
                opcZona = menuZonas();
                switch (opcZona)
                {
                case 1:
                    registrarZona(zonas, &nZonas);
                    break;
                case 2:
                    mostrarZonas(zonas, nZonas);
                    break;
                case 3:
                    eliminarZona(zonas, &nZonas);
                    break;
                case 4:
                    printf("  Regresando al menu principal...\n");
                    break;
                }
            } while (opcZona != 4);
            break;
        }

        // ---- INGRESAR MEDICION DEL DIA DE HOY ---------------
        case 2:
            // El usuario ingresa los valores medidos hoy
            // Estos datos son la base para predecir manana
            ingresarMedicionHoy(zonas, nZonas);
            break;

        // ---- MODULO MONITOREO Y ANALISIS --------------------
        case 3:
        {
            int opcMon = 0;
            do
            {
                opcMon = menuMonitoreo();
                switch (opcMon)
                {
                case 1:
                    monitoreoActual(zonas, nZonas);
                    break;
                case 2:
                    calcularPromedios(zonas, nZonas);
                    break;
                case 3:
                    // Predice manana usando historial + medicion de hoy
                    mostrarPredicciones(zonas, nZonas, predicciones, &nPredicciones);
                    break;
                case 4:
                    // Recalcula predicciones antes de recomendar para datos frescos
                    mostrarPredicciones(zonas, nZonas, predicciones, &nPredicciones);
                    generarRecomendaciones(zonas, nZonas, predicciones, nPredicciones);
                    break;
                case 5:
                    printf("  Regresando al menu principal...\n");
                    break;
                }
            } while (opcMon != 5);
            break;
        }

        // ---- EXPORTAR REPORTE --------------------------------
        case 4:
            // Recalcular predicciones para el reporte con datos actualizados
            mostrarPredicciones(zonas, nZonas, predicciones, &nPredicciones);
            exportarReporte(zonas, nZonas, predicciones, nPredicciones);
            break;

        // ---- SALIR ------------------------------------------
        case 5:
            continuar = 0;
            break;
        }

    } while (continuar == 1);

    // --------------------------------------------------------
    // Resumen al salir
    // --------------------------------------------------------
    printf("\n  Saliendo del sistema...\n");
    printf("  Zonas registradas   : %d\n", nZonas);
    printf("  Predicciones activas: %d\n", nPredicciones);
    printf("  Todos los datos estan guardados. Hasta pronto.\n");

    return 0;
}
