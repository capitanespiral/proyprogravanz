#ifndef RANDOM__
#define RANDOM__

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "matriz.hh"
#include <string>
#include <fstream>

//agregar funcion que haga automatico lo del archivo, y una que muestre cositas con frecuencias (intervalitos)
using namespace std;
typedef unsigned int uint;

class rdom{
 public:
  rdom(uint=time(0));
  void upd(uint=time(0));
  int irand(int,int=1); //hasta donde y desde cuando
  double drand(double,double=0);//same pero con double
};

matriz<double> mat_rand(int,int,double,double=0);//filas columnas, max y min

matriz<double> distr(double (*f)(double),double,double,double,double,int=100);//funcion, min y max de esta, intervalo de x, cantidad de puntos

//agrega ruido a datos, recibo matriz de datos, cuanto ruido maximo,que fila o columna y bool si es fila o columna, por defecto columna.
matriz<double> noise(const matriz<double> &,double,int=1,bool=true);

//Crea matriz de columna x y columna y, con que funcion, desde donde hasta donde,cuantos datos.
matriz<double> dat_func1(double (*f)(double),double,double,int);

matriz<double> dat_func2(double (*f)(double,double),double ,double ,double );

//Devuelve matriz normalizada, recibe la a normalizar, el nuevo valor minimo y el maximo. (Puedo usar esta misma para desnormalizar) al final recibe columna a normalizar.
matriz<double> normaliza_col(const matriz<double> &,double,double,int);

//Que nomrmalice toda la matriz
matriz<double> normaliza_todo(const matriz<double> &,double,double);



#endif
