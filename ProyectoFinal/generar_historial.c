// ============================================================
// generar_historial.c
// Programa auxiliar que crea el archivo zonas.dat con
// 5 zonas urbanas y 30 dias de historial precargados
//
// Compilar: gcc generar_historial.c -o gen_historial -lm
// Ejecutar: ./gen_historial
// Resultado: crea zonas.dat listo para usar con el SIGPCAZU
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funciones.h"

// ------------------------------------------------------------
// Genera un valor flotante con variacion aleatoria alrededor
// de una base dada, con un margen de +/- variacion
// Se usa para crear datos realistas para cada dia del historial
// ------------------------------------------------------------
float valorAleatorio(float base, float variacion)
{
    float r = (float)(rand() % 1000) / 1000.0f; // Numero entre 0.0 y 1.0
    return base + (r * 2.0f - 1.0f) * variacion; // base +/- variacion
}

int main()
{
    srand(42); // Semilla fija para que el historial sea siempre el mismo

    Zona zonas[MAX_ZONAS];
    int  n = 5; // Cinco zonas urbanas de ejemplo

    // --------------------------------------------------------
    // Definicion de las 5 zonas urbanas con sus datos base
    // Los valores base son representativos de ciudades ecuatorianas
    // --------------------------------------------------------

    // Zona 1: Norte - zona residencial con trafico moderado
    zonas[0].id          = 1;
    strcpy(zonas[0].nombre, "Norte");
    zonas[0].temperatura  = 18.0f;
    zonas[0].viento       = 12.0f;
    zonas[0].humedad      = 72.0f;

    // Zona 2: Centro Historico - zona con trafico alto y actividad comercial
    zonas[1].id          = 2;
    strcpy(zonas[1].nombre, "Centro Historico");
    zonas[1].temperatura  = 19.5f;
    zonas[1].viento       = 8.0f;
    zonas[1].humedad      = 65.0f;

    // Zona 3: Sur - zona industrial y de alto trafico pesado
    zonas[2].id          = 3;
    strcpy(zonas[2].nombre, "Sur Industrial");
    zonas[2].temperatura  = 20.0f;
    zonas[2].viento       = 10.0f;
    zonas[2].humedad      = 60.0f;

    // Zona 4: Valle de los Chillos - zona suburbana con menor contaminacion
    zonas[3].id          = 4;
    strcpy(zonas[3].nombre, "Valle Los Chillos");
    zonas[3].temperatura  = 22.0f;
    zonas[3].viento       = 20.0f;
    zonas[3].humedad      = 55.0f;

    // Zona 5: Carapungo - zona norte con actividad mixta
    zonas[4].id          = 5;
    strcpy(zonas[4].nombre, "Carapungo");
    zonas[4].temperatura  = 17.5f;
    zonas[4].viento       = 15.0f;
    zonas[4].humedad      = 70.0f;

    // --------------------------------------------------------
    // Valores base de contaminacion por zona (ug/m3)
    // Basados en datos tipicos de monitoreo urbano ecuatoriano
    // Limites TULSMA: CO<=10000 | SO2<=350 | NO2<=150 | PM2.5<=65
    // --------------------------------------------------------
    //                    CO      SO2    NO2    PM2.5
    float bases[5][4] = {
        { 3500.0f, 45.0f,  60.0f, 20.0f },  // Norte (dentro del limite)
        { 7200.0f, 120.0f, 110.0f, 45.0f }, // Centro (alto pero dentro)
        { 9500.0f, 200.0f, 130.0f, 58.0f }, // Sur Industrial (cerca del limite)
        { 1800.0f, 20.0f,  30.0f,  8.0f },  // Valle (muy bajo)
        { 4500.0f, 60.0f,  75.0f, 28.0f }   // Carapungo (moderado)
    };

    // Variaciones diarias para simular fluctuaciones reales
    float vars[5][4] = {
        {  800.0f, 12.0f, 15.0f,  5.0f },
        { 1500.0f, 30.0f, 25.0f, 12.0f },
        { 2000.0f, 50.0f, 30.0f, 15.0f },
        {  400.0f,  5.0f,  8.0f,  2.0f },
        {  900.0f, 15.0f, 18.0f,  7.0f }
    };

    // --------------------------------------------------------
    // Generar 30 dias de historial para cada zona
    // --------------------------------------------------------
    for (int z = 0; z < n; z++)
    {
        zonas[z].nMediciones   = MAX_HISTORIAL; // 30 dias de historial
        zonas[z].hayMedicionHoy = 0;            // Hoy no se ha ingresado aun
        memset(&zonas[z].hoy, 0, sizeof(Medicion));

        for (int d = 0; d < MAX_HISTORIAL; d++)
        {
            zonas[z].historial[d].dia  = d + 1;

            // Generar valor con variacion aleatoria, asegurar que no sea negativo
            float co = valorAleatorio(bases[z][0], vars[z][0]);
            zonas[z].historial[d].co   = co   > 0 ? co   : bases[z][0] * 0.5f;

            float so2 = valorAleatorio(bases[z][1], vars[z][1]);
            zonas[z].historial[d].so2  = so2  > 0 ? so2  : bases[z][1] * 0.5f;

            float no2 = valorAleatorio(bases[z][2], vars[z][2]);
            zonas[z].historial[d].no2  = no2  > 0 ? no2  : bases[z][2] * 0.5f;

            float pm25 = valorAleatorio(bases[z][3], vars[z][3]);
            zonas[z].historial[d].pm25 = pm25 > 0 ? pm25 : bases[z][3] * 0.5f;
        }
    }

    // --------------------------------------------------------
    // Escribir el archivo binario zonas.dat
    // --------------------------------------------------------
    FILE *f = fopen(ARCHIVO_ZONAS, "wb");
    if (f == NULL)
    {
        printf("Error: no se pudo crear zonas.dat\n");
        return 1;
    }

    fwrite(&n, sizeof(int), 1, f);
    fwrite(zonas, sizeof(Zona), n, f);
    fclose(f);

    // --------------------------------------------------------
    // Confirmar lo que se genero
    // --------------------------------------------------------
    printf("  Archivo zonas.dat generado correctamente.\n");
    printf("  Zonas creadas: %d | Dias de historial por zona: %d\n\n", n, MAX_HISTORIAL);

    printf("  %-4s | %-20s | %-6s | %-6s | %-6s | %-6s\n",
           "ID", "Nombre", "CO", "SO2", "NO2", "PM2.5");
    printf("  -----|----------------------|--------|--------|--------|-------\n");

    for (int z = 0; z < n; z++)
    {
        // Mostrar el promedio del historial generado como verificacion
        float avgCO = 0, avgSO2 = 0, avgNO2 = 0, avgPM25 = 0;
        for (int d = 0; d < MAX_HISTORIAL; d++)
        {
            avgCO   += zonas[z].historial[d].co;
            avgSO2  += zonas[z].historial[d].so2;
            avgNO2  += zonas[z].historial[d].no2;
            avgPM25 += zonas[z].historial[d].pm25;
        }
        printf("  %-4d | %-20s | %6.1f | %6.1f | %6.1f | %6.1f\n",
               zonas[z].id, zonas[z].nombre,
               avgCO / MAX_HISTORIAL, avgSO2 / MAX_HISTORIAL,
               avgNO2 / MAX_HISTORIAL, avgPM25 / MAX_HISTORIAL);
    }

    printf("\n  Listo. Ejecute ./sigpcazu para iniciar el sistema.\n");
    return 0;
}
