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
            printf("|  Fuente: ultima medicion del historial               |\n");
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

// ------------------------------------------------------------
// Genera recomendaciones de mitigacion por zona
// Considera 4 dimensiones:
//   AMBIENTAL  - reduccion de emisiones en la fuente
//   SOCIAL     - proteccion de grupos vulnerables y poblacion
//   CULTURAL   - habitos urbanos, transporte, consumo
//   GLOBAL     - cumplimiento normativo y coordinacion interinstitucional
// Las recomendaciones se activan segun el contaminante que excede
// y se refuerzan si hay alerta de manana
// ------------------------------------------------------------
void generarRecomendaciones(Zona *zonas, int n, Prediccion *preds, int nPreds)
{
    if (n == 0) { printf("  No hay zonas registradas.\n"); return; }

    printf("\n");
    printf("+==============================================================+\n");
    printf("|         RECOMENDACIONES DE MITIGACION                       |\n");
    printf("|  Dimensiones: Ambiental | Social | Cultural | Global        |\n");
    printf("+==============================================================+\n");

    for (int i = 0; i < n; i++)
    {
        if (!zonas[i].hayMedicionHoy) continue;

        Medicion *m   = &zonas[i].hoy;
        int       rec = 0; // Bandera: al menos una recomendacion emitida

        printf("\n+--------------------------------------------------------------+\n");
        printf("| ZONA [%d] %-50s |\n", zonas[i].id, zonas[i].nombre);
        printf("+--------------------------------------------------------------+\n");

        // ---- CO ALTO ---------------------------------------------------
        if (m->co > LIMITE_CO)
        {
            printf("| [CO ALTO]                                                    |\n");
            printf("+----+--------------------------------------------------------+\n");
            printf("| AM | Restringir circulacion de vehiculos en zonas de alto   |\n");
            printf("|    | trafico; prohibir quema de residuos solidos urbanos.   |\n");
            printf("| SO | Alertar a pacientes con enfermedades cardiovasculares  |\n");
            printf("|    | y EPOC; evitar exposicion prolongada al exterior.      |\n");
            printf("| CU | Promover teletrabajo y uso de transporte no motorizado |\n");
            printf("|    | (bicicleta, caminata) en dias de alta contaminacion.   |\n");
            printf("| GL | Notificar al MAATE segun Art. 6 TULSMA; coordinar     |\n");
            printf("|    | operativos con AMT y Municipio para control vehicular. |\n");
            printf("+----+--------------------------------------------------------+\n");
            rec = 1;
        }

        // ---- SO2 ALTO --------------------------------------------------
        if (m->so2 > LIMITE_SO2)
        {
            printf("| [SO2 ALTO]                                                   |\n");
            printf("+----+--------------------------------------------------------+\n");
            printf("| AM | Reducir actividad en plantas industriales y centrales  |\n");
            printf("|    | termicas; inspeccionar chimeneas y quemadores.         |\n");
            printf("| SO | Proteger a ninos, adultos mayores y personas con asma; |\n");
            printf("|    | mantener ventanas cerradas en zonas afectadas.         |\n");
            printf("| CU | Fomentar auditorias energeticas en industrias locales; |\n");
            printf("|    | sensibilizar a empresas sobre combustibles limpios.    |\n");
            printf("| GL | Revisar permisos de operacion industrial (MAE-RA);    |\n");
            printf("|    | aplicar planes de reduccion progresiva de SO2.        |\n");
            printf("+----+--------------------------------------------------------+\n");
            rec = 1;
        }

        // ---- NO2 ALTO --------------------------------------------------
        if (m->no2 > LIMITE_NO2)
        {
            printf("| [NO2 ALTO]                                                   |\n");
            printf("+----+--------------------------------------------------------+\n");
            printf("| AM | Restringir vehiculos diesel anteriores a 2010;         |\n");
            printf("|    | suspender obras con maquinaria pesada en hora pico.    |\n");
            printf("| SO | Informar a escuelas y guarderias para evitar           |\n");
            printf("|    | actividades al aire libre; distribuir mascarillas N95. |\n");
            printf("| CU | Impulsar revision tecnica vehicular (RTV) con enfoque  |\n");
            printf("|    | en emisiones; incentivar renovacion de flota antigua.  |\n");
            printf("| GL | Coordinar con INER y universidades la medicion de      |\n");
            printf("|    | NOx; alinear politicas con Agenda Ambiental Nacional.  |\n");
            printf("+----+--------------------------------------------------------+\n");
            rec = 1;
        }

        // ---- PM2.5 ALTO ------------------------------------------------
        if (m->pm25 > LIMITE_PM25)
        {
            printf("| [PM2.5 ALTO]                                                 |\n");
            printf("+----+--------------------------------------------------------+\n");
            printf("| AM | Humedecer vias no pavimentadas; incrementar frecuencia  |\n");
            printf("|    | de barrido y recoleccion de residuos en la zona.       |\n");
            printf("| SO | Suspender actividades deportivas al aire libre;        |\n");
            printf("|    | activar protocolo de atencion para grupos vulnerables. |\n");
            printf("| CU | Campanas de educacion ambiental en colegios y medios   |\n");
            printf("|    | de comunicacion; difundir indice de calidad del aire.  |\n");
            printf("| GL | Emitir boletin de alerta ciudadana (MSP / MAATE);      |\n");
            printf("|    | escalar a declaratoria de alerta amarilla si persiste. |\n");
            printf("+----+--------------------------------------------------------+\n");
            rec = 1;
        }

        // ---- ALERTA MANANA (prediccion supera limites) -----------------
        for (int j = 0; j < nPreds; j++)
        {
            if (preds[j].idZona == zonas[i].id && preds[j].alerta)
            {
                printf("| [ALERTA MANANA - MEDIDAS PREVENTIVAS]                        |\n");
                printf("+----+--------------------------------------------------------+\n");
                printf("| AM | Preparar equipos de monitoreo movil para verificacion  |\n");
                printf("|    | en campo; revisar fuentes emisoras identificadas.      |\n");
                printf("| SO | Emitir alerta preventiva a la ciudadania con 12h de   |\n");
                printf("|    | anticipacion; reforzar centros de salud cercanos.      |\n");
                printf("| CU | Activar comunicacion en redes sociales institucionales |\n");
                printf("|    | con recomendaciones de autocuidado para la poblacion.  |\n");
                printf("| GL | Notificar al COE Municipal; coordinar acciones con     |\n");
                printf("|    | Secretaria de Ambiente y organismos de respuesta.      |\n");
                printf("+----+--------------------------------------------------------+\n");
                rec = 1;
                break;
            }
        }

        if (!rec)
        {
            printf("| Estado: todos los contaminantes dentro del limite TULSMA.   |\n");
            printf("| Mantener monitoreo preventivo y registro diario.            |\n");
            printf("+--------------------------------------------------------------+\n");
        }
    }

    // Leyenda de dimensiones al final
    printf("\n");
    printf("+--------------------------------------------------------------+\n");
    printf("| LEYENDA DE DIMENSIONES                                       |\n");
    printf("+----+---------------------------------------------------------+\n");
    printf("| AM | AMBIENTAL  : reduccion de emisiones en la fuente        |\n");
    printf("| SO | SOCIAL     : proteccion de grupos vulnerables           |\n");
    printf("| CU | CULTURAL   : habitos urbanos, educacion, transporte     |\n");
    printf("| GL | GLOBAL     : normativa, institucional, interagencial    |\n");
    printf("+----+---------------------------------------------------------+\n");
}
