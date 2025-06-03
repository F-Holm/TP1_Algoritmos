// NO USAR MEMORIA DINÁMICA
#include <fstream>
#include <iostream>

using namespace std;

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);

int main() {
  // Declarar variables

  Abrir(Articulos, IndDescripArt, Rubros, ListaCompras);
  VolcarArchivos(lista de parámetros que
                     correspondan);  // indicados por el grupo de trabajo.
  ProcCompras(lista de parámetros que correspondan);
  EmitirTicket(lista de parámetros que correspondan);
  EmitirArt_x_Rubro(lista de parámetros que correspondan);
  Cerrar(Articulos, IndDescripArt, Rubros, ListaCompras);
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
