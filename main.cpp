// NO USAR MEMORIA DINÁMICA (new, delete, mallo, realloc, free)
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

// Constantes globales
const ushort kMaxArticulos = 10000;
const ushort kCantRubros = 15;

// Registros / structs
struct Articulo {   // max 10.000 - desordenado
  int cod_ven;      // 8 dígitos
  short cod_rubro;  // 2 dígitos
  str30 desc_articulo;
  ushort stock;      // 4 dígitos
  float percio_uni;  // 6,2
  str10 medida;
  short ofertas[14];  //= {tipo descuento; porcentaje; tipo; porcentaje...}
};

struct IndDescripcion {  // ordenado por descripción
  str30 desc_articulo;
  int pos_articulo;
  bool estado;
};

struct Rubro {  // hay 15 - ordenado por codigo
  short cod_rubro;
  str20 desc_rubro;
};

struct Compra {  // desordenado
  str30 desc_articulo;
  short cant_requerida;  // 2 dígitos
};

typedef Articulo Articulos[kMaxArticulos];
typedef IndDescripcion IndDescripciones[kMaxArticulos];
typedef Rubro Rubros[kCantRubros];
typedef Compra ListasCompras[kMaxArticulos];

// Declaraciones de funciones (las que dan error son las que no sabemos que tipo
// de variable deberían ser sus parámetros)

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);
bool LeerArticulo(ifstream &archivo, Articulo &articulo);               // Falta
bool LeerDescripcion(ifstream &archivo, IndDescripcion &ind_desc);      // Falta
bool LeerRubro(ifstream &archivo, Rubro &rubro);                        // Falta
bool LeerCompra(ifstream &archivo, Compra &ListaCompras);               // Falta
void PieTicket(float impTot, float impTotDesto, float impTotConDesto);  // Falta
void CabeceraTicket(int &ds);                                           // Falta
void OrdxBur(tid tbl, tid card);                                        // Falta
void IntCmb(id &elem1, id &elem2);                                      // Falta
void ActLinea(fstream &arch_articulos, sid id);                         // Falta
int BusBinVec(tbl id, tid clv, tid ult);                                // Falta
string Replicate(char car, unsigned n);                                 // Falta
void Abrir(fstream &arch_articulos, ifstream &arch_ind_desc,
           ifstream &arch_rubros, ifstream &arch_compras);  // Falta
void Cerrar(fstream &arch_articulos, ifstream &arch_ind_desc,
            ifstream &arch_rubros, ifstream &arch_compras);  // Falta

int main() {
  Articulos articulos;
  IndDescripciones descripciones;
  Rubros rubros;
  ListasCompras ListaCompras;
  fstream arch_articulos;
  ifstream arch_ind_desc, arch_rubros, arch_compras;

  Abrir(arch_articulos, arch_ind_desc, arch_rubros, arch_compras);
  // VolcarArchivos(lista de parámetros que
  //                    correspondan);  // indicados por el grupo de trabajo.
  // ProcCompras(lista de parámetros que correspondan);
  // EmitirTicket(lista de parámetros que correspondan);
  // EmitirArt_x_Rubro(lista de parámetros que correspondan);
  Cerrar(arch_articulos, arch_ind_desc, arch_rubros, arch_compras);
  return 0;
}

// Definiciones de funciones

long GetTime(int &hh, int &mm, int &ss) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  hh = timeinfo->tm_hour;
  mm = timeinfo->tm_min;
  ss = timeinfo->tm_sec;
  return timeinfo->tm_hour * 10000 + timeinfo->tm_min * 100 + timeinfo->tm_sec;
}  // GetTime

long GetDate(int &year, int &mes, int &dia, int &ds) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  year = 1900 + timeinfo->tm_year;
  mes = 1 + timeinfo->tm_mon;
  dia = timeinfo->tm_mday;
  ds = 1 + timeinfo->tm_wday;
  return (1900 + timeinfo->tm_year) * 10000 + (1 + timeinfo->tm_mon) * 100 +
         timeinfo->tm_mday;
}  // GetDate
