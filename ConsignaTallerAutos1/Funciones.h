// ============================================================
// funciones.h
// Archivo de cabecera: definicion de estructuras y prototipos
// Sistema de Gestion de Inventario y Clientes - Concesionaria
// ============================================================

#ifndef FUNCIONES_H
#define FUNCIONES_H

// ------------------------------------------------------------
// ESTRUCTURAS DE DATOS
// ------------------------------------------------------------

// Estructura que representa un vehiculo del inventario
typedef struct
{
    int     id;             // Identificador unico del vehiculo
    char    marca[50];      // Marca del vehiculo (ej. Chevrolet, Toyota)
    char    modelo[50];     // Modelo del vehiculo (ej. D-MAX, Hilux)
    char    tipo[30];       // Tipo: camioneta, sedan, SUV, etc.
    int     anio;           // Anio de fabricacion
    float   precio;         // Precio de venta en dolares
    char    condicion;      // 'N' = Nuevo, 'U' = Usado
    char    disponible;     // 'S' = Disponible, 'V' = Vendido
} Vehiculo;

// Estructura que representa un cliente registrado
typedef struct
{
    int     id;             // Identificador unico del cliente
    char    nombre[80];     // Nombre completo del cliente
    int     edad;           // Edad del cliente
    char    telefono[20];   // Numero de telefono de contacto
} Cliente;

// Estructura que representa una venta realizada
typedef struct
{
    int     id;             // Identificador unico de la venta
    int     idCliente;      // ID del cliente que compro
    int     idVehiculo;     // ID del vehiculo vendido
    char    fecha[12];      // Fecha de la venta (dd/mm/aaaa)
    float   precioFinal;    // Precio final negociado
} Venta;

// ------------------------------------------------------------
// NOMBRES DE ARCHIVOS DE PERSISTENCIA
// ------------------------------------------------------------
#define ARCHIVO_VEHICULOS   "vehiculos.dat"
#define ARCHIVO_CLIENTES    "clientes.dat"
#define ARCHIVO_VENTAS      "ventas.dat"

// Maximo de registros en memoria
#define MAX_VEHICULOS   50
#define MAX_CLIENTES    50
#define MAX_VENTAS      50

// ------------------------------------------------------------
// PROTOTIPOS DE FUNCIONES DE VALIDACION
// ------------------------------------------------------------
int   validarIntRango(int a, int b);
float validarFloat(float min, float max);
void  leerCadena(char *cadena, int n);
void  convertirMayuscula(char *c);

// ------------------------------------------------------------
// PROTOTIPOS DE FUNCIONES DE ARCHIVOS
// ------------------------------------------------------------
void cargarVehiculos(Vehiculo *vehiculos, int *n);
void guardarVehiculos(Vehiculo *vehiculos, int n);
void cargarClientes(Cliente *clientes, int *n);
void guardarClientes(Cliente *clientes, int n);
void cargarVentas(Venta *ventas, int *n);
void guardarVentas(Venta *ventas, int n);

// ------------------------------------------------------------
// PROTOTIPOS DE FUNCIONES DE MENU
// ------------------------------------------------------------
int  menuPrincipal();
int  menuVehiculos();
int  menuClientes();

// ------------------------------------------------------------
// PROTOTIPOS DE FUNCIONES DE VEHICULOS
// ------------------------------------------------------------
void registrarVehiculo(Vehiculo *vehiculos, int *n);
void mostrarVehiculos(Vehiculo *vehiculos, int n);
int  buscarVehiculo(Vehiculo *vehiculos, int n);
void buscarPorPreferencias(Vehiculo *vehiculos, int n);
void actualizarVehiculo(Vehiculo *vehiculos, int n);
void eliminarVehiculo(Vehiculo *vehiculos, int *n);

// ------------------------------------------------------------
// PROTOTIPOS DE FUNCIONES DE CLIENTES Y VENTAS
// ------------------------------------------------------------
void registrarCliente(Cliente *clientes, int *n);
void mostrarClientes(Cliente *clientes, int n);
int  buscarCliente(Cliente *clientes, int n);
void registrarVenta(Vehiculo *vehiculos, int nV,
                    Cliente  *clientes,  int nC,
                    Venta    *ventas,    int *nVt);
void mostrarVentas(Venta *ventas, int nVt,
                   Vehiculo *vehiculos, int nV,
                   Cliente  *clientes,  int nC);

#endif
