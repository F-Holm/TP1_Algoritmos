// NO USAR MEMORIA DINÁMICA (new, delete, malloc, realloc, free)
// Muchos comentarios van a ser eliminados en la entrega final
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

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

const ushort kMaxArticulos = 10000;
const ushort kCantRubros = 15;
typedef Articulo Articulos[kMaxArticulos];
typedef IndDescripcion IndDescripciones[kMaxArticulos];
typedef Rubro Rubros[kCantRubros];
typedef Compra ListaCompras[kMaxArticulos];

#define ARCHIVOS_LECTURA                                                   \
  fstream &arch_articulos, ifstream &arch_ind_desc, ifstream &arch_rubros, \
      ifstream &arch_compras
#define ARCHIVOS \
  ARCHIVOS_LECTURA, ofstream &arch_ticket, ofstream &arch_list_articulos
#define REGISTROS                                                            \
  Articulos &articulos, IndDescripciones &ind_descripciones, Rubros &rubros, \
      ListaCompras &lista_compras

// Declaraciones de funciones
// Las que tienen comentario '// Falta' no están definidas
// Puede que tengan que cambiar, eliminar o agregar algunos parámetros
// Las declaraciones que tienen errores son porque todavía no definimos los
// parámetros de esa función

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);
bool LeerArticulo(ifstream &archivo, Articulo &articulo);
bool LeerDescripcion(ifstream &archivo, IndDescripcion &ind_desc);
bool LeerRubro(ifstream &archivo, Rubro &rubro);
bool LeerCompra(ifstream &archivo, Compra &compra);
void PieTicket(float impTot, float impTotDesto, float impTotConDesto);  // Falta
void CabeceraTicket(int &ds);                                           // Falta
void OrdxBur(Articulos &articulos, ushort card);                        // Falta
void IntCmb(Articulo &elem1, Articulo &elem2);                          // Falta
void ActLinea(fstream &arch_articulos, Articulo &articulo);             // Falta
int BusBinVec(IndDescripciones &ind_descripciones, str30 &desc_articulo,
              ushort ult);             // Falta
string Replicate(char car, ushort n);  // Falta
void Abrir(ARCHIVOS);                  // Falta
void VolcarArchivos(ARCHIVOS_LECTURA, REGISTROS, ushort &cant_articulos,
                    ushort &cant_compras);  // Falta
void ProcCompras(fstream &arch_articulos, Articulos &articulos,
                 IndDescripciones &ind_descripciones,
                 ListaCompras &lista_compras, ushort cant_articulos,
                 ushort cant_compras);  // Falta
void EmitirTicket(ofstream &archivo, Articulos &articulos,
                  IndDescripciones &ind_descripciones,
                  ListaCompras &lista_compras, ushort cant_articulos,
                  ushort cant_compras);  // Falta
void EmitirArt_x_Rubro(ofstream &archivo, Articulos &articulos, Rubros &rubros,
                       ushort cant_articulos);  // Falta
void Cerrar(ARCHIVOS);                          // Falta

int main() {
  Articulos articulos;
  IndDescripciones ind_descripciones;
  Rubros rubros;
  ListaCompras lista_compras;
  fstream arch_articulos;                             // Lectura y Escritura
  ifstream arch_ind_desc, arch_rubros, arch_compras;  // Lectura
  ofstream arch_ticket, arch_list_articulos;          // Escritura
  ushort cant_articulos = 0, cant_compras = 0;

  Abrir(arch_articulos, arch_ind_desc, arch_rubros, arch_compras, arch_ticket,
        arch_list_articulos);
  VolcarArchivos(arch_articulos, arch_ind_desc, arch_rubros, arch_compras,
                 articulos, ind_descripciones, rubros, lista_compras,
                 cant_articulos, cant_compras);
  ProcCompras(arch_articulos, articulos, ind_descripciones, lista_compras,
              cant_articulos, cant_compras);
  EmitirTicket(arch_list_articulos, articulos, ind_descripciones, lista_compras,
               cant_articulos, cant_compras);
  EmitirArt_x_Rubro(arch_ticket, articulos, rubros, cant_articulos);
  Cerrar(arch_articulos, arch_ind_desc, arch_rubros, arch_compras, arch_ticket,
         arch_list_articulos);
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

bool LeerArticulo(ifstream &archivo, Articulo &articulo) {
  archivo >> articulo.cod_ven >> articulo.cod_rubro;
  archivo.get(articulo.desc_articulo,31);
  archivo >> articulo.stock >> articulo.percio_uni;
  archivo.get(articulo.medida,11);
  for (short i = 0;i < 14;i++)
    archivo >> articulo.ofertas[i];
  archivo.ignore();
  return archivo.good();
}

bool LeerDescripcion(ifstream &archivo, IndDescripcion &ind_desc) {
}

bool LeerRubro(ifstream &archivo, Rubro &rubro) {
}

bool LeerCompra(ifstream &archivo, Compra &compra) {
}