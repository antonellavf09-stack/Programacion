// ============================================================
// funciones.c
// Implementacion de todas las funciones del sistema SGIC
// Concesionaria "Ruedas de Oro"
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

// ============================================================
// FUNCIONES DE VALIDACION
// ============================================================

/*
 * validarIntRango: Lee un entero del usuario y lo valida
 * hasta que este dentro del rango [a, b].
 * Retorna el entero valido ingresado.
 */
int validarIntRango(int a, int b)
{
    int n, aux;
    do
    {
        aux = scanf("%d", &n);
        while (getchar() != '\n'); // Limpiar buffer
        if (aux != 1 || n < a || n > b)
        {
            printf("  Error: ingrese un valor entre %d y %d: ", a, b);
        }
    } while (aux != 1 || n < a || n > b);
    return n;
}

/*
 * validarFloat: Lee un numero decimal del usuario y lo valida
 * hasta que este dentro del rango [min, max].
 * Retorna el float valido ingresado.
 */
float validarFloat(float min, float max)
{
    float n;
    int aux;
    do
    {
        aux = scanf("%f", &n);
        while (getchar() != '\n'); // Limpiar buffer
        if (aux != 1 || n < min || n > max)
        {
            printf("  Error: ingrese un valor entre %.2f y %.2f: ", min, max);
        }
    } while (aux != 1 || n < min || n > max);
    return n;
}

/*
 * leerCadena: Lee una linea de texto desde la entrada estandar
 * y elimina el salto de linea al final.
 * Parametros: cadena = buffer destino, n = tamano maximo.
 */
void leerCadena(char *cadena, int n)
{
    fgets(cadena, n, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n')
        cadena[len] = '\0';
}

/*
 * convertirMayuscula: Convierte un caracter a mayuscula
 * si es una letra minuscula valida para estado/condicion.
 */
void convertirMayuscula(char *c)
{
    if (*c >= 'a' && *c <= 'z')
        *c = *c - 32; // Diferencia ASCII entre minuscula y mayuscula
}

// ============================================================
// FUNCIONES DE ARCHIVOS - VEHICULOS
// ============================================================

/*
 * cargarVehiculos: Lee todos los vehiculos del archivo binario
 * y los carga en el arreglo en memoria.
 * Si el archivo no existe, inicia con inventario vacio.
 */
void cargarVehiculos(Vehiculo *vehiculos, int *n)
{
    FILE *f = fopen(ARCHIVO_VEHICULOS, "rb");
    if (f == NULL)
    {
        *n = 0; // No hay archivo todavia, inventario vacio
        return;
    }
    // Leer todos los registros de una vez
    *n = fread(vehiculos, sizeof(Vehiculo), MAX_VEHICULOS, f);
    fclose(f);
}

/*
 * guardarVehiculos: Escribe todo el arreglo de vehiculos
 * en el archivo binario (sobreescribe el contenido anterior).
 */
void guardarVehiculos(Vehiculo *vehiculos, int n)
{
    FILE *f = fopen(ARCHIVO_VEHICULOS, "wb");
    if (f == NULL)
    {
        printf("  Error: no se pudo guardar el archivo de vehiculos.\n");
        return;
    }
    fwrite(vehiculos, sizeof(Vehiculo), n, f);
    fclose(f);
}

// ============================================================
// FUNCIONES DE ARCHIVOS - CLIENTES
// ============================================================

/*
 * cargarClientes: Lee todos los clientes del archivo binario.
 * Si el archivo no existe, inicia con lista vacia.
 */
void cargarClientes(Cliente *clientes, int *n)
{
    FILE *f = fopen(ARCHIVO_CLIENTES, "rb");
    if (f == NULL)
    {
        *n = 0;
        return;
    }
    *n = fread(clientes, sizeof(Cliente), MAX_CLIENTES, f);
    fclose(f);
}

/*
 * guardarClientes: Guarda todos los clientes en archivo binario.
 */
void guardarClientes(Cliente *clientes, int n)
{
    FILE *f = fopen(ARCHIVO_CLIENTES, "wb");
    if (f == NULL)
    {
        printf("  Error: no se pudo guardar el archivo de clientes.\n");
        return;
    }
    fwrite(clientes, sizeof(Cliente), n, f);
    fclose(f);
}

// ============================================================
// FUNCIONES DE ARCHIVOS - VENTAS
// ============================================================

/*
 * cargarVentas: Lee todas las ventas del archivo binario.
 */
void cargarVentas(Venta *ventas, int *n)
{
    FILE *f = fopen(ARCHIVO_VENTAS, "rb");
    if (f == NULL)
    {
        *n = 0;
        return;
    }
    *n = fread(ventas, sizeof(Venta), MAX_VENTAS, f);
    fclose(f);
}

/*
 * guardarVentas: Guarda todas las ventas en archivo binario.
 */
void guardarVentas(Venta *ventas, int n)
{
    FILE *f = fopen(ARCHIVO_VENTAS, "wb");
    if (f == NULL)
    {
        printf("  Error: no se pudo guardar el archivo de ventas.\n");
        return;
    }
    fwrite(ventas, sizeof(Venta), n, f);
    fclose(f);
}

// ============================================================
// FUNCIONES DE MENU
// ============================================================

/*
 * menuPrincipal: Muestra el menu principal del sistema
 * y retorna la opcion seleccionada por el usuario (1-4).
 */
int menuPrincipal()
{
    printf("\n========================================\n");
    printf("   CONCESIONARIA RUEDAS DE ORO - SGIC  \n");
    printf("========================================\n");
    printf("  1. Gestion de Vehiculos\n");
    printf("  2. Gestion de Clientes\n");
    printf("  3. Registrar Venta\n");
    printf("  4. Historial de Ventas\n");
    printf("  5. Salir\n");
    printf("----------------------------------------\n");
    printf("  Seleccione una opcion: ");
    return validarIntRango(1, 5);
}

/*
 * menuVehiculos: Muestra el submenu de vehiculos
 * y retorna la opcion seleccionada (1-6).
 */
int menuVehiculos()
{
    printf("\n--- GESTION DE VEHICULOS ---\n");
    printf("  1. Registrar vehiculo\n");
    printf("  2. Mostrar todos los vehiculos\n");
    printf("  3. Buscar por ID o Marca\n");
    printf("  4. Buscar por preferencias (tipo/marca/precio)\n");
    printf("  5. Actualizar disponibilidad\n");
    printf("  6. Eliminar vehiculo\n");
    printf("  7. Regresar al menu principal\n");
    printf("  Seleccione: ");
    return validarIntRango(1, 7);
}

/*
 * menuClientes: Muestra el submenu de clientes
 * y retorna la opcion seleccionada (1-3).
 */
int menuClientes()
{
    printf("\n--- GESTION DE CLIENTES ---\n");
    printf("  1. Registrar cliente\n");
    printf("  2. Mostrar todos los clientes\n");
    printf("  3. Buscar cliente por ID o nombre\n");
    printf("  4. Regresar al menu principal\n");
    printf("  Seleccione: ");
    return validarIntRango(1, 4);
}

// ============================================================
// FUNCIONES DE VEHICULOS
// ============================================================

/*
 * registrarVehiculo: Solicita los datos de un vehiculo nuevo,
 * valida que el ID no se repita, lo agrega al arreglo
 * y lo persiste en el archivo. Permite registrar varios seguidos.
 */
void registrarVehiculo(Vehiculo *vehiculos, int *n)
{
    // Verificar que no se supere el maximo permitido
    if (*n >= MAX_VEHICULOS)
    {
        printf("  Error: inventario lleno (maximo %d vehiculos).\n", MAX_VEHICULOS);
        return;
    }

    printf("\n-- Registro de Vehiculo --\n");

    // Ingresar y validar el ID unico
    printf("  ID del vehiculo: ");
    vehiculos[*n].id = validarIntRango(1, 999999);

    for (int i = 0; i < *n; i++)
    {
        if (vehiculos[i].id == vehiculos[*n].id)
        {
            printf("  Error: ya existe un vehiculo con ese ID.\n");
            return;
        }
    }

    // Ingresar datos del vehiculo
    printf("  Marca: ");
    leerCadena(vehiculos[*n].marca, 50);

    printf("  Modelo: ");
    leerCadena(vehiculos[*n].modelo, 50);

    printf("  Tipo (camioneta/sedan/SUV/hatchback/otro): ");
    leerCadena(vehiculos[*n].tipo, 30);

    printf("  Anio (1990-2026): ");
    vehiculos[*n].anio = validarIntRango(1990, 2026);

    printf("  Precio (USD): ");
    vehiculos[*n].precio = validarFloat(500.0f, 200000.0f);

    printf("  Condicion (N=Nuevo / U=Usado): ");
    scanf(" %c", &vehiculos[*n].condicion);
    while (getchar() != '\n');
    convertirMayuscula(&vehiculos[*n].condicion);

    // Validar que la condicion sea N o U
    if (vehiculos[*n].condicion != 'N' && vehiculos[*n].condicion != 'U')
    {
        printf("  Error: condicion invalida. Use N o U.\n");
        return;
    }

    // El vehiculo nuevo siempre inicia como disponible
    vehiculos[*n].disponible = 'S';

    (*n)++;

    // Persistir los cambios en el archivo
    guardarVehiculos(vehiculos, *n);
    printf("  Vehiculo registrado exitosamente.\n");

    // Opcion de registrar otro vehiculo
    printf("  Desea registrar otro vehiculo? 1. Si / 2. No: ");
    int opc = validarIntRango(1, 2);
    if (opc == 1)
        registrarVehiculo(vehiculos, n);
    else
        printf("  Regresando al menu...\n");
}

/*
 * mostrarVehiculos: Muestra la lista completa del inventario
 * en formato tabla. Permite ver el detalle de un vehiculo especifico.
 */
void mostrarVehiculos(Vehiculo *vehiculos, int n)
{
    if (n == 0)
    {
        printf("  El inventario esta vacio.\n");
        return;
    }

    printf("\n%-5s %-12s %-12s %-12s %-6s %-10s %-5s %-5s\n",
           "#", "ID", "Marca", "Modelo", "Anio", "Precio", "Cond", "Disp");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%-5d %-12d %-12s %-12s %-6d %-10.2f %-5c %-5c\n",
               i + 1,
               vehiculos[i].id,
               vehiculos[i].marca,
               vehiculos[i].modelo,
               vehiculos[i].anio,
               vehiculos[i].precio,
               vehiculos[i].condicion,
               vehiculos[i].disponible);
    }

    // Oferta de ver detalle completo de un vehiculo
    printf("\n  Desea ver detalle de un vehiculo? 1. Si / 2. No: ");
    int opc = validarIntRango(1, 2);
    if (opc == 1)
    {
        printf("  Ingrese el numero del vehiculo (#): ");
        int num = validarIntRango(1, n);
        int idx = num - 1;
        printf("\n  --- Detalle del Vehiculo ---\n");
        printf("  ID        : %d\n",   vehiculos[idx].id);
        printf("  Marca     : %s\n",   vehiculos[idx].marca);
        printf("  Modelo    : %s\n",   vehiculos[idx].modelo);
        printf("  Tipo      : %s\n",   vehiculos[idx].tipo);
        printf("  Anio      : %d\n",   vehiculos[idx].anio);
        printf("  Precio    : $%.2f\n",vehiculos[idx].precio);
        printf("  Condicion : %c\n",   vehiculos[idx].condicion);
        printf("  Disponible: %c\n",   vehiculos[idx].disponible);
    }
}

/*
 * buscarVehiculo: Busca un vehiculo por ID (numero) o por Marca (texto).
 * Retorna el indice en el arreglo si lo encuentra, o -1 si no.
 */
int buscarVehiculo(Vehiculo *vehiculos, int n)
{
    if (n == 0)
    {
        printf("  El inventario esta vacio.\n");
        return -1;
    }

    char busqueda[50];
    printf("  Ingrese el ID o Marca del vehiculo a buscar: ");
    leerCadena(busqueda, 50);

    for (int i = 0; i < n; i++)
    {
        // Comparar con ID (si el texto ingresado es numerico)
        // o con la marca (comparacion sin importar exactitud del tipo)
        if (atoi(busqueda) == vehiculos[i].id ||
            strcmp(busqueda, vehiculos[i].marca) == 0)
        {
            printf("  Vehiculo encontrado: ID=%d | %s %s | $%.2f | Disp: %c\n",
                   vehiculos[i].id, vehiculos[i].marca,
                   vehiculos[i].modelo, vehiculos[i].precio,
                   vehiculos[i].disponible);
            return i;
        }
    }

    printf("  Vehiculo no encontrado.\n");
    return -1;
}

/*
 * buscarPorPreferencias: Busca vehiculos que cumplan con
 * el tipo, marca y precio maximo indicados por el usuario.
 * Caso de uso: el vendedor busca opciones para un cliente especifico.
 */
void buscarPorPreferencias(Vehiculo *vehiculos, int n)
{
    if (n == 0)
    {
        printf("  El inventario esta vacio.\n");
        return;
    }

    char tipo[30], marca[50];
    float presupuesto;

    printf("\n  -- Busqueda por Preferencias del Cliente --\n");
    printf("  Tipo de vehiculo (camioneta/sedan/SUV/otro, o ENTER para cualquiera): ");
    leerCadena(tipo, 30);

    printf("  Marca preferida (o ENTER para cualquier marca): ");
    leerCadena(marca, 50);

    printf("  Presupuesto maximo en USD (0 = sin limite): ");
    presupuesto = validarFloat(0.0f, 200000.0f);

    printf("\n  -- Vehiculos que coinciden con las preferencias --\n");
    printf("%-5s %-12s %-12s %-12s %-6s %-10s %-5s\n",
           "ID", "Marca", "Modelo", "Tipo", "Anio", "Precio", "Cond");
    printf("-----------------------------------------------------------\n");

    int encontrados = 0;
    for (int i = 0; i < n; i++)
    {
        // Filtrar solo vehiculos disponibles
        if (vehiculos[i].disponible != 'S')
            continue;

        // Filtrar por tipo (si se ingreso un tipo)
        if (strlen(tipo) > 0 && strcmp(tipo, vehiculos[i].tipo) != 0)
            continue;

        // Filtrar por marca (si se ingreso una marca)
        if (strlen(marca) > 0 && strcmp(marca, vehiculos[i].marca) != 0)
            continue;

        // Filtrar por presupuesto (si se indico un limite)
        if (presupuesto > 0 && vehiculos[i].precio > presupuesto)
            continue;

        // Si pasa todos los filtros, mostrar el vehiculo
        printf("%-5d %-12s %-12s %-12s %-6d %-10.2f %-5c\n",
               vehiculos[i].id, vehiculos[i].marca, vehiculos[i].modelo,
               vehiculos[i].tipo, vehiculos[i].anio,
               vehiculos[i].precio, vehiculos[i].condicion);
        encontrados++;
    }

    if (encontrados == 0)
        printf("  No se encontraron vehiculos con esas preferencias.\n");
    else
        printf("\n  Total encontrados: %d vehiculo(s).\n", encontrados);
}

/*
 * actualizarVehiculo: Permite cambiar la disponibilidad
 * de un vehiculo (S=disponible / V=vendido).
 * Busca el vehiculo primero y luego actualiza el campo.
 */
void actualizarVehiculo(Vehiculo *vehiculos, int n)
{
    int index = buscarVehiculo(vehiculos, n);
    if (index == -1)
        return;

    printf("  Nueva disponibilidad (S=Disponible / V=Vendido): ");
    scanf(" %c", &vehiculos[index].disponible);
    while (getchar() != '\n');
    convertirMayuscula(&vehiculos[index].disponible);

    if (vehiculos[index].disponible != 'S' && vehiculos[index].disponible != 'V')
    {
        printf("  Error: estado invalido. Use S o V.\n");
        return;
    }

    guardarVehiculos(vehiculos, n);
    printf("  Disponibilidad actualizada correctamente.\n");
}

/*
 * eliminarVehiculo: Elimina un vehiculo del inventario buscandolo
 * por ID o marca. Desplaza los elementos posteriores para mantener
 * el arreglo compacto, luego guarda los cambios.
 */
void eliminarVehiculo(Vehiculo *vehiculos, int *n)
{
    int index = buscarVehiculo(vehiculos, *n);
    if (index == -1)
        return;

    // Confirmar antes de eliminar
    printf("  Confirma la eliminacion del vehiculo ID=%d? 1. Si / 2. No: ",
           vehiculos[index].id);
    int conf = validarIntRango(1, 2);
    if (conf == 2)
    {
        printf("  Operacion cancelada.\n");
        return;
    }

    // Desplazar elementos hacia la izquierda para llenar el hueco
    for (int i = index; i < *n - 1; i++)
        vehiculos[i] = vehiculos[i + 1];

    (*n)--;
    guardarVehiculos(vehiculos, *n);
    printf("  Vehiculo eliminado exitosamente.\n");
}

// ============================================================
// FUNCIONES DE CLIENTES
// ============================================================

/*
 * registrarCliente: Solicita los datos de un cliente nuevo,
 * valida que el ID no exista, lo agrega al arreglo y lo guarda.
 */
void registrarCliente(Cliente *clientes, int *n)
{
    if (*n >= MAX_CLIENTES)
    {
        printf("  Error: lista de clientes llena.\n");
        return;
    }

    printf("\n-- Registro de Cliente --\n");

    printf("  ID del cliente: ");
    clientes[*n].id = validarIntRango(1, 999999);

    // Verificar ID duplicado
    for (int i = 0; i < *n; i++)
    {
        if (clientes[i].id == clientes[*n].id)
        {
            printf("  Error: ya existe un cliente con ese ID.\n");
            return;
        }
    }

    printf("  Nombre completo: ");
    leerCadena(clientes[*n].nombre, 80);

    printf("  Edad: ");
    clientes[*n].edad = validarIntRango(18, 100);

    printf("  Telefono: ");
    leerCadena(clientes[*n].telefono, 20);

    (*n)++;
    guardarClientes(clientes, *n);
    printf("  Cliente registrado exitosamente.\n");

    printf("  Desea registrar otro cliente? 1. Si / 2. No: ");
    int opc = validarIntRango(1, 2);
    if (opc == 1)
        registrarCliente(clientes, n);
}

/*
 * mostrarClientes: Lista todos los clientes registrados.
 */
void mostrarClientes(Cliente *clientes, int n)
{
    if (n == 0)
    {
        printf("  No hay clientes registrados.\n");
        return;
    }

    printf("\n%-5s %-10s %-30s %-6s %-15s\n",
           "#", "ID", "Nombre", "Edad", "Telefono");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%-5d %-10d %-30s %-6d %-15s\n",
               i + 1, clientes[i].id, clientes[i].nombre,
               clientes[i].edad, clientes[i].telefono);
    }
}

/*
 * buscarCliente: Busca un cliente por su ID (numerico)
 * o por su nombre (texto exacto).
 * Retorna el indice del cliente encontrado o -1.
 */
int buscarCliente(Cliente *clientes, int n)
{
    if (n == 0)
    {
        printf("  No hay clientes registrados.\n");
        return -1;
    }

    char busqueda[80];
    printf("  Ingrese el ID o nombre del cliente a buscar: ");
    leerCadena(busqueda, 80);

    for (int i = 0; i < n; i++)
    {
        if (atoi(busqueda) == clientes[i].id ||
            strcmp(busqueda, clientes[i].nombre) == 0)
        {
            printf("  Cliente encontrado: ID=%d | %s | Edad: %d | Tel: %s\n",
                   clientes[i].id, clientes[i].nombre,
                   clientes[i].edad, clientes[i].telefono);
            return i;
        }
    }

    printf("  Cliente no encontrado.\n");
    return -1;
}

// ============================================================
// FUNCIONES DE VENTAS
// ============================================================

/*
 * registrarVenta: Registra la compra de un vehiculo por parte
 * de un cliente. Busca ambos por ID, verifica que el vehiculo
 * este disponible, solicita la fecha y el precio final,
 * marca el vehiculo como vendido y guarda todo en archivos.
 */
void registrarVenta(Vehiculo *vehiculos, int nV,
                    Cliente  *clientes,  int nC,
                    Venta    *ventas,    int *nVt)
{
    if (*nVt >= MAX_VENTAS)
    {
        printf("  Error: registro de ventas lleno.\n");
        return;
    }

    printf("\n-- Registro de Venta --\n");

    // Buscar el cliente
    printf("  Busqueda del cliente:\n  ");
    int idxCliente = buscarCliente(clientes, nC);
    if (idxCliente == -1)
        return;

    // Buscar el vehiculo
    printf("  Busqueda del vehiculo:\n  ");
    int idxVehiculo = buscarVehiculo(vehiculos, nV);
    if (idxVehiculo == -1)
        return;

    // Verificar que el vehiculo este disponible
    if (vehiculos[idxVehiculo].disponible != 'S')
    {
        printf("  Error: ese vehiculo ya fue vendido y no esta disponible.\n");
        return;
    }

    // Ingresar datos de la venta
    printf("  ID de la venta: ");
    ventas[*nVt].id = validarIntRango(1, 999999);

    ventas[*nVt].idCliente  = clientes[idxCliente].id;
    ventas[*nVt].idVehiculo = vehiculos[idxVehiculo].id;

    printf("  Fecha de la venta (dd/mm/aaaa): ");
    leerCadena(ventas[*nVt].fecha, 12);

    printf("  Precio final negociado (USD, precio lista: $%.2f): ",
           vehiculos[idxVehiculo].precio);
    ventas[*nVt].precioFinal = validarFloat(1.0f, 200000.0f);

    // Marcar el vehiculo como vendido
    vehiculos[idxVehiculo].disponible = 'V';

    (*nVt)++;

    // Guardar todos los cambios en archivos
    guardarVentas(ventas, *nVt);
    guardarVehiculos(vehiculos, nV);

    printf("  Venta registrada exitosamente.\n");
    printf("  Cliente : %s\n", clientes[idxCliente].nombre);
    printf("  Vehiculo: %s %s (ID %d)\n",
           vehiculos[idxVehiculo].marca,
           vehiculos[idxVehiculo].modelo,
           vehiculos[idxVehiculo].id);
    printf("  Precio  : $%.2f\n", ventas[*nVt - 1].precioFinal);
    printf("  Fecha   : %s\n",    ventas[*nVt - 1].fecha);
}

/*
 * mostrarVentas: Muestra el historial completo de ventas
 * con el nombre del cliente y los datos del vehiculo vendido.
 */
void mostrarVentas(Venta *ventas, int nVt,
                   Vehiculo *vehiculos, int nV,
                   Cliente  *clientes,  int nC)
{
    if (nVt == 0)
    {
        printf("  No hay ventas registradas.\n");
        return;
    }

    printf("\n-- Historial de Ventas --\n");
    printf("%-6s %-20s %-18s %-12s %-10s\n",
           "VentaID", "Cliente", "Vehiculo", "Fecha", "Precio");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < nVt; i++)
    {
        // Buscar nombre del cliente por ID
        char nombreCliente[80] = "Desconocido";
        for (int j = 0; j < nC; j++)
        {
            if (clientes[j].id == ventas[i].idCliente)
            {
                strcpy(nombreCliente, clientes[j].nombre);
                break;
            }
        }

        // Buscar datos del vehiculo por ID
        char descVehiculo[110] = "Desconocido";
        for (int j = 0; j < nV; j++)
        {
            if (vehiculos[j].id == ventas[i].idVehiculo)
            {
                snprintf(descVehiculo, 110, "%s %s",
                         vehiculos[j].marca, vehiculos[j].modelo);
                break;
            }
        }

        printf("%-6d %-20s %-18s %-12s $%-10.2f\n",
               ventas[i].id, nombreCliente, descVehiculo,
               ventas[i].fecha, ventas[i].precioFinal);
    }
}
