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

struct tsArt {   // max 10.000 - desordenado
  int codVen;    // 8 dígitos
  short codRub;  // 2 dígitos
  str30 descArt;
  ushort stock;     // 4 dígitos
  float percioUni;  // 6,2
  str10 medida;
  short ofertas[14];  //= {tipo descuento; porcentaje; tipo; porcentaje...}
};  // Cada par de "ofertas" es de un dia específico de la semana
// ofertas[0] = oferta_domingo

struct tsIndDesc {  // ordenado por descripción
  str30 descArt;
  int posArt;
  bool estado;
};

struct tsRub {  // hay 15 - ordenado por codigo
  short codRub;
  str20 descRub;
};

struct tsCompra {  // desordenado
  str30 descArt;
  short cantReq;  // 2 dígitos
};

const ushort MAX_ART = 10000;
const ushort CANT_RUB = 15;
typedef tsArt tvsArt[MAX_ART];
typedef tsIndDesc tvsIndDesc[MAX_ART];
typedef tsRub tvsRub[CANT_RUB];
typedef tsCompra tvsListCmp[MAX_ART];  // lista compras

#define ARCHIVOS_LECTURA \
  fstream &Art, ifstream &IndDesc, ifstream &Rub, ifstream &ListCmp
#define ARCHIVOS ARCHIVOS_LECTURA, ofstream &Ticket, ofstream &ListArt
#define REGISTROS \
  tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsRub &vsRub, tvsListCmp &vsListCmp

// Declaraciones de funciones
// Las que tienen comentario '// Falta' no están definidas
// Puede que tengan que cambiar, eliminar o agregar algunos parámetros
// Las declaraciones que tienen errores son porque todavía no definimos los
// parámetros de esa función

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);
bool LeerArticulo(ifstream &Art, tsArt &sArt);                          // Falta
bool LeerDescripcion(ifstream &IndDesc, tsIndDesc &sIndDesc);           // Falta
bool LeerRubro(ifstream &Rub, tsRub &sRub);                             // Falta
bool LeerCompra(ifstream &ListCmp, tsCompra &sCompra);                  // Falta
void PieTicket(float impTot, float impTotDesto, float impTotConDesto);  // Falta
void CabeceraTicket(int &ds);                                           // Falta
void OrdxBur(tvsArt &vsArt, ushort card);                               // Falta
void IntCmb(tsArt &sElem1, tsArt &sElem2);                              // Falta
void ActLinea(fstream &Art, tsArt &sArt);                               // Falta
int BusBinVec(tvsIndDesc &vsIndDesc, str30 &descArt, ushort ult);       // Falta
string Replicate(char car, ushort n);                                   // Falta
void Abrir(ARCHIVOS);                                                   // Falta
void VolcarArchivos(ARCHIVOS_LECTURA, REGISTROS, ushort &cantArt,
                    ushort &cantCmp);  // Falta
void ProcCompras(fstream &Art, tvsArt &vsArt, tvsIndDesc &vsIndDesc,
                 tvsListCmp &vsListCmp, ushort cantArt,
                 ushort cantCmp);  // Falta
void EmitirTicket(ofstream &Ticket, tvsArt &vsArt, tvsIndDesc &vsIndDesc,
                  tvsListCmp &vsListCmp, ushort cantArt,
                  ushort cantCmp);  // Falta
void EmitirArt_x_Rubro(ofstream &ListArt, tvsArt &vsArt, tvsRub &vsRub,
                       ushort cantArt);  // Falta
void Cerrar(ARCHIVOS);                   // Falta

int main() {
  tvsArt vsArt;
  tvsIndDesc vsIndDesc;
  tvsRub vsRub;
  tvsListCmp vsListCmp;
  fstream Art;                     // Lectura y Escritura
  ifstream IndDesc, Rub, ListCmp;  // Lectura
  ofstream Ticket, ListArt;        // Escritura
  ushort cantArt = 0, cantCmp = 0;

  Abrir(Art, IndDesc, Rub, ListCmp, Ticket, ListArt);
  VolcarArchivos(Art, IndDesc, Rub, ListCmp, vsArt, vsIndDesc, vsRub, vsListCmp,
                 cantArt, cantCmp);
  ProcCompras(Art, vsArt, vsIndDesc, vsListCmp, cantArt, cantCmp);
  EmitirTicket(ListArt, vsArt, vsIndDesc, vsListCmp, cantArt, cantCmp);
  EmitirArt_x_Rubro(Ticket, vsArt, vsRub, cantArt);
  Cerrar(Art, IndDesc, Rub, ListCmp, Ticket, ListArt);
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
