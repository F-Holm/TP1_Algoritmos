/*
 - Nombre  del  programa: TP1V2_K1021G10_HOLM FEDERICO.cpp
 - fecha  entrega: 03/09/2025
 - Nro. versión: 2
 - Breve comentario del objetivo del programa:
   Este programa gestiona un sistema de ventas de artículos, permitiendo
   registrar artículos, rubros, descripciones y compras. Genera tickets
   de compra y listados de artículos por rubro, aplicando descuentos según
   promociones. Utiliza archivos para almacenar y recuperar datos.


 - Curso: Algoritmos y Estructuras de Datos
 - Comision: K1021
 - Turno: Mañana
 - Docente: Lic. Hugo A. Cuello

 Integrantes (Apellido, Nombre):
    Almada, Tomas
    Baquero, Francisco
    Barcala Roca, Santiago
    Cejas, Facundo Javier
    Dominguez, Joaquin Ezequiel
    Holm, Federico
    Incutti, Mateo
    Mampaso Romero, Brayan
    Soria, Francisco

 - Nombre del compilador: Borland C++ V.5.5
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

struct tsArt {
  int codVen;
  short codRub;
  str30 descArt;
  ushort stock;
  float preUni;
  str10 medida;
  short ofertas[14];
};

struct tsIndDesc {
  str30 descArt;
  int posArt;
  bool estado;
};

struct tsRub {
  short codRub;
  str20 descRub;
};

struct tsCompra {
  str30 descArt;
  short cantReq;
};

struct tsArtRub {
  short codRub;
  int posArt;
};

const ushort MAX_ART = 10000;
const ushort CANT_RUB = 15;
const ushort MAX_COMPRAS = 100;
typedef tsArtRub tvsArtRub[MAX_ART];
typedef tsIndDesc tvsIndDesc[MAX_ART];
typedef tsRub tvsRub[CANT_RUB];
typedef tsCompra tvsListCmpr[MAX_COMPRAS];

#define ARCHIVOS \
  fstream &Art, ifstream &IndDesc, ifstream &Rub, ifstream &ListCmpr
#define REGISTROS                                            \
  tvsArtRub &vsArtRub, tvsIndDesc &vsIndDesc, tvsRub &vsRub, \
      tvsListCmpr &vsListCmpr

long GetTime(int &hh, int &mm, int &ss);
long GetDate(int &year, int &mes, int &dia, int &ds);
bool LeerArticulo(fstream &Art, tsArt &sArt);
bool LeerIndDescrip(ifstream &IndDesc, tsIndDesc &sIndDesc);
bool LeerRubro(ifstream &Rub, tsRub &sRub);
bool LeerCompra(ifstream &ListCmpr, tsCompra &sCompra);
void PieTicket(float impTot, float impTotDesto, float impTotConDesto);
void CabeceraTicket(int &ds);
void OrdxBur(tvsArtRub &vsArt, ushort card);
void IntCmb(tsArtRub &sElem1, tsArtRub &sElem2);
void ActLinea(fstream &Art, tsArt &sArt);
int BusBinVec(tvsIndDesc &vsIndDesc, str30 descArt, ushort ult);
string Replicate(char car, ushort n);
void Abrir(ARCHIVOS);
void VolcarArchivos(ARCHIVOS, REGISTROS, ushort &cantArt, ushort &cantCmpr);
void ProcCompras(fstream &Art, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr,
                 ushort cantArt, ushort cantCmpr);
void EmitirTicket(fstream &Art, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr,
                  ushort cantArt, ushort cantCmpr);
void EmitirArt_x_Rubro(fstream &Art, tvsArtRub &vsArtRub, tvsRub &vsRub,
                       ushort cantArt);
void Cerrar(ARCHIVOS);

int main() {
  tvsArtRub vsArtRub;
  tvsIndDesc vsIndDesc;
  tvsRub vsRub;
  tvsListCmpr vsListCmpr;
  fstream Art;
  ifstream IndDesc, Rub, ListCmpr;
  ushort cantArt, cantCmpr;

  Abrir(Art, IndDesc, Rub, ListCmpr);
  VolcarArchivos(Art, IndDesc, Rub, ListCmpr, vsArtRub, vsIndDesc, vsRub,
                 vsListCmpr, cantArt, cantCmpr);
  ProcCompras(Art, vsIndDesc, vsListCmpr, cantArt, cantCmpr);
  EmitirTicket(Art, vsIndDesc, vsListCmpr, cantArt, cantCmpr);
  EmitirArt_x_Rubro(Art, vsArtRub, vsRub, cantArt);
  Cerrar(Art, IndDesc, Rub, ListCmpr);
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

bool LeerArticulo(fstream &Art, tsArt &sArt) {
  Art >> sArt.codVen >> sArt.codRub;
  Art.ignore();
  Art.get(sArt.descArt, 31);
  Art >> sArt.stock >> sArt.preUni;
  Art.ignore();
  Art.get(sArt.medida, 11);
  for (short i = 0; i < 14; i++)
    Art >> sArt.ofertas[i];
  Art.ignore(2, '\n');
  return Art.good();
}  // LeerArticulo

bool LeerIndDescrip(ifstream &IndDesc, tsIndDesc &sIndDesc) {
  IndDesc.get(sIndDesc.descArt, 31);
  IndDesc >> sIndDesc.posArt >> sIndDesc.estado;
  IndDesc.ignore(2, '\n');
  strlwr(sIndDesc.descArt);
  return IndDesc.good();
}  // LeerIndDescrip

bool LeerRubro(ifstream &Rub, tsRub &sRub) {
  Rub >> sRub.codRub;
  Rub.ignore();
  Rub.get(sRub.descRub, 21);
  Rub.ignore(2, '\n');
  return Rub.good();
}  // LeerRubro

bool LeerCompra(ifstream &ListCmpr, tsCompra &sCompra) {
  ListCmpr.get(sCompra.descArt, 31);
  ListCmpr >> sCompra.cantReq;
  ListCmpr.ignore(2, '\n');
  return ListCmpr.good();
}  // LeerCompra

void PieTicket(float impTot, float impTotDesto, float impTotConDesto) {
  float pagoUsuario = impTotConDesto;  // El comprador paga exacto

  float vuelto = pagoUsuario - impTotConDesto;

  cout << setw(42) << left << "SubTot. sin descuentos....:" << "$ " << setw(10)
       << right << impTot << '\n'
       << setw(42) << left << "Descuentos por promociones:" << "$ " << setw(10)
       << right << -impTotDesto << '\n'
       << Replicate('=', 54) << '\n'
       << setw(42) << left << "T O T A L" << "$ " << setw(10) << right
       << impTotConDesto << '\n'
       << Replicate('=', 54) << '\n'
       << setw(42) << left << "Su pago con Mercado Pago:" << "$ " << setw(10)
       << right << pagoUsuario << endl
       << setw(42) << left << "Su vuelto:" << "$ " << setw(10) << right
       << vuelto << '\n'
       << Replicate(' ', 8) << "G R A C I A S  P O R  S U  C O M P R A\n"
       << Replicate(' ', 8) << "Para consultas, sugerencias o reclamos\n"
       << Replicate(' ', 8) << "comunicarse al correo infoKotto.com.ar";
}  // PieTicket

void CabeceraTicket(int &ds) {
  int hh, mm, ss, anio, mes, dia;
  GetTime(hh, mm, ss);
  GetDate(anio, mes, dia, ds);

  const char *diasSemana[] = {"Domingo", "Lunes",   "Martes", "Miercoles",
                              "Jueves",  "Viernes", "Sabado"};

  cout << "K O T T O\n"
       << "Yo te reconozco\n"
       << "SUC 170\n"
       << "XXXXXX...X 9999\n"
       << "XX...X\n"
       << "C.U.I.T. 99-99999999-9\n"
       << setfill('0') << "Fecha: " << diasSemana[ds - 1] << " " << setw(2)
       << right << dia << "/" << setw(2) << mes << "/" << setw(4) << anio
       << "\nHora: " << setw(2) << hh << ":" << setw(2) << mm << ":" << setw(2)
       << ss << "\nNro. Ticket: 9999-99999999\n"
       << "Nro. Caja: 9999\n"
       << Replicate('-', 54) << "\nF A C T U R A - B\n"
       << "ORIGINAL\n"
       << Replicate('-', 54) << endl;

}  // CabeceraTicket

void OrdxBur(tvsArtRub &vsArtRub, ushort card) {
  bool hayCambios;
  ushort k = 0;

  do {
    hayCambios = false;
    k++;

    for (ushort i = 0; i < card - k; i++) {
      if (vsArtRub[i].codRub > vsArtRub[i + 1].codRub) {
        IntCmb(vsArtRub[i], vsArtRub[i + 1]);
        hayCambios = true;
      }
    }
  } while (hayCambios);
}  // OrdxBur

void IntCmb(tsArtRub &sElem1, tsArtRub &sElem2) {
  tsArtRub auxiliar = sElem1;
  sElem1 = sElem2;
  sElem2 = auxiliar;
}  // IntCmb

void ActLinea(fstream &Art, tsArt &sArt) {
  Art << setw(8) << sArt.codVen << ' ' << setw(2) << sArt.codRub << ' '
      << setw(30) << sArt.descArt << ' ' << setw(4) << sArt.stock << ' '
      << setw(9) << sArt.preUni << ' ' << setw(10) << sArt.medida;
  for (ushort j = 0; j < 7; j++)
    Art << ' ' << sArt.ofertas[2 * j] << ' ' << setw(2)
        << sArt.ofertas[2 * j + 1];
}  // ActLinea

int BusBinVec(tvsIndDesc &vsIndDesc, str30 descArt, ushort ult) {
  int li = 0, ls = ult, pm;

  strlwr(descArt);
 
  while (li <= ls) {
    pm = (li + ls) / 2;

    int cmp = strcmp(descArt, vsIndDesc[pm].descArt);

    if (cmp == 0) {
      return pm;
    } else if (cmp < 0) {
      ls = pm - 1;
    } else {
      li = pm + 1;
    }
  }

  return -1;  // No encontrado
}  // BusBinVec

string Replicate(char car, ushort n) {
  string resultado = "";
  for (ushort i = 0; i < n; i++)
    resultado += car;
  return resultado;
}  // Replicate

void Abrir(ARCHIVOS) {
  Art.open("Articulos.txt");
  IndDesc.open("IndDescripArt.txt");
  Rub.open("Rubros.txt");
  ListCmpr.open("ListaCompras.txt");
}  // Abrir

void VolcarArchivos(ARCHIVOS, REGISTROS, ushort &cantArt, ushort &cantCmpr) {
  tsArt sArt;
  tsIndDesc sIndDesc;
  tsRub sRub;
  tsCompra sCompra;
  cantArt = 0;
  cantCmpr = 0;

  while (LeerArticulo(Art, sArt) && cantArt < MAX_ART) {
    vsArtRub[cantArt].codRub = sArt.codRub;
    vsArtRub[cantArt].posArt = cantArt;
    cantArt++;
  }
  for (ushort i = 0; LeerIndDescrip(IndDesc, sIndDesc) && i < cantArt; i++)
    vsIndDesc[i] = sIndDesc;
  for (ushort i = 0; LeerRubro(Rub, sRub) && i < CANT_RUB; i++)
    vsRub[i] = sRub;
  while (LeerCompra(ListCmpr, sCompra) && cantCmpr < MAX_COMPRAS) {
    vsListCmpr[cantCmpr] = sCompra;
    cantCmpr++;
  }

  OrdxBur(vsArtRub, cantArt);
}  // VolcarArchivos

void ProcCompras(fstream &Art, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr,
                 ushort cantArt, ushort cantCmpr) {
  str30 descBuscada;
  int pos;
  ushort posArt;
  tsArt sArt;
  Art << fixed << setprecision(2);

  for (ushort i = 0; i < cantCmpr; i++) {
    strcpy(descBuscada, vsListCmpr[i].descArt);
    pos = BusBinVec(vsIndDesc, descBuscada, cantArt - 1);

    if (pos != -1 && vsIndDesc[pos].estado) {
      posArt = vsIndDesc[pos].posArt;

      Art.clear();
      Art.seekp(105 * posArt);
      LeerArticulo(Art, sArt);

      if (sArt.stock >= vsListCmpr[i].cantReq) {
        sArt.stock -= vsListCmpr[i].cantReq;
      } else {
        vsListCmpr[i].cantReq = sArt.stock;
        sArt.stock = 0;
      }
      Art.clear();
      Art.seekp(105 * posArt);
      ActLinea(Art, sArt);

    } else {
      vsListCmpr[i].cantReq = 0;
    }
  }
}  // ProcCompras

void EmitirTicket(fstream &Art, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr,
                  ushort cantArt, ushort cantCmpr) {
  int ds;
  float impTot = 0.0f, impTotDesto = 0.0f;
  tsArt sArt;

  freopen("Ticket.txt", "w", stdout);
  CabeceraTicket(ds);
  cout << fixed << setprecision(2) << setfill(' ');

  for (ushort i = 0; i < cantCmpr; i++) {
    if (vsListCmpr[i].cantReq > 0) {
      int pos = BusBinVec(vsIndDesc, vsListCmpr[i].descArt, cantArt - 1);
      if (pos > -1) {
        Art.clear();
        Art.seekp(105 * vsIndDesc[pos].posArt);
        LeerArticulo(Art, sArt);

        ushort cant = vsListCmpr[i].cantReq;
        float precio = sArt.preUni;
        float subtotal = cant * precio;

        ushort tipo = sArt.ofertas[(ds - 1) * 2];
        ushort porc = sArt.ofertas[(ds - 1) * 2 + 1];
        float descuento = 0.0f;
        str10 strDesc;

        if (tipo >= 2 && tipo <= 7)  // Solo aplicar si es válido
          descuento = subtotal * porc / 100.0f;

        switch (tipo) {
          case 2:
            strcpy(strDesc, "Promo");
            break;
          case 3:
            strcpy(strDesc, "Marca");
            break;
          case 4:
            strcpy(strDesc, "Jub.");
            break;
          case 5:
            strcpy(strDesc, "Comunid.");
            break;
          case 6:
            strcpy(strDesc, "MercPago");
            break;
          case 7:
            strcpy(strDesc, "ANSES");
            break;
          default: // case 0:
            strcpy(strDesc, "SinPromo");
            break;
        }

        cout << setw(2) << right << cant << " x $ " << setw(9) << precio << '\n'
             << setw(30) << left << sArt.descArt << ' ' << setw(10)
             << sArt.medida << '\n'
             << setw(8) << right << sArt.codVen << setw(36) << "$ " << setw(10)
             << subtotal << '\n';

        if (descuento > 0.0f) {
          cout << setw(12) << left << strDesc << setw(5) << right << porc
               << setw(27) << "$ " << setw(10) << -descuento << '\n';
        }

        impTot += subtotal;
        impTotDesto += descuento;
        cout << '\n';
      }
    }
  }

  float impTotConDesto = impTot - impTotDesto;

  PieTicket(impTot, impTotDesto, impTotConDesto);
}

void EmitirArt_x_Rubro(fstream &Art, tvsArtRub &vsArtRub, tvsRub &vsRub,
                       ushort cantArt) {
  cout << setfill(' ') << setprecision(2) << fixed;
  ushort codRubro = 200;
  short posRubro = -1;
  tsArt sArt;

  cout << Replicate('\n', 10) << '\n'  // Separa el Ticket del Listado
       << Replicate('-', 100) << '\n'
       << Replicate(' ', floor((100.0 - 50.0) / 2.0))
       << "Listado de Articulos ordenados por Codigo de Rubro"
       << Replicate(' ', ceil((100.0 - 50.0) / 2.0)) << '\n'
       << Replicate('=', 100) << '\n';
  for (ushort i = 0; i < cantArt; i++) {
    Art.clear();
    Art.seekp(105 * vsArtRub[i].posArt);
    LeerArticulo(Art, sArt);
    if (i != 0)
      cout << '\n';
    if (codRubro != sArt.codRub) {
      codRubro = sArt.codRub;
      do {
        posRubro++;
      } while (posRubro < 15 && codRubro != vsRub[posRubro].codRub);

      cout << "\nCod. Rubro: " << codRubro << ' ' << vsRub[posRubro].descRub
           << "\nCod.Art. Descripcion" << Replicate(' ', 20)
           << "Stk. Prec.Uni. Uni.Medida TD % TD % TD % TD % TD % TD % TD %\n"
           << Replicate('-', 100) << '\n';
    }
    cout << setw(8) << right << sArt.codVen << ' ' << setw(30) << left
         << sArt.descArt << ' ' << setw(4) << right << sArt.stock << ' '
         << setw(9) << right << sArt.preUni << ' ' << setw(10) << left
         << sArt.medida;
    for (ushort j = 0; j < 7; j++)
      cout << ' ' << sArt.ofertas[2 * j] << ' ' << setw(2) << right
           << sArt.ofertas[2 * j + 1];
  }
  fclose(stdout);
}  // EmitirArt_x_Rubro

void Cerrar(ARCHIVOS) {
  Art.close();
  IndDesc.close();
  Rub.close();
  ListCmpr.close();
}  // Cerrar
