// ============================================================
// main.c
// Programa principal - SIGPCAZU
//
// Flujo de uso diario:
//   1. Al iniciar: carga zonas con historial de 30 dias
//   2. Opcion 2:   usuario ingresa medicion del dia de HOY
//   3. Opcion 3:   bloqueado hasta que haya al menos una medicion de hoy
//   4. Opcion 3-3: predice MANANA (requiere >= 7 dias de historial)
//   5. Opcion 4:   exporta reporte
// ============================================================

#include <stdio.h>
#include "funciones.h"

int main(int argc, char const *argv[])
{
    Zona       zonas[MAX_ZONAS];
    Prediccion predicciones[MAX_ZONAS];

    int nZonas        = 0;
    int nPredicciones = 0;

    cargarZonas(zonas, &nZonas);

    printf("  =============================================\n");
    printf("   SIGPCAZU - Monitoreo de Calidad del Aire\n");
    printf("   Norma: TULSMA Libro VI Anexo 4 (Ecuador)\n");
    printf("  =============================================\n");
    printf("  Sistema iniciado: %d zona(s) cargada(s).\n\n", nZonas);

    if (nZonas > 0)
        printf("  PASO 1: Ingrese la medicion de HOY (opcion 2) antes de monitorear.\n");

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
                    // Error 4: pasar predicciones para limpiar huerfanas
                    eliminarZona(zonas, &nZonas, predicciones, &nPredicciones);
                    break;
                case 4:
                    printf("  Regresando al menu principal...\n");
                    break;
                }
            } while (opcZona != 4);
            break;
        }

        // ---- INGRESAR MEDICION DE HOY -----------------------
        case 2:
            ingresarMedicionHoy(zonas, nZonas);
            break;

        // ---- MODULO MONITOREO Y ANALISIS --------------------
        case 3:
            // Error 1: bloquear el menu completo si ninguna zona
            // tiene hayMedicionHoy = 1; mostrar mensaje claro
            if (!hayAlgunaMedicionHoy(zonas, nZonas))
            {
                printf("\n+--------------------------------------------------+\n");
                printf("| ACCESO BLOQUEADO                                 |\n");
                printf("+--------------------------------------------------+\n");
                printf("| Ninguna zona tiene medicion del dia ingresada.   |\n");
                printf("| Vaya a la opcion 2 del menu principal e ingrese  |\n");
                printf("| los datos de HOY antes de acceder al monitoreo.  |\n");
                printf("+--------------------------------------------------+\n");
                break;
            }
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
                        mostrarPredicciones(zonas, nZonas, predicciones, &nPredicciones);
                        break;
                    case 4:
                        mostrarPredicciones(zonas, nZonas, predicciones, &nPredicciones);
                        generarRecomendaciones(zonas, nZonas, predicciones, nPredicciones);
                        break;
                    case 5:
                        printf("  Regresando al menu principal...\n");
                        break;
                    }
                } while (opcMon != 5);
            }
            break;

        // ---- EXPORTAR REPORTE --------------------------------
        case 4:
            mostrarPredicciones(zonas, nZonas, predicciones, &nPredicciones);
            exportarReporte(zonas, nZonas, predicciones, nPredicciones);
            break;

        // ---- SALIR ------------------------------------------
        case 5:
            continuar = 0;
            break;
        }

    } while (continuar == 1);

    printf("\n  Saliendo del sistema...\n");
    printf("  Zonas registradas   : %d\n", nZonas);
    printf("  Predicciones activas: %d\n", nPredicciones);
    printf("  Todos los datos estan guardados. Hasta pronto.\n");

    return 0;
}
