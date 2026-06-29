// ============================================================
// funciones.h
// Archivo de cabecera: definicion de estructuras y prototipos
// Sistema Integral de Gestion y Prediccion de
// Contaminacion del Aire en Zonas Urbanas (SIGPCAZU)
//
// Limites basados en: TULSMA Libro VI Anexo 4
// Acuerdo Ministerial 097-A - Ministerio del Ambiente Ecuador
// ============================================================

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>

// ------------------------------------------------------------
// LIMITES MAXIMOS PERMISIBLES - TULSMA LIBRO VI ANEXO 4
// Acuerdo Ministerial 097-A, Ecuador
// Condiciones de referencia: 25 degC y 760 mmHg
// ------------------------------------------------------------
#define LIMITE_CO    10000.0f  // CO  en ug/m3 (media 8 horas)
#define LIMITE_SO2     350.0f  // SO2 en ug/m3 (concentracion maxima 24h)
#define LIMITE_NO2     150.0f  // NO2 en ug/m3 (concentracion maxima 24h)
#define LIMITE_PM25     65.0f  // PM2.5 en ug/m3 (concentracion maxima 24h)

// ------------------------------------------------------------
// CONFIGURACION DEL SISTEMA
// ------------------------------------------------------------
#define MAX_ZONAS       10     // Numero maximo de zonas permitidas
#define MAX_HISTORIAL   30     // Dias de historial por zona (ventana deslizante)

// Archivos de persistencia binaria
#define ARCHIVO_ZONAS   "zonas.dat"
#define ARCHIVO_REPORTE "reporte.txt"

// ------------------------------------------------------------
// ESTRUCTURAS DE DATOS
// ------------------------------------------------------------

// Medicion diaria de contaminantes en una zona
typedef struct
{
    int   dia;    // Numero de dia del historial (1..30)
    float co;     // Monoxido de carbono en ug/m3
    float so2;    // Dioxido de azufre en ug/m3
    float no2;    // Dioxido de nitrogeno en ug/m3
    float pm25;   // Material particulado fino en ug/m3
} Medicion;

// Zona urbana monitoreada con su historial de 30 dias
typedef struct
{
    int      id;                        // Identificador unico de la zona
    char     nombre[50];                // Nombre de la zona
    float    temperatura;               // Temperatura actual en Celsius
    float    viento;                    // Velocidad del viento en km/h
    float    humedad;                   // Humedad relativa en %
    Medicion historial[MAX_HISTORIAL];  // Ultimos 30 dias de mediciones
    int      nMediciones;               // Cantidad de dias cargados en historial
    // Medicion del dia actual (ingresada por el usuario en esta sesion)
    Medicion hoy;                       // Datos del dia en curso
    int      hayMedicionHoy;            // 1 = el usuario ya ingreso datos de hoy
} Zona;

// Resultado de la prediccion para las proximas 24 horas
typedef struct
{
    int   idZona;      // ID de la zona pronosticada
    float co_pred;     // CO predicho en ug/m3
    float so2_pred;    // SO2 predicho en ug/m3
    float no2_pred;    // NO2 predicho en ug/m3
    float pm25_pred;   // PM2.5 predicho en ug/m3
    int   alerta;      // 1 = supera algun limite TULSMA, 0 = dentro del rango
} Prediccion;

// ------------------------------------------------------------
// PROTOTIPOS: VALIDACION DE ENTRADA
// ------------------------------------------------------------
int   validarIntRango(int a, int b);
float validarFloat(float min, float max);
void  leerCadena(char *cadena, int n);
void  convertirMayuscula(char *c);

// ------------------------------------------------------------
// PROTOTIPOS: PERSISTENCIA
// ------------------------------------------------------------
void cargarZonas(Zona *zonas, int *n);
void guardarZonas(Zona *zonas, int n);
void exportarReporte(Zona *zonas, int n, Prediccion *preds, int nPreds);

// ------------------------------------------------------------
// PROTOTIPOS: MENUS
// ------------------------------------------------------------
int menuPrincipal();
int menuZonas();
int menuMonitoreo();

// ------------------------------------------------------------
// PROTOTIPOS: GESTION DE ZONAS
// ------------------------------------------------------------
void registrarZona(Zona *zonas, int *n);
void mostrarZonas(Zona *zonas, int n);
int  buscarZona(Zona *zonas, int n);
void eliminarZona(Zona *zonas, int *n);

// ------------------------------------------------------------
// PROTOTIPOS: MONITOREO Y PREDICCION
// ------------------------------------------------------------
void ingresarMedicionHoy(Zona *zonas, int n);
void monitoreoActual(Zona *zonas, int n);
void calcularPromedios(Zona *zonas, int n);
Prediccion predecirManana(Zona *zona);
void mostrarPredicciones(Zona *zonas, int n, Prediccion *preds, int *nPreds);
void generarRecomendaciones(Zona *zonas, int n, Prediccion *preds, int nPreds);

#endif
