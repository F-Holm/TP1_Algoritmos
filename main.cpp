// NO USAR MEMORIA DINÁMICA (new, delete, malloc, realloc, free)
// Muchos comentarios van a ser eliminados en la entrega final
// UTILIZAR LA NOMENCLATURA DE HUGO CUELLO
#include <cstdio>
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
  ushort stock;  // 4 dígitos
  float preUni;  // 6,2
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

struct tsCompra {  // desordenado - máx 100
  str30 descArt;
  short cantReq;  // 2 dígitos
};

const ushort MAX_ART = 10000;
const ushort CANT_RUB = 15;
const ushort MAX_COMPRAS = 100;
typedef tsArt tvsArt[MAX_ART];
typedef tsIndDesc tvsIndDesc[MAX_ART];
typedef tsRub tvsRub[CANT_RUB];
typedef tsCompra tvsListCmp[MAX_COMPRAS];  // lista compras

#define ARCHIVOS \
  fstream &Art, ifstream &IndDesc, ifstream &Rub, ifstream &ListCmpr
#define REGISTROS \
  tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsRub &vsRub, tvsListCmp &vsListCmpr
#define ARCHIVO_ART fstream &Art
#define REG_COMPRAS tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsListCmp &vsListCmpr

// Declaraciones de funciones
// Las que tienen comentario '// Falta' no están definidas
// Puede que tengan que cambiar, eliminar o agregar algunos parámetros
// Las declaraciones que tienen errores son porque todavía no definimos los
// parámetros de esa función

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);
bool LeerArticulo(fstream &Art, tsArt &sArt);
bool LeerDescripcion(ifstream &IndDesc, tsIndDesc &sIndDesc);
bool LeerRubro(ifstream &Rub, tsRub &sRub);
bool LeerCompra(ifstream &ListCmpr, tsCompra &sCompra);
void PieTicket(float impTot, float impTotDesto, float impTotConDesto);
void CabeceraTicket(int &ds);  // Falta
void OrdxBur(tvsArt &vsArt, ushort card);
void IntCmb(tsArt &sElem1, tsArt &sElem2);
void ActLinea(fstream &Art, tsArt &sArt);                          // Falta
int BusBinVec(tvsIndDesc &vsIndDesc, str30 &descArt, ushort ult); 
string Replicate(char car, ushort n);
void Abrir(ARCHIVOS);
void VolcarArchivos(ARCHIVOS, REGISTROS, ushort &cantArt, ushort &cantCmpr);
void ProcCompras(ARCHIVO_ART, REG_COMPRAS, ushort cantArt, ushort cantCmpr);

void EmitirTicket(tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsListCmp &vsListCmpr,
                  ushort cantArt, ushort cantCmpr);                    // Falta
void EmitirArt_x_Rubro(tvsArt &vsArt, tvsRub &vsRub, ushort cantArt);  // Falta
void Cerrar(ARCHIVOS);

int main() {
  tvsArt vsArt;
  tvsIndDesc vsIndDesc;
  tvsRub vsRub;
  tvsListCmp vsListCmpr;
  fstream Art;
  ifstream IndDesc, Rub, ListCmpr;
  ushort cantArt, cantCmpr;

  Abrir(Art, IndDesc, Rub, ListCmpr);
  VolcarArchivos(Art, IndDesc, Rub, ListCmpr, vsArt, vsIndDesc, vsRub,
                 vsListCmpr, cantArt, cantCmpr);
  ProcCompras(Art, vsArt, vsIndDesc, vsListCmpr, cantArt, cantCmpr);
  EmitirTicket(vsArt, vsIndDesc, vsListCmpr, cantArt, cantCmpr);
  EmitirArt_x_Rubro(vsArt, vsRub, cantArt);
  Cerrar(Art, IndDesc, Rub, ListCmpr);
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
int BusBinVec(tvsIndDesc &vsIndDesc, str30 &descArt, ushort ult) {
  int li = 0, ls = ult, pm;

  while (li <= ls) {
    pm = (li + ls) / 2;

    int cmp = strcmp(descArt, vsIndDesc[pm].descArt);

    if (cmp == 0) {
      if (vsIndDesc[pm].estado) {
        return pm;  // Encontrado y activo
      } else {
        return -1;  // Encontrado pero inactivo
      }
    } else if (cmp < 0) {
      ls = pm - 1;
    } else {
      li = pm + 1;
    }
  }

  return -1;  // No encontrado
}


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

bool LeerArticulo(fstream &Art, tsArt &sArt) {
  Art >> sArt.codVen >> sArt.codRub;
  Art.get(sArt.descArt, 31);
  Art >> sArt.stock >> sArt.preUni;
  Art.get(sArt.medida, 11);
  for (short i = 0; i < 14; i++)
    Art >> sArt.ofertas[i];
  Art.ignore();
  return Art.good();
}  // LeerArticulo

bool LeerDescripcion(ifstream &IndDesc, tsIndDesc &sIndDesc) {
  IndDesc.get(sIndDesc.descArt, 31);
  IndDesc >> sIndDesc.posArt >> sIndDesc.estado;
  IndDesc.ignore();
  return IndDesc.good();
}  // LeerDescripcion

bool LeerRubro(ifstream &Rub, tsRub &sRub) {
  Rub >> sRub.codRub;
  Rub.get(sRub.descRub, 21);
  Rub.ignore();
  return Rub.good();
}  // LeerRubro

bool LeerCompra(ifstream &ListCmpr, tsCompra &sCompra) {
  ListCmpr.get(sCompra.descArt, 31);
  ListCmpr >> sCompra.cantReq;
  ListCmpr.ignore();
  return ListCmpr.good();
}  // LeerCompra

void PieTicket(float impTot, float impTotDesto, float impTotConDesto) {
  float pagoUsuario = 1500.00;

  float vuelto = pagoUsuario - impTotConDesto;

  cout << fixed << setprecision(2);
  cout << Replicate('-', 40) << endl;
  cout << left << setw(28) << "Total bruto:" << "$ " << setw(9) << impTot
       << endl;
  cout << left << setw(28) << "Descuento aplicado:" << "$ " << setw(9)
       << impTotDesto << endl;
  cout << left << setw(28) << "Total a pagar:" << "$ " << setw(9)
       << impTotConDesto << endl;
  cout << left << setw(28) << "Su pago con Tipo Pago:" << "$ " << setw(9)
       << pagoUsuario << endl;
  cout << left << setw(28) << "Su vuelto:" << "$ " << setw(9) << vuelto << endl;
  cout << endl;
  cout << "         G R A C I A S  P O R  S U  C O M P R A" << endl;
  cout << "Para consultas, sugerencias o reclamos" << endl;
  cout << "comunicarse al correo infoKotto.com.ar" << endl;
  cout << Replicate('-', 40) << endl;
}  // PieTicket

void OrdxBur(tvsArt &vsArt, ushort card) {
  bool hayCambios;
  ushort k = 0;

  do {
    hayCambios = false;
    k++;

    for (ushort i = 0; i < card - k; i++) {
      if (strcmp(vsArt[i].descArt, vsArt[i + 1].descArt) > 0) {
        IntCmb(vsArt[i], vsArt[i + 1]);
        hayCambios = true;
      }
    }
  } while (hayCambios);

}  // OrdxBur

void IntCmb(tsArt &sElem1, tsArt &sElem2) {
  tsArt auxiliar = sElem1;
  sElem1 = sElem2;
  sElem2 = auxiliar;
}  // IntCmb

string Replicate(char car, ushort n) {
  string resultado = "";
  for (ushort i = 0; i < n; i++)
    resultado += car;
  return resultado;
}  // Replicate

void Abrir(ARCHIVOS) {
  Art.open("Articulos.txt", ios::in | ios::out);
  IndDesc.open("IndDescripArt.txt");
  Rub.open("Rubros.txt");
  ListCmpr.open("ListaCompras.txt");
}  // Abrir
void ProcCompras(ARCHIVO_ART, REG_COMPRAS, ushort cantArt, ushort cantCmpr) {
  str30 descBuscada;
  int pos;
  ushort posArt;
  short cantSolicitada, cantDisponible, cantComprada;

  for (ushort i = 0; i < cantCmpr; i++) {
    strcpy(descBuscada, vsListCmpr[i].descArt);
    pos = BusBinVec(vsIndDesc, descBuscada, cantArt - 1);

    if (pos != -1 && vsIndDesc[pos].estado) {
      posArt = vsIndDesc[pos].posArt;
      cantSolicitada = vsListCmpr[i].cantReq;
      cantDisponible = vsArt[posArt].stock;

      if (cantDisponible >= cantSolicitada) {
        vsArt[posArt].stock -= cantSolicitada;
        cantComprada = cantSolicitada;
      } else {
        cantComprada = cantDisponible;
        vsArt[posArt].stock = 0;
      }

      cout << left << setw(30) << vsArt[posArt].descArt
           << " Comprado: " << cantComprada << " "
           << vsArt[posArt].medida << endl;

      ActLinea(Art, vsArt[posArt]);

    } else {
      cout << left << setw(30) << vsListCmpr[i].descArt
           << " Comprado: 0 (No disponible)" << endl;
    }
  }
}

void VolcarArchivos(ARCHIVOS, REGISTROS, ushort &cantArt, ushort &cantCmpr) {
  tsArt sArt;
  tsIndDesc sIndDesc;
  tsRub sRub;
  tsCompra sCompra;
  cantArt = 0;
  cantCmpr = 0;

  while (LeerArticulo(Art, sArt)) {
    vsArt[cantArt] = sArt;
    cantArt++;
  }
  for (ushort i = 0; LeerDescripcion(IndDesc, sIndDesc); i++)
    vsIndDesc[i] = sIndDesc;
  for (ushort i = 0; LeerRubro(Rub, sRub); i++)
    vsRub[i] = sRub;
  while (LeerCompra(ListCmpr, sCompra)) {
    vsListCmpr[cantCmpr] = sCompra;
    cantCmpr++;
  }
}  // VolcarArchivos

void Cerrar(ARCHIVOS) {
  Art.close();
  IndDesc.close();
  Rub.close();
  ListCmpr.close();
}  // Cerrar
void CabeceraTicket(int &ds) {
  int hh, mm, ss, year, mes, dia;
  GetTime(hh, mm, ss);
  GetDate(year, mes, dia, ds);

  const char* diasSemana[] = {
    "Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"
  };

  cout << Replicate('=', 40) << endl;
  cout << "        TICKET DE COMPRA KOTTO" << endl;
  cout << "Fecha: " << setfill('0') << setw(2) << dia << "/"
       << setw(2) << mes << "/" << year << "  "
       << "Hora: " << setw(2) << hh << ":" << setw(2) << mm << ":" << setw(2) << ss << endl;
  cout << "Día: " << diasSemana[(ds - 1) % 7] << endl;
  cout << Replicate('=', 40) << endl;
  cout << setfill(' ');
}

void EmitirTicket(tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsListCmp &vsListCmpr,
                  ushort cantArt, ushort cantCmpr) {
  int ds;
  CabeceraTicket(ds);
  // Por ahora solo la cabecera.
} 