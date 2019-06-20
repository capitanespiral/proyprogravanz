#ifndef _NEURAL
#define _NEURAL
#include "matriz.h"
#include "random.h"
#include <iostream>

using namespace std;

class neural_l{//clase de capas neuronales
 private:
  int c_neu,inp;//cant de neuronas en capa y cant de inputs
  matriz<double> B;//vector columna de las cts
  matriz<double> W;//pesos
 public:
  neural_l();
  neural_l(int,int);//Crea capa, con inputs y cantidad de celulas (que sera el output dah,y la funcion de activacion
  neural_l operator=(const neural_l &);
  ostream & print(ostream &) const;
  int cant_n();
  matriz<double> operator[](int) const ;
  matriz<double> & operator[](int);
};

ostream & operator<<(ostream &,const neural_l &);

matriz<neural_l> red(const matriz<int> &); //Crea red, recibe vector columna donde cada dimension es capa (excepto la primera que son valores de input) asi que size -1 seran las capas y cada numero cantidad de neuronas.Rcda que primer y ultimo valor es input y output. (no se crea la capa de "input")
matriz<double> forward(const matriz<neural_l> &,const matriz<double> &,double (*f)(double)); //red, valores de entrada, funcion de activacion. escupe valores de salida.Cada fila un dato, cada columna componente de cada dato.
#endif
