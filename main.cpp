// NO USAR MEMORIA DINÁMICA
#include <fstream>
#include <iostream>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef unsigned short ushort;

struct Articulo {   // max 10.000 - desordenado
  int cod_ven;      // 8 dígitos
  short cod_rubro;  // 2 dígitos
  str30 desc_articulo;
  ushort stock;      // 4 dígitos
  float percio_uni;  // 6,2
};

struct Descripcion {  // ordenado por descripción
  str30 desc_articulo;
  int pos_articulo;
  bool estado;
};

struct Rubro {  // hay 15 - ordenado por codigo
  short cod_rubro;
  str20 desc_rubro;
};

struct ListaCompras {  // desordenado
  str30 desc_articulo;
  short cant_requerida;  // 2 dígitos
};

const ushort kMaxArticulos = 10000;
const ushort kCantRubros = 15;
typedef Articulo Articulos[kMaxArticulos];
typedef Descripcion Descripciones[kMaxArticulos];  // no se que tamaño
typedef Rubro Rubros[kCantRubros];
typedef ListaCompras ListasCompras[kMaxArticulos];  // no se que tamaño

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);
void Abrir(Articulos &Articulos, Descripciones &descripcion, Rubros &rubro,
           ListasCompras &ListaCompras);

int main() {
  Articulos articulo;
  Descripciones descripcion;
  Rubros rubro;
  ListasCompras ListaCompras;

  // Abrir(Articulos, IndDescripArt, Rubros, ListaCompras);
  // VolcarArchivos(lista de parámetros que
  //                    correspondan);  // indicados por el grupo de trabajo.
  // ProcCompras(lista de parámetros que correspondan);
  // EmitirTicket(lista de parámetros que correspondan);
  // EmitirArt_x_Rubro(lista de parámetros que correspondan);
  // Cerrar(Articulos, IndDescripArt, Rubros, ListaCompras);
  return 0;
}

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
