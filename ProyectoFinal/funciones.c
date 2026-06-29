// ============================================================
// funciones.c
// Implementacion de todas las funciones del sistema SIGPCAZU
//
// Limites: TULSMA Libro VI Anexo 4 (AM 097-A, Ecuador)
//   CO    <= 10 000 ug/m3 (media 8h)
//   SO2   <=   350 ug/m3  (max 24h)
//   NO2   <=   150 ug/m3  (max 24h)
//   PM2.5 <=    65 ug/m3  (max 24h)
//
// Todas las salidas de datos se presentan en formato tabla
// para facilitar la lectura y comparacion de valores
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

// ============================================================
// SECCION 0: FUNCIONES AUXILIARES DE PRESENTACION EN TABLA
// ============================================================

// ------------------------------------------------------------
// Imprime una linea separadora horizontal de tabla
// Ejemplo: +--------+----------+----------+
// Los anchos[] es el arreglo con el ancho de cada columna
// ncols es la cantidad de columnas
// ------------------------------------------------------------
void imprimirLinea(int anchos[], int ncols)
{
    printf("+");
    for (int i = 0; i < ncols; i++)
    {
        for (int j = 0; j < anchos[i]; j++) printf("-");
        printf("+");
    }
    printf("\n");
}

// ------------------------------------------------------------
// Imprime una fila de encabezado centrado en cada celda
// titulos[] es el arreglo de strings con los titulos
// anchos[] es el arreglo con el ancho de cada columna
// ncols es la cantidad de columnas
// ------------------------------------------------------------
void imprimirEncabezado(char *titulos[], int anchos[], int ncols)
{
    imprimirLinea(anchos, ncols);
    printf("|");
    for (int i = 0; i < ncols; i++)
    {
        // Calcular espacios para centrar el texto dentro de la celda
        int espacio = anchos[i] - 2; // 2 = un espacio en cada lado
        printf(" %-*s|", espacio, titulos[i]);
    }
    printf("\n");
    imprimirLinea(anchos, ncols);
}

// ============================================================
// SECCION 1: FUNCIONES DE VALIDACION DE ENTRADA
// ============================================================

int validarIntRango(int a, int b)
{
    int n, aux;
    do
    {
        aux = scanf("%d", &n);
        while ((getchar()) != '\n');
        if (aux != 1 || n < a || n > b)
            printf("  Error: ingrese un valor entre %d y %d: ", a, b);
    } while (aux != 1 || n < a || n > b);
    return n;
}

float validarFloat(float min, float max)
{
    float f;
    int   aux;
    do
    {
        aux = scanf("%f", &f);
        while ((getchar()) != '\n');
        if (aux != 1 || f < min || f > max)
            printf("  Error: ingrese un valor entre %.2f y %.2f: ", min, max);
    } while (aux != 1 || f < min || f > max);
    return f;
}

void leerCadena(char *cadena, int n)
{
    int len;
    fgets(cadena, n, stdin);
    len = strlen(cadena) - 1;
    if (cadena[len] == '\n')
        cadena[len] = '\0';
}

void convertirMayuscula(char *c)
{
    if (*c >= 'a' && *c <= 'z')
        *c = *c - 32;
}

// ============================================================
// SECCION 2: FUNCIONES DE PERSISTENCIA EN ARCHIVOS
// ============================================================

void cargarZonas(Zona *zonas, int *n)
{
    FILE *f = fopen(ARCHIVO_ZONAS, "rb");
    if (f == NULL) { *n = 0; return; }
    fread(n, sizeof(int), 1, f);
    fread(zonas, sizeof(Zona), *n, f);
    fclose(f);

    // --- Error 3: verificar IDs duplicados despues de la carga ---
    // Recorre todos los pares posibles; si encuentra ID igual elimina
    // la segunda ocurrencia desplazando el arreglo y decrementando n
    int eliminados = 0;
    for (int i = 0; i < *n; i++)
    {
        for (int j = i + 1; j < *n; )
        {
            if (zonas[i].id == zonas[j].id)
            {
                printf("  [AVISO] ID duplicado al cargar: ID=%d ('%s') duplica a '%s'. Se elimina.\n",
                       zonas[j].id, zonas[j].nombre, zonas[i].nombre);
                // Desplazar el resto del arreglo una posicion hacia atras
                for (int k = j; k < *n - 1; k++)
                    zonas[k] = zonas[k + 1];
                (*n)--;
                eliminados++;
                // No incrementar j: el nuevo elemento en pos j puede ser duplicado tambien
            }
            else
            {
                j++;
            }
        }
    }
    if (eliminados > 0)
    {
        printf("  [AVISO] %d zona(s) duplicada(s) eliminadas. Archivo corregido.\n", eliminados);
        guardarZonas(zonas, *n);
    }
}

// ------------------------------------------------------------
// Retorna 1 si al menos una zona tiene hayMedicionHoy = 1
// Se usa en main.c para bloquear el menu de monitoreo (Error 1)
// ------------------------------------------------------------
int hayAlgunaMedicionHoy(Zona *zonas, int n)
{
    for (int i = 0; i < n; i++)
        if (zonas[i].hayMedicionHoy)
            return 1;
    return 0;
}

void guardarZonas(Zona *zonas, int n)
{
    FILE *f = fopen(ARCHIVO_ZONAS, "wb");
    if (f == NULL) { printf("  Error: no se pudo guardar el archivo.\n"); return; }
    fwrite(&n, sizeof(int), 1, f);
    fwrite(zonas, sizeof(Zona), n, f);
    fclose(f);
    printf("  Datos guardados correctamente.\n");
}

// ------------------------------------------------------------
// Exporta reporte de texto con estado actual y predicciones
// ------------------------------------------------------------
void exportarReporte(Zona *zonas, int n, Prediccion *preds, int nPreds)
{
    FILE *f = fopen(ARCHIVO_REPORTE, "w");
    if (f == NULL) { printf("  Error: no se pudo crear el reporte.\n"); return; }

    fprintf(f, "=======================================================\n");
    fprintf(f, "  REPORTE SIGPCAZU\n");
    fprintf(f, "  Norma: TULSMA Libro VI Anexo 4 (AM 097-A, Ecuador)\n");
    fprintf(f, "=======================================================\n\n");

    fprintf(f, "+------+--------------------+----------+----------+----------+----------+--------+\n");
    fprintf(f, "| ID   | Zona               |  CO hoy  | SO2 hoy  | NO2 hoy  |PM2.5 hoy | Alerta |\n");
    fprintf(f, "+------+--------------------+----------+----------+----------+----------+--------+\n");
    for (int i = 0; i < n; i++)
    {
        if (zonas[i].hayMedicionHoy)
            fprintf(f, "| %-4d | %-18s | %8.2f | %8.2f | %8.2f | %8.2f |  %s  |\n",
                zonas[i].id, zonas[i].nombre,
                zonas[i].hoy.co, zonas[i].hoy.so2,
                zonas[i].hoy.no2, zonas[i].hoy.pm25,
                zonas[i].hoy.co > LIMITE_CO || zonas[i].hoy.so2 > LIMITE_SO2 ||
                zonas[i].hoy.no2 > LIMITE_NO2 || zonas[i].hoy.pm25 > LIMITE_PM25 ? "SI" : "No");
        else
            fprintf(f, "| %-4d | %-18s | %8s | %8s | %8s | %8s |  %s  |\n",
                zonas[i].id, zonas[i].nombre,
                "---", "---", "---", "---", "--");
    }
    fprintf(f, "+------+--------------------+----------+----------+----------+----------+--------+\n");

    fprintf(f, "\n--- PREDICCIONES PARA MANANA ---\n");
    fprintf(f, "+------+----------+----------+----------+----------+-------------------+\n");
    fprintf(f, "| ID   | CO pred  | SO2 pred | NO2 pred |PM2.5 pred| Estado TULSMA     |\n");
    fprintf(f, "+------+----------+----------+----------+----------+-------------------+\n");
    for (int i = 0; i < nPreds; i++)
        fprintf(f, "| %-4d | %8.2f | %8.2f | %8.2f | %8.2f | %-17s |\n",
            preds[i].idZona,
            preds[i].co_pred, preds[i].so2_pred,
            preds[i].no2_pred, preds[i].pm25_pred,
            preds[i].alerta ? "*** EXCEDE TULSMA ***" : "Dentro del limite");
    fprintf(f, "+------+----------+----------+----------+----------+-------------------+\n");

    fprintf(f, "\n=======================================================\n");
    fprintf(f, "  FIN DEL REPORTE\n");
    fprintf(f, "=======================================================\n");
    fclose(f);
    printf("  Reporte exportado a '%s'.\n", ARCHIVO_REPORTE);
}

// ============================================================
// SECCION 3: FUNCIONES DE MENU
// ============================================================

int menuPrincipal()
{
    printf("\n");
    printf("+--------------------------------------------------+\n");
    printf("|         SIGPCAZU - Calidad del Aire              |\n");
    printf("|   Norma: TULSMA Libro VI Anexo 4 (AM 097-A)     |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 1. Gestion de Zonas                              |\n");
    printf("| 2. Ingresar medicion del dia de HOY              |\n");
    printf("| 3. Monitoreo y Analisis                          |\n");
    printf("| 4. Exportar Reporte                              |\n");
    printf("| 5. Salir                                         |\n");
    printf("+--------------------------------------------------+\n");
    printf("  Opcion: ");
    return validarIntRango(1, 5);
}

int menuZonas()
{
    printf("\n");
    printf("+----------------------------------+\n");
    printf("|       Gestion de Zonas           |\n");
    printf("+----------------------------------+\n");
    printf("| 1. Registrar nueva zona          |\n");
    printf("| 2. Mostrar todas las zonas       |\n");
    printf("| 3. Eliminar zona                 |\n");
    printf("| 4. Volver al menu principal      |\n");
    printf("+----------------------------------+\n");
    printf("  Opcion: ");
    return validarIntRango(1, 4);
}

int menuMonitoreo()
{
    printf("\n");
    printf("+------------------------------------------+\n");
    printf("|          Monitoreo y Analisis            |\n");
    printf("+------------------------------------------+\n");
    printf("| 1. Ver estado de HOY vs limites TULSMA   |\n");
    printf("| 2. Calcular promedios historicos (30d)   |\n");
    printf("| 3. Predecir contaminacion de MANANA      |\n");
    printf("| 4. Recomendaciones de mitigacion         |\n");
    printf("| 5. Volver al menu principal              |\n");
    printf("+------------------------------------------+\n");
    printf("  Opcion: ");
    return validarIntRango(1, 5);
}

// ============================================================
// SECCION 4: FUNCIONES DE GESTION DE ZONAS
// ============================================================

void registrarZona(Zona *zonas, int *n)
{
    if (*n >= MAX_ZONAS)
    {
        printf("  Error: se alcanzo el maximo de %d zonas.\n", MAX_ZONAS);
        return;
    }

    printf("\n+----------------------------------+\n");
    printf("|        Registrar Zona            |\n");
    printf("+----------------------------------+\n");
    printf("  ID de la zona: ");
    zonas[*n].id = validarIntRango(1, 9999);

    for (int i = 0; i < *n; i++)
    {
        if (zonas[i].id == zonas[*n].id)
        {
            printf("  Error: ya existe una zona con ese ID.\n");
            return;
        }
    }

    // Error 5: validar nombre no vacio; pedir reingreso hasta recibir al menos 1 caracter
    do
    {
        printf("  Nombre de la zona: ");
        leerCadena(zonas[*n].nombre, 50);
        if (zonas[*n].nombre[0] == '\0')
            printf("  Error: el nombre no puede estar vacio. Ingrese al menos un caracter.\n");
    } while (zonas[*n].nombre[0] == '\0');
    printf("  Temperatura actual (C, -10 a 50): ");
    zonas[*n].temperatura = validarFloat(-10.0f, 50.0f);
    printf("  Velocidad del viento (km/h, 0 a 150): ");
    zonas[*n].viento = validarFloat(0.0f, 150.0f);
    printf("  Humedad relativa (%%, 0 a 100): ");
    zonas[*n].humedad = validarFloat(0.0f, 100.0f);

    zonas[*n].nMediciones    = 0;
    zonas[*n].hayMedicionHoy = 0;
    memset(zonas[*n].historial, 0, sizeof(zonas[*n].historial));
    memset(&zonas[*n].hoy, 0, sizeof(Medicion));

    (*n)++;
    guardarZonas(zonas, *n);
    printf("  Zona registrada exitosamente.\n");
}

// ------------------------------------------------------------
// Muestra tabla con resumen de todas las zonas registradas
// Columnas: ID | Nombre | Temp | Viento | Humedad | Dias | Hoy
// ------------------------------------------------------------
void mostrarZonas(Zona *zonas, int n)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return; }

    printf("\n");
    printf("+------+--------------------+-------+--------+---------+------+-----+\n");
    printf("| ID   | Nombre             | Temp  | Viento | Humedad | Dias | Hoy |\n");
    printf("|      |                    |  (C)  | (km/h) |   (%%)   | hist |     |\n");
    printf("+------+--------------------+-------+--------+---------+------+-----+\n");
    for (int i = 0; i < n; i++)
    {
        printf("| %-4d | %-18s | %5.1f | %6.1f | %7.1f | %-4d | %-3s |\n",
               zonas[i].id, zonas[i].nombre,
               zonas[i].temperatura, zonas[i].viento, zonas[i].humedad,
               zonas[i].nMediciones,
               zonas[i].hayMedicionHoy ? "SI" : "No");
    }
    printf("+------+--------------------+-------+--------+---------+------+-----+\n");
}

int buscarZona(Zona *zonas, int n)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return -1; }
    printf("  Ingrese el ID de la zona: ");
    int id = validarIntRango(1, 9999);
    for (int i = 0; i < n; i++)
        if (zonas[i].id == id)
        {
            printf("  Zona encontrada: [%d] %s\n", zonas[i].id, zonas[i].nombre);
            return i;
        }
    printf("  Error: no se encontro la zona con ID %d.\n", id);
    return -1;
}

void eliminarZona(Zona *zonas, int *n, Prediccion *preds, int *nPreds)
{
    int idx = buscarZona(zonas, *n);
    if (idx == -1) return;

    int idEliminado = zonas[idx].id; // Guardar el ID antes de borrar la zona

    // Eliminar la zona desplazando el arreglo
    for (int i = idx; i < *n - 1; i++)
        zonas[i] = zonas[i + 1];
    (*n)--;
    guardarZonas(zonas, *n);
    printf("  Zona [%d] eliminada del arreglo.\n", idEliminado);

    // --- Error 4: eliminar predicciones huerfanas cuyo idZona coincide ---
    int predEliminadas = 0;
    for (int i = 0; i < *nPreds; )
    {
        if (preds[i].idZona == idEliminado)
        {
            // Desplazar el arreglo de predicciones
            for (int k = i; k < *nPreds - 1; k++)
                preds[k] = preds[k + 1];
            (*nPreds)--;
            predEliminadas++;
            // No incrementar i: la nueva posicion i puede ser otro duplicado
        }
        else
        {
            i++;
        }
    }
    if (predEliminadas > 0)
        printf("  %d prediccion(es) huerfana(s) eliminadas del arreglo.\n", predEliminadas);

    printf("  Zona eliminada exitosamente.\n");
}

// ============================================================
// SECCION 5: MONITOREO, PREDICCION Y RECOMENDACIONES
// ============================================================

// ------------------------------------------------------------
// Ingresa la medicion del dia actual (HOY) para una zona
// Solicita tambien actualizar condiciones climaticas
// ------------------------------------------------------------
void ingresarMedicionHoy(Zona *zonas, int n)
{
    int idx = buscarZona(zonas, n);
    if (idx == -1) return;

    Zona *z = &zonas[idx];

    printf("\n+------------------------------------------------------+\n");
    printf("| Medicion de HOY  -  Zona [%d] %-20s |\n", z->id, z->nombre);
    printf("+------------------------------------------------------+\n");
    printf("| Limites TULSMA:  CO<=10000 | SO2<=350               |\n");
    printf("|                  NO2<=150  | PM2.5<=65  (ug/m3)     |\n");
    printf("+------------------------------------------------------+\n\n");

    printf("  Condiciones climaticas actuales:\n");
    printf("  Temperatura (C, -10 a 50)    : ");
    z->temperatura = validarFloat(-10.0f, 50.0f);
    printf("  Viento (km/h, 0 a 150)       : ");
    z->viento = validarFloat(0.0f, 150.0f);
    printf("  Humedad (%%, 0 a 100)         : ");
    z->humedad = validarFloat(0.0f, 100.0f);

    printf("\n  Niveles de contaminantes medidos hoy:\n");
    printf("  CO    (ug/m3, 0-40000)       : ");
    z->hoy.co   = validarFloat(0.0f, 40000.0f);
    printf("  SO2   (ug/m3, 0-500)         : ");
    z->hoy.so2  = validarFloat(0.0f, 500.0f);
    printf("  NO2   (ug/m3, 0-400)         : ");
    z->hoy.no2  = validarFloat(0.0f, 400.0f);
    printf("  PM2.5 (ug/m3, 0-300)         : ");
    z->hoy.pm25 = validarFloat(0.0f, 300.0f);

    z->hoy.dia       = z->nMediciones + 1;
    z->hayMedicionHoy = 1;
    guardarZonas(zonas, n);

    // Mostrar resumen de lo ingresado en tabla
    printf("\n  Resumen ingresado:\n");
    printf("+----------+----------+----------+----------+----------+\n");
    printf("| Temp (C) | Viento   | Humedad  | CO       | SO2      |\n");
    printf("+----------+----------+----------+----------+----------+\n");
    printf("| %8.1f | %8.1f | %8.1f | %8.1f | %8.1f |\n",
           z->temperatura, z->viento, z->humedad, z->hoy.co, z->hoy.so2);
    printf("+----------+----------+----------+----------+----------+\n");
    printf("+----------+----------+\n");
    printf("| NO2      | PM2.5    |\n");
    printf("+----------+----------+\n");
    printf("| %8.1f | %8.1f |\n", z->hoy.no2, z->hoy.pm25);
    printf("+----------+----------+\n");
    printf("  Medicion registrada. Ya puede calcular la prediccion de manana.\n");
}

// ------------------------------------------------------------
// Compara niveles de HOY con limites TULSMA en formato tabla
// Columnas: Contaminante | Valor HOY | Limite TULSMA | Estado
// ------------------------------------------------------------
void monitoreoActual(Zona *zonas, int n)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return; }

    printf("\n");
    printf("+============================================================+\n");
    printf("|      MONITOREO ACTUAL vs LIMITES TULSMA (AM 097-A)        |\n");
    printf("+============================================================+\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n+------------------------------------------------------+\n");
        printf("| Zona [%d] %-41s |\n", zonas[i].id, zonas[i].nombre);
        printf("+------------------------------------------------------+\n");

        Medicion *m;
        if (zonas[i].hayMedicionHoy)
        {
            m = &zonas[i].hoy;
            printf("|  Fuente: medicion ingresada hoy                      |\n");
        }
        else if (zonas[i].nMediciones > 0)
        {
            m = &zonas[i].historial[zonas[i].nMediciones - 1];
            printf("|  AVISO: NO se ingreso medicion de hoy (opcion 2).    |\n");
            printf("|  Los valores mostrados corresponden al dia %2d del   |\n", m->dia);
            printf("|  historial. Los promedios de los 30 dias en esta     |\n");
            printf("|  zona fueron calculados SIN la lectura del dia actual.|\n");
        }
        else
        {
            printf("|  Sin datos disponibles.                              |\n");
            printf("+------------------------------------------------------+\n");
            continue;
        }

        printf("+--------------+-----------+-----------+------------------+\n");
        printf("| Contaminante | Valor HOY | Lim TULSMA| Estado           |\n");
        printf("|              | (ug/m3)   | (ug/m3)   |                  |\n");
        printf("+--------------+-----------+-----------+------------------+\n");
        printf("| CO           | %9.2f | %9.0f | %-16s |\n",
               m->co,   LIMITE_CO,   m->co   > LIMITE_CO   ? "!!! EXCEDE TULSMA" : "OK - Dentro");
        printf("| SO2          | %9.2f | %9.0f | %-16s |\n",
               m->so2,  LIMITE_SO2,  m->so2  > LIMITE_SO2  ? "!!! EXCEDE TULSMA" : "OK - Dentro");
        printf("| NO2          | %9.2f | %9.0f | %-16s |\n",
               m->no2,  LIMITE_NO2,  m->no2  > LIMITE_NO2  ? "!!! EXCEDE TULSMA" : "OK - Dentro");
        printf("| PM2.5        | %9.2f | %9.0f | %-16s |\n",
               m->pm25, LIMITE_PM25, m->pm25 > LIMITE_PM25 ? "!!! EXCEDE TULSMA" : "OK - Dentro");
        printf("+--------------+-----------+-----------+------------------+\n");
    }
}

// ------------------------------------------------------------
// Calcula y muestra promedios historicos de los 30 dias
// Tabla: Contaminante | Promedio | Limite | Estado | % del limite
// ------------------------------------------------------------
void calcularPromedios(Zona *zonas, int n)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return; }

    printf("\n");
    printf("+============================================================+\n");
    printf("|        PROMEDIOS HISTORICOS - ULTIMOS 30 DIAS             |\n");
    printf("+============================================================+\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n+------------------------------------------------------+\n");
        printf("| Zona [%d] %-41s |\n", zonas[i].id, zonas[i].nombre);
        printf("+------------------------------------------------------+\n");

        if (zonas[i].nMediciones == 0)
        {
            printf("|  Sin datos historicos.                               |\n");
            printf("+------------------------------------------------------+\n");
            continue;
        }

        float sumCO = 0, sumSO2 = 0, sumNO2 = 0, sumPM25 = 0;
        int   dias  = zonas[i].nMediciones;

        for (int j = 0; j < dias; j++)
        {
            sumCO   += zonas[i].historial[j].co;
            sumSO2  += zonas[i].historial[j].so2;
            sumNO2  += zonas[i].historial[j].no2;
            sumPM25 += zonas[i].historial[j].pm25;
        }

        float avgCO   = sumCO   / dias;
        float avgSO2  = sumSO2  / dias;
        float avgNO2  = sumNO2  / dias;
        float avgPM25 = sumPM25 / dias;

        // Aviso claro si el usuario no ha ingresado la medicion del dia actual
        if (!zonas[i].hayMedicionHoy)
        {
            printf("+------------------------------------------------------+\n");
            printf("|  AVISO: los promedios de los %2d dias en esta zona    |\n", dias);
            printf("|  fueron calculados SIN la lectura de datos del dia   |\n");
            printf("|  actual. Para incluir hoy use la opcion 2 del menu.  |\n");
            printf("+------------------------------------------------------+\n");
        }
        printf("| Dias en historial: %-4d                                |\n", dias);
        printf("+--------------+-----------+-----------+--------+--------+\n");
        printf("| Contaminante | Promedio  | Lim TULSMA| Estado | %% Lim  |\n");
        printf("|              | (ug/m3)   | (ug/m3)   |        |        |\n");
        printf("+--------------+-----------+-----------+--------+--------+\n");
        printf("| CO           | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               avgCO,   LIMITE_CO,   avgCO   > LIMITE_CO   ? "EXCEDE" : "OK",
               (avgCO   / LIMITE_CO)   * 100.0f);
        printf("| SO2          | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               avgSO2,  LIMITE_SO2,  avgSO2  > LIMITE_SO2  ? "EXCEDE" : "OK",
               (avgSO2  / LIMITE_SO2)  * 100.0f);
        printf("| NO2          | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               avgNO2,  LIMITE_NO2,  avgNO2  > LIMITE_NO2  ? "EXCEDE" : "OK",
               (avgNO2  / LIMITE_NO2)  * 100.0f);
        printf("| PM2.5        | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               avgPM25, LIMITE_PM25, avgPM25 > LIMITE_PM25 ? "EXCEDE" : "OK",
               (avgPM25 / LIMITE_PM25) * 100.0f);
        printf("+--------------+-----------+-----------+--------+--------+\n");
    }
}

// ------------------------------------------------------------
// Calcula prediccion de MANANA con promedio ponderado
// Historial 30 dias + medicion de HOY (doble peso)
// Ajustes climaticos: viento, humedad, temperatura
// ------------------------------------------------------------
Prediccion predecirManana(Zona *zona)
{
    Prediccion pred;
    pred.idZona    = zona->id;
    pred.co_pred   = 0;
    pred.so2_pred  = 0;
    pred.no2_pred  = 0;
    pred.pm25_pred = 0;
    pred.alerta    = 0;

    if (!zona->hayMedicionHoy) return pred;

    int   dias     = zona->nMediciones;

    // Error 2: requerir minimo MIN_DIAS_PREDICCION dias de historial
    if (dias < MIN_DIAS_PREDICCION)
    {
        printf("  [ADVERTENCIA] Zona [%d] %s: solo tiene %d dia(s) de historial.\n",
               zona->id, zona->nombre, dias);
        printf("  Se necesitan minimo %d dias para calcular la prediccion.\n", MIN_DIAS_PREDICCION);
        pred.idZona = -1; // Centinela: indica que la prediccion no fue calculada
        return pred;
    }
    float sumCO    = 0, sumSO2 = 0, sumNO2 = 0, sumPM25 = 0;
    float sumPesos = 0;

    // --- Promedio ponderado del historial ---
    // Los dias mas recientes (indice mayor) tienen mayor peso
    for (int i = 0; i < dias; i++)
    {
        float peso = (float)(i + 1);
        sumCO   += zona->historial[i].co   * peso;
        sumSO2  += zona->historial[i].so2  * peso;
        sumNO2  += zona->historial[i].no2  * peso;
        sumPM25 += zona->historial[i].pm25 * peso;
        sumPesos += peso;
    }

    // --- Medicion de HOY con doble peso por ser la mas reciente ---
    float pesoHoy = (float)(dias + 1) * 2.0f;
    sumCO    += zona->hoy.co   * pesoHoy;
    sumSO2   += zona->hoy.so2  * pesoHoy;
    sumNO2   += zona->hoy.no2  * pesoHoy;
    sumPM25  += zona->hoy.pm25 * pesoHoy;
    sumPesos += pesoHoy;

    pred.co_pred   = sumCO   / sumPesos;
    pred.so2_pred  = sumSO2  / sumPesos;
    pred.no2_pred  = sumNO2  / sumPesos;
    pred.pm25_pred = sumPM25 / sumPesos;

    // --- Ajustes por condiciones climaticas ---
    if (zona->viento > 30.0f)
    {
        // Viento fuerte dispersa contaminantes: reducir 10%
        pred.co_pred   *= 0.90f;
        pred.so2_pred  *= 0.90f;
        pred.no2_pred  *= 0.90f;
        pred.pm25_pred *= 0.90f;
    }
    if (zona->humedad > 70.0f)
        pred.pm25_pred *= 1.15f; // Humedad alta retiene PM2.5

    if (zona->temperatura > 30.0f)
        pred.no2_pred  *= 1.10f; // Calor activa reacciones fotoquimicas

    // --- Verificar excesos vs limites TULSMA ---
    if (pred.co_pred   > LIMITE_CO   ||
        pred.so2_pred  > LIMITE_SO2  ||
        pred.no2_pred  > LIMITE_NO2  ||
        pred.pm25_pred > LIMITE_PM25)
        pred.alerta = 1;

    return pred;
}

// ------------------------------------------------------------
// Muestra las predicciones para MANANA en formato tabla
// Columnas: Contaminante | Predicho | Limite | Estado | % Lim
// Solo procesa zonas con medicion de hoy ingresada
// ------------------------------------------------------------
void mostrarPredicciones(Zona *zonas, int n, Prediccion *preds, int *nPreds)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return; }

    *nPreds = 0;
    printf("\n");
    printf("+============================================================+\n");
    printf("|          PREDICCION DE CONTAMINACION PARA MANANA          |\n");
    printf("|       Modelo: Promedio ponderado + ajustes climaticos      |\n");
    printf("+============================================================+\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n+------------------------------------------------------+\n");
        printf("| Zona [%d] %-41s |\n", zonas[i].id, zonas[i].nombre);
        printf("+------------------------------------------------------+\n");

        if (!zonas[i].hayMedicionHoy)
        {
            printf("|  *** Ingrese la medicion de HOY para predecir ***    |\n");
            printf("+------------------------------------------------------+\n");
            continue;
        }
        if (zonas[i].nMediciones == 0)
        {
            printf("|  Sin historial de 30 dias disponible.                |\n");
            printf("+------------------------------------------------------+\n");
            continue;
        }

        preds[*nPreds] = predecirManana(&zonas[i]);

        // Si predecirManana devolvio centinela -1 (menos de MIN_DIAS_PREDICCION dias)
        // la advertencia ya se imprimio dentro de predecirManana; saltar esta zona
        if (preds[*nPreds].idZona == -1)
        {
            printf("+------------------------------------------------------+\n");
            continue;
        }

        // Tabla de factores climaticos usados en el calculo
        printf("| Clima usado: Temp=%.1fC  Viento=%.1fkm/h  Hum=%.1f%%\n",
               zonas[i].temperatura, zonas[i].viento, zonas[i].humedad);
        printf("+--------------+-----------+-----------+--------+--------+\n");
        printf("| Contaminante | Predicho  | Lim TULSMA| Estado | %% Lim  |\n");
        printf("|              | (ug/m3)   | (ug/m3)   |        |        |\n");
        printf("+--------------+-----------+-----------+--------+--------+\n");
        printf("| CO           | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               preds[*nPreds].co_pred, LIMITE_CO,
               preds[*nPreds].co_pred > LIMITE_CO ? "ALERTA" : "OK",
               (preds[*nPreds].co_pred / LIMITE_CO) * 100.0f);
        printf("| SO2          | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               preds[*nPreds].so2_pred, LIMITE_SO2,
               preds[*nPreds].so2_pred > LIMITE_SO2 ? "ALERTA" : "OK",
               (preds[*nPreds].so2_pred / LIMITE_SO2) * 100.0f);
        printf("| NO2          | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               preds[*nPreds].no2_pred, LIMITE_NO2,
               preds[*nPreds].no2_pred > LIMITE_NO2 ? "ALERTA" : "OK",
               (preds[*nPreds].no2_pred / LIMITE_NO2) * 100.0f);
        printf("| PM2.5        | %9.2f | %9.0f | %-6s | %5.1f%% |\n",
               preds[*nPreds].pm25_pred, LIMITE_PM25,
               preds[*nPreds].pm25_pred > LIMITE_PM25 ? "ALERTA" : "OK",
               (preds[*nPreds].pm25_pred / LIMITE_PM25) * 100.0f);
        printf("+--------------+-----------+-----------+--------+--------+\n");

        if (preds[*nPreds].alerta)
            printf("| *** ALERTA TULSMA: se prevee exceder limites manana *** |\n");
        else
            printf("| Estado general: dentro de los limites TULSMA.          |\n");
        printf("+------------------------------------------------------+\n");

        (*nPreds)++;
    }
}

// ============================================================
// SECCION 6: RECOMENDACIONES DE MITIGACION
// ============================================================

// ------------------------------------------------------------
// Imprime el encabezado de un bloque de contaminante con
// el nombre de la zona, el contaminante, valor medido,
// limite TULSMA y porcentaje del limite
// ------------------------------------------------------------
static void recCabecera(const char *zona, const char *cont,
                        float valor, float limite)
{
    float pct = (valor / limite) * 100.0f;
    printf("+----------------------------------------------------------------+\n");
    printf("| Zona: %-15s  Contaminante: %-20s  |\n", zona, cont);
    printf("| Medido hoy: %8.2f ug/m3  Limite TULSMA: %6.0f  (%.1f%% del lim) |\n",
           valor, limite, pct);
    printf("+----+------------------------------------------------------------+\n");
    printf("| DIM| Recomendacion especifica                                   |\n");
    printf("+----+------------------------------------------------------------+\n");
}

// ------------------------------------------------------------
// Genera recomendaciones completamente dinamicas:
// - El texto de cada recomendacion incluye el NOMBRE de la zona,
//   el VALOR exacto medido, los datos climaticos y el nivel
//   de riesgo calculado como porcentaje del limite TULSMA.
// - Los cuatro aspectos (AM/SO/CU/GL) siempre estan presentes.
// - Tres niveles de intensidad segun pct del limite:
//     <= 60%  : preventivo (no se imprime bloque de alerta)
//     61-100% : alerta moderada
//     > 100%  : critico / accion inmediata
// ------------------------------------------------------------
void generarRecomendaciones(Zona *zonas, int n, Prediccion *preds, int nPreds)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return; }

    printf("\n");
    printf("+================================================================+\n");
    printf("|        RECOMENDACIONES DE MITIGACION  SIGPCAZU                |\n");
    printf("|  Basadas en valores medidos HOY - TULSMA Libro VI Anexo 4     |\n");
    printf("|  AM=Ambiental  SO=Social  CU=Cultural  GL=Global              |\n");
    printf("+================================================================+\n");

    for (int i = 0; i < n; i++)
    {
        if (!zonas[i].hayMedicionHoy)
        {
            printf("\n+----------------------------------------------------------------+\n");
            printf("| Zona [%d] %-20s : sin medicion de hoy.          |\n",
                   zonas[i].id, zonas[i].nombre);
            printf("| Ingrese los datos del dia actual mediante la opcion 2.         |\n");
            printf("+----------------------------------------------------------------+\n");
            continue;
        }

        Medicion *m   = &zonas[i].hoy;
        char     *z   = zonas[i].nombre;   // Alias corto para usar en printf
        float     tmp = zonas[i].temperatura;
        float     vie = zonas[i].viento;
        float     hum = zonas[i].humedad;
        int       rec = 0;

        // Calcular porcentajes del limite para determinar el nivel de riesgo
        float pctCO   = (m->co   / LIMITE_CO)   * 100.0f;
        float pctSO2  = (m->so2  / LIMITE_SO2)  * 100.0f;
        float pctNO2  = (m->no2  / LIMITE_NO2)  * 100.0f;
        float pctPM25 = (m->pm25 / LIMITE_PM25) * 100.0f;

        // Encabezado de zona con resumen de datos del dia
        printf("\n+================================================================+\n");
        printf("| ZONA [%d] %-56s |\n", zonas[i].id, z);
        printf("| Datos climaticos de hoy:  Temp=%.1fC  Viento=%.1fkm/h  Hum=%.1f%% |\n",
               tmp, vie, hum);
        printf("| CO=%.1f(%.0f%%)  SO2=%.1f(%.0f%%)  NO2=%.1f(%.0f%%)  PM2.5=%.1f(%.0f%%)    |\n",
               m->co,   pctCO,
               m->so2,  pctSO2,
               m->no2,  pctNO2,
               m->pm25, pctPM25);
        printf("+================================================================+\n");

        // ==============================================================
        // BLOQUE CO
        // ==============================================================
        if (pctCO > 60.0f)
        {
            recCabecera(z, "CO - Monoxido de Carbono", m->co, LIMITE_CO);
            rec = 1;

            if (pctCO <= 100.0f)
            {
                // Nivel moderado: 61-100% del limite
                printf("| AM | En %s el CO llego a %.0f ug/m3 (%.0f%% del limite). Se   |\n", z, m->co, pctCO);
                printf("|    | debe reducir la velocidad en vias de alto trafico para    |\n");
                printf("|    | bajar las emisiones de escape vehicular en la zona.       |\n");
                printf("| AM | Revisar que los vehiculos de transporte publico que       |\n");
                printf("|    | circulan en %s cuenten con revision tecnica vigente.  |\n", z);
                printf("| SO | Alertar a residentes de %s con problemas cardiacos     |\n", z);
                printf("|    | y EPOC que eviten actividad fisica al exterior hoy,       |\n");
                printf("|    | ya que %.0f ug/m3 de CO puede causar cefalea y mareo.    |\n", m->co);
                printf("| SO | Notificar a los centros de salud cercanos a %s sobre  |\n", z);
                printf("|    | el nivel de CO registrado para preparar atencion a        |\n");
                printf("|    | posibles casos de intoxicacion leve durante el dia.      |\n");
                printf("| CU | Difundir en grupos comunitarios de %s la recomendacion|\n", z);
                printf("|    | de usar transporte publico o bicicleta hoy; el CO alto   |\n");
                printf("|    | indica exceso de vehiculos particulares en la zona.      |\n");
                printf("| CU | Publicar en redes del municipio el valor de %.0f ug/m3   |\n", m->co);
                printf("|    | para que la ciudadania de %s tome decisiones de         |\n", z);
                printf("|    | movilidad consciente durante el resto del dia.           |\n");
                printf("| GL | Activar el protocolo de monitoreo continuo de CO segun   |\n");
                printf("|    | TULSMA Anexo 4; registrar el evento en el informe diario |\n");
                printf("|    | de la red de calidad del aire de la zona %s.           |\n", z);
                printf("| GL | Compartir el dato de %.0f ug/m3 con la red del MAATE    |\n", m->co);
                printf("|    | para analisis interinstitucional; si la tendencia sube   |\n");
                printf("|    | escalar a alerta segun el Acuerdo Ministerial 097-A.    |\n");
            }
            else
            {
                // Nivel critico: supera el limite TULSMA
                printf("| AM | URGENTE en %s: CO=%-.0f supera el limite (10000 ug/m3). |\n", z, m->co);
                printf("|    | Restringir circulacion vehicular de inmediato; implementar|\n");
                printf("|    | pico y placa extendido y cerrar fuentes de combustion.   |\n");
                printf("| AM | Suspender quema de residuos, incineracion y uso de       |\n");
                printf("|    | generadores a combustion en toda el area de %s hasta  |\n", z);
                printf("|    | que el CO baje por debajo de 10000 ug/m3.               |\n");
                printf("| SO | Activar alerta sanitaria en %s: notificar al MSP y     |\n", z);
                printf("|    | centros de salud para atencion de intoxicacion por CO;   |\n");
                printf("|    | el nivel de %.0f ug/m3 representa riesgo vital.         |\n", m->co);
                printf("| SO | Evaluar evacuacion temporal de poblacion vulnerable de   |\n");
                printf("|    | %s: ninos menores de 5, adultos mayores y embarazadas. |\n", z);
                printf("|    | Distribuir mascarillas de carbon activado en la zona.    |\n");
                printf("| CU | Suspender clases presenciales y eventos masivos en       |\n");
                printf("|    | %s; la concentracion de %.0f ug/m3 es peligrosa para  |\n", z, m->co);
                printf("|    | la salud de la poblacion en espacios al aire libre.      |\n");
                printf("| CU | Activar campana de comunicacion de crisis en %s via   |\n", z);
                printf("|    | radio, TV y redes; instrucciones claras de autocuidado:  |\n");
                printf("|    | cerrar ventanas, no hacer ejercicio, usar mascarilla.    |\n");
                printf("| GL | Notificar al COE Municipal de %s el valor de %.0f      |\n", z, m->co);
                printf("|    | ug/m3 de CO; solicitar declaratoria de emergencia         |\n");
                printf("|    | ambiental si el nivel se mantiene por mas de 2 horas.   |\n");
                printf("| GL | Reportar al MAATE en maximo 24h segun AM 097-A;          |\n");
                printf("|    | solicitar auditoria urgente de fuentes emisoras en        |\n");
                printf("|    | la zona %s que expliquen el nivel critico registrado.  |\n", z);
            }
            printf("+----+------------------------------------------------------------+\n");
        }

        // ==============================================================
        // BLOQUE SO2
        // ==============================================================
        if (pctSO2 > 60.0f)
        {
            recCabecera(z, "SO2 - Dioxido de Azufre", m->so2, LIMITE_SO2);
            rec = 1;

            if (pctSO2 <= 100.0f)
            {
                printf("| AM | En %s el SO2 alcanzo %.1f ug/m3 (%.0f%% del limite de   |\n", z, m->so2, pctSO2);
                printf("|    | 350 ug/m3). Solicitar a industrias de la zona reportes    |\n");
                printf("|    | de operacion de sus sistemas de control de emisiones.     |\n");
                printf("| AM | Con %.1f ug/m3 de SO2 y viento de %.0f km/h en %s,    |\n", m->so2, vie, z);
                printf("|    | revisar el estado de filtros en plantas que usen          |\n");
                printf("|    | combustibles con alto contenido de azufre en la zona.    |\n");
                printf("| SO | Alertar a personas con asma en %s: %.1f ug/m3 de SO2  |\n", z, m->so2);
                printf("|    | puede desencadenar crisis respiratoria; evitar salir      |\n");
                printf("|    | entre las 10h00 y 16h00 cuando la concentracion es mayor.|\n");
                printf("| SO | Informar a colegios y guarderias de %s que suspendan    |\n", z);
                printf("|    | actividades al aire libre; la humedad de %.0f%% combinada |\n", hum);
                printf("|    | con SO2 aumenta el riesgo de irritacion ocular y garganta.|\n");
                printf("| CU | Organizar taller de concientizacion en %s con las       |\n", z);
                printf("|    | empresas industriales: el registro de %.1f ug/m3 de SO2 |\n", m->so2);
                printf("|    | indica necesidad de revisar combustibles usados.         |\n");
                printf("| CU | Fomentar en la comunidad de %s la denuncia ciudadana    |\n", z);
                printf("|    | de olores y humos industriales a traves del sistema de   |\n");
                printf("|    | quejas ambientales del GAD Municipal.                    |\n");
                printf("| GL | Verificar permisos ambientales de industrias en %s;    |\n", z);
                printf("|    | el nivel de %.1f ug/m3 activa la obligacion de           |\n", m->so2);
                printf("|    | monitoreo reforzado segun TULSMA Libro VI Anexo 4.       |\n");
                printf("| GL | Alinear el caso de %s con el Plan Climatico Nacional   |\n", z);
                printf("|    | y los compromisos de reduccion de SO2 del Ecuador en el  |\n");
                printf("|    | marco del Acuerdo de Paris (NDC Ecuador 2020-2025).      |\n");
            }
            else
            {
                printf("| AM | URGENTE en %s: SO2=%.1f supera 350 ug/m3 (TULSMA).    |\n", z, m->so2);
                printf("|    | Ordenar cierre temporal de la fuente industrial          |\n");
                printf("|    | identificada como principal emisora en la zona.          |\n");
                printf("| AM | Activar inspeccion tecnica urgente de la Entidad         |\n");
                printf("|    | Ambiental de Control en %s; exigir uso de diesel con   |\n", z);
                printf("|    | maximo 50 ppm de azufre en toda la zona afectada.        |\n");
                printf("| SO | Coordinar con el MSP puestos de atencion rapida en       |\n");
                printf("|    | %s para casos de irritacion ocular y respiratoria;     |\n", z);
                printf("|    | el nivel de %.1f ug/m3 requiere atencion medica urgente.|\n", m->so2);
                printf("| SO | Distribuir informacion puerta a puerta en barrios de     |\n");
                printf("|    | %s sobre sintomas de SO2: tos, ardor nasal y garganta; |\n", z);
                printf("|    | indicar donde acudir y como protegerse de inmediato.    |\n");
                printf("| CU | Suspender todo evento masivo al aire libre en %s;      |\n", z);
                printf("|    | generar contenido educativo sobre el origen del SO2 y   |\n");
                printf("|    | su relacion con la lluvia acida para difusion escolar.  |\n");
                printf("| CU | Documentar y publicar el episodio de %.1f ug/m3 en     |\n", m->so2);
                printf("|    | %s como caso de estudio de impacto industrial en la   |\n", z);
                printf("|    | calidad del aire para fortalecer la cultura ambiental.  |\n");
                printf("| GL | Notificar al MAATE y a la Contraloria General sobre el   |\n");
                printf("|    | incumplimiento en %s si la fuente tenia permiso vigente;|\n", z);
                printf("|    | documentar el pasivo ambiental conforme al COOTAD.      |\n");
                printf("| GL | Registrar el evento de %.1f ug/m3 de SO2 en %s en    |\n", m->so2, z);
                printf("|    | el sistema nacional de inventario de emisiones del       |\n");
                printf("|    | MAATE para ajuste del plan de monitoreo en la zona.     |\n");
            }
            printf("+----+------------------------------------------------------------+\n");
        }

        // ==============================================================
        // BLOQUE NO2
        // ==============================================================
        if (pctNO2 > 60.0f)
        {
            recCabecera(z, "NO2 - Dioxido de Nitrogeno", m->no2, LIMITE_NO2);
            rec = 1;

            if (pctNO2 <= 100.0f)
            {
                printf("| AM | En %s el NO2 llego a %.1f ug/m3 (%.0f%% del limite de  |\n", z, m->no2, pctNO2);
                printf("|    | 150 ug/m3). Solicitar a la AMT intensificar operativos   |\n");
                printf("|    | de revision tecnica vehicular (RTV) en la zona.          |\n");
                printf("| AM | Con temperatura de %.0fC en %s las reacciones          |\n", tmp, z);
                printf("|    | fotoquimicas son mas activas; revisar fuentes de NOx como|\n");
                printf("|    | vehiculos diesel y maquinaria pesada en hora pico.       |\n");
                printf("| SO | Advertir a embarazadas y ninos de %s sobre los riesgos  |\n", z);
                printf("|    | del NO2 (%.1f ug/m3): puede afectar el desarrollo        |\n", m->no2);
                printf("|    | pulmonar; recomendar reduccion de exposicion al exterior.|\n");
                printf("| SO | Coordinar con centros de salud de %s el monitoreo de   |\n", z);
                printf("|    | bronquitis y enfermedades respiratorias en menores;      |\n");
                printf("|    | el nivel de %.1f ug/m3 requiere seguimiento clinico.    |\n", m->no2);
                printf("| CU | Promover en %s el carpooling y el uso de ciclovias     |\n", z);
                printf("|    | como alternativa al vehiculo individual; la lectura de   |\n");
                printf("|    | %.1f ug/m3 indica exceso de trafico motorizado en la zona.|\n", m->no2);
                printf("| CU | Incentivar entre los ciudadanos de %s el mantenimiento  |\n", z);
                printf("|    | preventivo de vehiculos (afinacion, cambio de catalizador)|\n");
                printf("|    | para reducir las emisiones de NOx desde la fuente movil. |\n");
                printf("| GL | Coordinar con el INER programas de renovacion de flota   |\n");
                printf("|    | antigua en %s; el nivel de %.1f ug/m3 indica vehiculos |\n", z, m->no2);
                printf("|    | sin control de emision que deben ser retirados de vias.  |\n");
                printf("| GL | Incorporar el dato de %.1f ug/m3 de NO2 en %s en el   |\n", m->no2, z);
                printf("|    | inventario nacional de emisiones GEI del MAATE;          |\n");
                printf("|    | alinear con la Agenda Ambiental Nacional vigente.        |\n");
            }
            else
            {
                printf("| AM | URGENTE en %s: NO2=%.1f supera 150 ug/m3 (TULSMA).    |\n", z, m->no2);
                printf("|    | Restringir circulacion de vehiculos diesel de alto       |\n");
                printf("|    | tonelaje y maquinaria pesada en toda la zona de inmediato.|\n");
                printf("| AM | Detener obras de construccion con maquinaria a diesel en |\n");
                printf("|    | %s; con %.0fC de temperatura el NO2 forma smog          |\n", z, tmp);
                printf("|    | fotoquimico que agrava la exposicion de la poblacion.    |\n");
                printf("| SO | Emitir alerta a escuelas y hospitales de %s para cerrar |\n", z);
                printf("|    | ventanas; el nivel de %.1f ug/m3 de NO2 puede causar    |\n", m->no2);
                printf("|    | inflamacion pulmonar aguda en exposicion prolongada.     |\n");
                printf("| SO | Informar urgente a la poblacion de %s sobre sintomas:  |\n", z);
                printf("|    | tos, dificultad respiratoria y ardor ocular; indicar     |\n");
                printf("|    | los centros de atencion medica habilitados en la zona.  |\n");
                printf("| CU | Activar campana informativa en %s: explicar que el NO2  |\n", z);
                printf("|    | de %.1f ug/m3 con %.0fC y %.0f km/h de viento genera  |\n", m->no2, tmp, vie);
                printf("|    | smog; reducir el auto particular es la medida principal. |\n");
                printf("| CU | Documentar el episodio de %.1f ug/m3 en %s como caso  |\n", m->no2, z);
                printf("|    | de estudio para talleres de educacion ambiental en       |\n");
                printf("|    | instituciones educativas y juntas parroquiales.          |\n");
                printf("| GL | Aplicar principio de precaucion del TULSMA: emitir       |\n");
                printf("|    | resolucion temporal de restriccion vehicular en %s;    |\n", z);
                printf("|    | notificar a SENPLADES para ajuste del plan de movilidad. |\n");
                printf("| GL | Reportar el incidente de %.1f ug/m3 en %s al MAATE;   |\n", m->no2, z);
                printf("|    | solicitar auditoria de flota vehicular y fuentes fijas   |\n");
                printf("|    | conforme al Acuerdo Ministerial 097-A vigente.           |\n");
            }
            printf("+----+------------------------------------------------------------+\n");
        }

        // ==============================================================
        // BLOQUE PM2.5
        // ==============================================================
        if (pctPM25 > 60.0f)
        {
            recCabecera(z, "PM2.5 - Particulado Fino", m->pm25, LIMITE_PM25);
            rec = 1;

            if (pctPM25 <= 100.0f)
            {
                printf("| AM | En %s el PM2.5 alcanzo %.1f ug/m3 (%.0f%% del limite de |\n", z, m->pm25, pctPM25);
                printf("|    | 65 ug/m3). Humectar las vias no pavimentadas de la zona   |\n");
                printf("|    | para reducir la generacion de polvo en suspension.        |\n");
                printf("| AM | Revisar obras de construccion activas en %s; con        |\n", z);
                printf("|    | humedad de %.0f%% y %.0f km/h de viento el particulado   |\n", hum, vie);
                printf("|    | se dispersa; exigir mallas y humectacion de escombros.   |\n");
                printf("| SO | Recomendar a personas con EPOC y cardiovasculares de      |\n");
                printf("|    | %s que limiten exposicion al exterior hoy;              |\n", z);
                printf("|    | %.1f ug/m3 de PM2.5 penetra en alveolos pulmonares.     |\n", m->pm25);
                printf("| SO | Publicar en las juntas parroquiales de %s el valor de  |\n", z);
                printf("|    | %.1f ug/m3 con recomendaciones de proteccion personal;  |\n", m->pm25);
                printf("|    | especialmente para adultos mayores y ninos escolares.   |\n");
                printf("| CU | Educar a la comunidad de %s: PM2.5=%.1f ug/m3 no se   |\n", z, m->pm25);
                printf("|    | percibe visualmente pero dania pulmones a largo plazo;  |\n");
                printf("|    | fomentar el monitoreo diario del indice de calidad.     |\n");
                printf("| CU | Impulsar jornadas de arborizado urbano en %s; los      |\n", z);
                printf("|    | arboles capturan PM2.5 y reducen el efecto isla de calor|\n");
                printf("|    | que con %.0fC favorece la acumulacion de particulado.   |\n", tmp);
                printf("| GL | Coordinar con el GAD Municipal de %s barrido mecanizado |\n", z);
                printf("|    | y limpieza de calzadas; vincular el dato de %.1f ug/m3  |\n", m->pm25);
                printf("|    | con la red de monitoreo del MAATE para validar tendencia.|\n");
                printf("| GL | Activar protocolo INCA (Indice Nacional de Calidad del   |\n");
                printf("|    | Aire) para %s con valor de %.1f ug/m3; si la tendencia  |\n", z, m->pm25);
                printf("|    | sube, escalar a alerta segun el protocolo del MAATE.    |\n");
            }
            else
            {
                printf("| AM | URGENTE en %s: PM2.5=%.1f supera 65 ug/m3 (TULSMA).   |\n", z, m->pm25);
                printf("|    | Suspender toda obra de construccion y demolicion en la   |\n");
                printf("|    | zona; iniciar humectacion de vias cada 2 horas.          |\n");
                printf("| AM | Prohibir quema a cielo abierto en %s y sus alrededores; |\n", z);
                printf("|    | con humedad de %.0f%% y %.0f km/h de viento el PM2.5   |\n", hum, vie);
                printf("|    | de %.1f ug/m3 se acumula sin dispersarse eficientemente. |\n", m->pm25);
                printf("| SO | Activar protocolo de atencion prioritaria en %s para   |\n", z);
                printf("|    | grupos vulnerables: ninos menores de 12, adultos mayores |\n");
                printf("|    | de 65 y personas con enfermedades respiratorias.         |\n");
                printf("| SO | Distribuir mascarillas N95 en centros de salud y         |\n");
                printf("|    | escuelas de %s; el PM2.5 de %.1f ug/m3 representa      |\n", z, m->pm25);
                printf("|    | riesgo de dano pulmonar irreversible en exposicion larga.|\n");
                printf("| CU | Suspender clases presenciales en %s; transmitir mensajes |\n", z);
                printf("|    | preventivos en radio y redes sobre PM2.5=%.1f ug/m3:   |\n", m->pm25);
                printf("|    | que hacer, que no hacer y donde buscar atencion medica. |\n");
                printf("| CU | Activar la cultura de monitoreo ciudadano en %s:        |\n", z);
                printf("|    | publicar el indice en tiempo real en pantallas digitales |\n");
                printf("|    | y apps municipales; la comunidad debe conocer el riesgo. |\n");
                printf("| GL | Emitir declaratoria de Alerta Amarilla Ambiental para    |\n");
                printf("|    | %s con PM2.5=%.1f ug/m3 segun protocolo COE Municipal;  |\n", z, m->pm25);
                printf("|    | notificar al MSP y activar el plan de contingencia.      |\n");
                printf("| GL | Documentar el episodio en el Registro Ambiental Nacional;|\n");
                printf("|    | solicitar auditoria de fuentes emisoras en %s que       |\n", z);
                printf("|    | expliquen el nivel critico de PM2.5 registrado hoy.     |\n");
            }
            printf("+----+------------------------------------------------------------+\n");
        }

        // ==============================================================
        // TODOS LOS CONTAMINANTES DENTRO DEL LIMITE: recomendaciones
        // preventivas tambien personalizadas con datos de la zona
        // ==============================================================
        if (!rec)
        {
            printf("+----------------------------------------------------------------+\n");
            printf("| TODOS LOS CONTAMINANTES DENTRO DEL LIMITE TULSMA EN %s\n", z);
            printf("+----+------------------------------------------------------------+\n");
            printf("| AM | Los niveles en %s (CO=%.0f SO2=%.0f NO2=%.0f PM2.5=%.0f) |\n",
                   z, m->co, m->so2, m->no2, m->pm25);
            printf("|    | estan dentro del limite. Mantener barrido de vias y        |\n");
            printf("|    | verificar que no haya fuentes nuevas de emision activas.   |\n");
            printf("| AM | Con viento de %.0f km/h en %s los contaminantes se        |\n", vie, z);
            printf("|    | dispersan bien; continuar monitoreo diario para detectar   |\n");
            printf("|    | tendencias de subida antes de que superen el limite.       |\n");
            printf("| SO | Aprovechar que %s tiene niveles aceptables para reforzar  |\n", z);
            printf("|    | educacion en salud ambiental en escuelas y centros         |\n");
            printf("|    | comunitarios: los buenos datos de hoy son un ejemplo.      |\n");
            printf("| SO | Informar a la ciudadania de %s sobre los valores de hoy   |\n", z);
            printf("|    | (CO=%.0f SO2=%.0f NO2=%.0f PM2.5=%.0f ug/m3) como ejemplo|\n",
                   m->co, m->so2, m->no2, m->pm25);
            printf("|    | de buenas practicas de gestion del aire urbano.            |\n");
            printf("| CU | Promover en %s habitos de movilidad sostenible mientras   |\n", z);
            printf("|    | los niveles son bajos: bicicleta, caminata, transporte     |\n");
            printf("|    | publico; los datos buenos refuerzan el cambio cultural.    |\n");
            printf("| CU | Organizar mingas de limpieza y arborizado urbano en %s;   |\n", z);
            printf("|    | con temperatura de %.0fC y humedad de %.0f%% es un dia     |\n", tmp, hum);
            printf("|    | ideal para actividades comunitarias al aire libre.         |\n");
            printf("| GL | Registrar el cumplimiento de %s en el sistema TULSMA;     |\n", z);
            printf("|    | los datos de CO=%.0f SO2=%.0f NO2=%.0f PM2.5=%.0f ug/m3  |\n",
                   m->co, m->so2, m->no2, m->pm25);
            printf("|    | demuestran efectividad de las medidas de gestion vigentes. |\n");
            printf("| GL | Coordinar con el MAATE la actualizacion del inventario de  |\n");
            printf("|    | fuentes emisoras de %s; los buenos resultados de hoy      |\n", z);
            printf("|    | deben sustentarse con politicas de monitoreo continuo.     |\n");
            printf("+----+------------------------------------------------------------+\n");
        }

        // ==============================================================
        // ALERTA PREVENTIVA PARA MANANA (si la prediccion supera limite)
        // Tambien personalizada con los valores predichos reales
        // ==============================================================
        for (int j = 0; j < nPreds; j++)
        {
            if (preds[j].idZona == zonas[i].id && preds[j].alerta)
            {
                printf("\n+----------------------------------------------------------------+\n");
                printf("| ALERTA PREVENTIVA PARA MANANA EN %s\n", z);
                printf("| Prediccion: CO=%.0f | SO2=%.0f | NO2=%.0f | PM2.5=%.0f ug/m3   |\n",
                       preds[j].co_pred, preds[j].so2_pred,
                       preds[j].no2_pred, preds[j].pm25_pred);
                printf("+----+------------------------------------------------------------+\n");
                printf("| AM | Iniciar HOY en %s la reduccion de actividades emisoras   |\n", z);
                printf("|    | para evitar que manana se superen los limites; preparar   |\n");
                printf("|    | equipos de monitoreo movil para verificacion en campo.    |\n");
                printf("| SO | Emitir boletin preventivo en %s con 12h de anticipacion; |\n", z);
                printf("|    | reforzar centros de salud con personal para atencion de   |\n");
                printf("|    | casos respiratorios esperados segun la prediccion.        |\n");
                printf("| CU | Activar comunicacion en redes y radio de %s con         |\n", z);
                printf("|    | recomendaciones especificas: reducir salidas, usar        |\n");
                printf("|    | mascarilla, evitar ejercicio al aire libre manana.        |\n");
                printf("| GL | Notificar al COE Municipal de %s los valores predichos;  |\n", z);
                printf("|    | activar el plan de contingencia ambiental segun el        |\n");
                printf("|    | protocolo del MAATE para episodios de alta contaminacion. |\n");
                printf("+----+------------------------------------------------------------+\n");
                break;
            }
        }
    }

    // Leyenda al final
    printf("\n+----------------------------------------------------------------+\n");
    printf("| LEYENDA DE DIMENSIONES                                         |\n");
    printf("+----+------------------------------------------------------------+\n");
    printf("| AM | AMBIENTAL : reduccion directa de la fuente emisora         |\n");
    printf("| SO | SOCIAL    : proteccion de la poblacion y grupos vulnerables |\n");
    printf("| CU | CULTURAL  : educacion, habitos urbanos, participacion       |\n");
    printf("| GL | GLOBAL    : normativa, instituciones, acuerdos nacionales   |\n");
    printf("+----+------------------------------------------------------------+\n");
}
