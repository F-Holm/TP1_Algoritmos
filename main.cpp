/*
NO USAR MEMORIA DINAMICA (new, delete, malloc, realloc, free)
Muchos comentarios van a ser eliminados en la entrega final
UTILIZAR LA NOMENCLATURA DE HUGO CUELLO

El acceso secuencial es rapidisimo si accedemos a cada compoennete para
  procesarlo en el mismo orden en el que estos fueron grabados

Articulos.txt tiene 103 caracteres por linea + CR y Lf => 105 caracteres por
  linea

Un archivo de texto se compone de lineas donde cada linea es de longitud
  variable y cada linea termina con una marca llamada fin de linea, ademas hay
  otra marca que indica fin de archivo

Tod0 el contenido de un archivo .txt es texto cuando se crea un archivo, siempre
  crear una copia

Entrega:
  Caratula
  Consigna
  Diagramas
  Muestras de datos
  Resultados
  Codigo

1° entrega: 06/08
2° entrega: 13/08
3° entrega: 17/09
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

struct tsArt {   // max 10.000 - desordenado
  int codVen;    // 8 digitos
  short codRub;  // 2 digitos
  str30 descArt;
  ushort stock;  // 4 digitos
  float preUni;  // 6,2
  str10 medida;
  short ofertas[14];  //= {tipo descuento; porcentaje; tipo; porcentaje...}
};  // Cada par de "ofertas" es de un dia especifico de la semana
// ofertas[0] = oferta_domingo

struct tsIndDesc {  // ordenado por descripcion
  str30 descArt;
  int posArt;  // 4 digitos (0 - 9999)
  bool estado;
};

struct tsRub {  // hay 15 - ordenado por codigo
  short codRub;
  str20 descRub;
};

struct tsCompra {  // desordenado - max 100
  str30 descArt;
  short cantReq;  // 2 digitos
};

const ushort MAX_ART = 1000;
const ushort CANT_RUB = 15;
const ushort MAX_COMPRAS = 100;
typedef tsArt tvsArt[MAX_ART];
typedef tsIndDesc tvsIndDesc[MAX_ART];
typedef tsRub tvsRub[CANT_RUB];
typedef tsCompra tvsListCmpr[MAX_COMPRAS];  // lista compras

#define ARCHIVOS \
  fstream &Art, ifstream &IndDesc, ifstream &Rub, ifstream &ListCmpr
#define REGISTROS \
  tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsRub &vsRub, tvsListCmpr &vsListCmpr
#define REG_COMPRAS \
  tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr

// Declaraciones de funciones
// Las que tienen comentario '// Falta' no estan definidas
// Puede que tengan que cambiar, eliminar o agregar algunos parametros
// Las declaraciones que tienen errores son porque todavia no definimos los
// parametros de esa funcion

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
void ActLinea(fstream &Art, tsArt &sArt);
int BusBinVec(tvsIndDesc &vsIndDesc, str30 &descArt, ushort ult);
string Replicate(char car, ushort n);
void Abrir(ARCHIVOS);
void VolcarArchivos(ARCHIVOS, REGISTROS, ushort &cantArt, ushort &cantCmpr);
void ProcCompras(fstream &Art, REG_COMPRAS, ushort cantArt, ushort cantCmpr);
void EmitirTicket(tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr,
                  ushort cantArt, ushort cantCmpr);
void EmitirArt_x_Rubro(tvsArt &vsArt, tvsRub &vsRub, ushort cantArt);
void Cerrar(ARCHIVOS);

int main() {
  tvsArt vsArt;
  tvsIndDesc vsIndDesc;
  tvsRub vsRub;
  tvsListCmpr vsListCmpr;
  fstream Art;
  ifstream IndDesc, Rub, ListCmpr;
  ushort cantArt, cantCmpr;

  Abrir(Art, IndDesc, Rub, ListCmpr);
  VolcarArchivos(Art, IndDesc, Rub, ListCmpr, vsArt, vsIndDesc, vsRub,
                 vsListCmpr, cantArt, cantCmpr);
  ProcCompras(Art, vsArt, vsIndDesc, vsListCmpr, cantArt, cantCmpr);
  // EmitirTicket(vsArt, vsIndDesc, vsListCmpr, cantArt, cantCmpr);
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

bool LeerDescripcion(ifstream &IndDesc, tsIndDesc &sIndDesc) {
  IndDesc.get(sIndDesc.descArt, 31);
  IndDesc >> sIndDesc.posArt >> sIndDesc.estado;
  IndDesc.ignore(2, '\n');
  return IndDesc.good();
}  // LeerDescripcion

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

void CabeceraTicket(int &ds) {  // MAL
  int hh, mm, ss, year, mes, dia;
  GetTime(hh, mm, ss);
  GetDate(year, mes, dia, ds);

  const char *diasSemana[] = {"Domingo", "Lunes",   "Martes", "Miercoles",
                              "Jueves",  "Viernes", "Sabado"};

  cout << Replicate('=', 40) << endl;
  cout << "        TICKET DE COMPRA KOTTO" << endl;
  cout << "Fecha: " << setfill('0') << setw(2) << dia << "/" << setw(2) << mes
       << "/" << year << "  " << "Hora: " << setw(2) << hh << ":" << setw(2)
       << mm << ":" << setw(2) << ss << endl;
  cout << "Dia: " << diasSemana[(ds - 1) % 7] << endl;
  cout << Replicate('=', 40) << endl;
  cout << setfill(' ');
}  // CabeceraTicket

void OrdxBur(tvsArt &vsArt, ushort card) {
  bool hayCambios;
  ushort k = 0;

  do {
    hayCambios = false;
    k++;

    for (ushort i = 0; i < card - k; i++) {
      if (vsArt[i].codRub > vsArt[i + 1].codRub) {
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

void ActLinea(fstream &Art, tsArt &sArt) {
  Art << setw(8) << sArt.codVen << ' ' << setw(2) << sArt.codRub << ' '
      << setw(30) << sArt.descArt << ' ' << setw(4) << sArt.stock << ' '
      << setw(9) << sArt.preUni << ' ' << setw(10) << sArt.medida;
  for (ushort j = 0; j < 7; j++)
    Art << ' ' << sArt.ofertas[2 * j] << ' ' << setw(2)
        << sArt.ofertas[2 * j + 1];
}  // ActLinea

int BusBinVec(tvsIndDesc &vsIndDesc, str30 &descArt, ushort ult) {
  int li = 0, ls = ult, pm;

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

  while (LeerArticulo(Art, sArt) && cantArt <= MAX_ART) {
    vsArt[cantArt] = sArt;
    cantArt++;
  }
  for (ushort i = 0; LeerDescripcion(IndDesc, sIndDesc) && i < cantArt; i++)
    vsIndDesc[i] = sIndDesc;
  for (ushort i = 0; LeerRubro(Rub, sRub) && i < CANT_RUB; i++)
    vsRub[i] = sRub;
  while (LeerCompra(ListCmpr, sCompra) && cantCmpr <= MAX_COMPRAS) {
    vsListCmpr[cantCmpr] = sCompra;
    cantCmpr++;
  }
}  // VolcarArchivos

void ProcCompras(fstream &Art, REG_COMPRAS, ushort cantArt, ushort cantCmpr) {
  str30 descBuscada;
  int pos;
  ushort posArt;
  Art << fixed << setprecision(2);

  for (ushort i = 0; i < cantCmpr; i++) {
    strcpy(descBuscada, vsListCmpr[i].descArt);
    pos = BusBinVec(vsIndDesc, descBuscada, cantArt - 1);

    if (pos != -1 && vsIndDesc[pos].estado) {
      posArt = vsIndDesc[pos].posArt;

      if (vsArt[posArt].stock >= vsListCmpr[i].cantReq) {
        vsArt[posArt].stock -= vsListCmpr[i].cantReq;
      } else {
        vsListCmpr[i].cantReq = vsArt[posArt].stock;
        vsArt[posArt].stock = 0;
      }
      Art.clear();
      Art.seekp(105 * posArt);
      ActLinea(Art, vsArt[posArt]);

    } else {
      vsListCmpr[i].cantReq = 0;
    }
  }
}  // ProcCompras

void EmitirTicket(tvsArt &vsArt, tvsIndDesc &vsIndDesc, tvsListCmpr &vsListCmpr,
                  ushort cantArt, ushort cantCmpr) {
  int ds;
  float impTot = 0.0f, impTotDesto = 0.0f;

  freopen("Ticket.txt", "w", stdout);
  CabeceraTicket(ds);  // ✔ Cabecera

  const char *tipoDesc[] = {"", "Jub.", "Marca.", "MercPago", "Comunid.", "ANSES", "Promo"};

  for (ushort i = 0; i < cantCmpr; i++) {
    if (vsListCmpr[i].cantReq == 0)
      continue;

    int pos = BusBinVec(vsIndDesc, vsListCmpr[i].descArt, cantArt - 1);
    if (pos == -1 || !vsIndDesc[pos].estado)
      continue;

    tsArt &art = vsArt[vsIndDesc[pos].posArt];
    ushort cant = vsListCmpr[i].cantReq;
    float precio = art.preUni;
    float subtotal = cant * precio;

    ushort tipo = art.ofertas[(ds - 1) * 2];
    ushort porc = art.ofertas[(ds - 1) * 2 + 1];
    float descuento = 0.0f;

    if (tipo >= 1 && tipo <= 6)  // Solo aplicar si es válido
      descuento = subtotal * porc / 100.0f;

    float total = subtotal - descuento;

    // Cuerpo del ticket (alineado)
    cout << right << setw(3) << cant << " x $ " << right << setw(8)
         << fixed << setprecision(2) << precio << '\n';

    cout << left << setw(30) << art.descArt << setw(10) << art.medida << '\n';

    cout << setw(8) << art.codVen << right << setw(42)
         << "$ " << setw(10) << fixed << setprecision(2) << subtotal << '\n';

    if (descuento > 0.0f) {
      cout << left << setw(12) << tipoDesc[tipo] << right << setw(5) << porc
           << right << setw(28)
           << "$ -" << setw(9) << fixed << setprecision(2) << descuento << '\n';
    }

    impTot += subtotal;
    impTotDesto += descuento;
  }

  float impTotConDesto = impTot - impTotDesto;

  cout << '\n' << left << setw(35) << "SubTot. sin descuentos....:"
       << "$ " << right << setw(10) << fixed << setprecision(2) << impTot << '\n';
  cout << left << setw(35) << "Descuentos por promociones:"
       << "$ -" << right << setw(9) << fixed << setprecision(2) << impTotDesto << '\n';
  cout << Replicate('=', 40) << '\n';
  cout << left << setw(28) << "T O T A L" << "$ " << right << setw(10) << impTotConDesto << '\n';
  cout << Replicate('=', 40) << '\n';

  // Pie del ticket
  PieTicket(impTot, impTotDesto, impTotConDesto);
  fclose(stdout);  // ✔ cerrar salida redirigida
}


void EmitirArt_x_Rubro(tvsArt &vsArt, tvsRub &vsRub, ushort cantArt) {
  OrdxBur(vsArt, cantArt);
  freopen("ListadoArticulos.txt", "w", stdout);
  cout << setprecision(2) << fixed;
  ushort codRubro = 200;

  cout << Replicate('-', 100) << '\n'
       << Replicate(' ', floor((100.0 - 50.0) / 2.0))
       << "Listado de Articulos ordenados por Codigo de Rubro"
       << Replicate(' ', ceil((100.0 - 50.0) / 2.0)) << '\n'
       << Replicate('=', 100) << '\n';
  for (ushort i = 0; i < cantArt; i++) {
    if (i != 0)
      cout << '\n';
    if (codRubro != vsArt[i].codRub) {
      codRubro = vsArt[i].codRub;
      cout << "\nCod. Rubro: " << codRubro << ' ' << vsRub[codRubro - 1].descRub
           << "\nCod.Art. Descripcion" << Replicate(' ', 20)
           << "Stk. Prec.Uni. Uni.Medida TD % TD % TD % TD % TD % TD % TD %\n"
           << Replicate('-', 100) << '\n';
    }
    cout << setw(8) << vsArt[i].codVen << ' ' << setw(30) << vsArt[i].descArt
         << ' ' << setw(4) << vsArt[i].stock << ' ' << setw(9)
         << vsArt[i].preUni << ' ' << setw(10) << vsArt[i].medida;
    for (ushort j = 0; j < 7; j++)
      cout << ' ' << vsArt[i].ofertas[2 * j] << ' ' << setw(2)
           << vsArt[i].ofertas[2 * j + 1];
  }
  fclose(stdout);
}  // EmitirArt_x_Rubro

void Cerrar(ARCHIVOS) {
  Art.close();
  IndDesc.close();
  Rub.close();
  ListCmpr.close();
}  // Cerrar
