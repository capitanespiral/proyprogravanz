#ifndef RANDOM__
#define RANDOM__

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "matriz.h"
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

/*matriz<double> distr(double (*f)(double),double,double,double,double,char,int=100);

//para histograma
matriz<double> distr(double (*f)(double),double,double,double,double,int=10,int=100);*/

#endif
