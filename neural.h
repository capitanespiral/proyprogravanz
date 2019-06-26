#ifndef _NEURAL
#define _NEURAL
#include "matriz.h"
#include "random.h"
#include <iostream>

using namespace std;

//Hacer que escupa un archivo con la configuracion de la red y un constructor que pueda leer estos archivos.

class neural_l{//clase de capas neuronales
 private:
  int c_neu,inp;//cant de neuronas en capa y cant de inputs
  matriz<double> B;//vector columna de las cts
  matriz<double> W;//pesos
  double (*act)(double);//func activacion
  double (*dact)(double);//derivada func activacion
 public:
  neural_l();
  neural_l(int,int,double (*f)(double),double (*g)(double));//Crea capa, con inputs y cantidad de celulas (que sera el output dah,y la funcion de activacion con su derivada
  neural_l operator=(const neural_l &);
  ostream & print(ostream &) const;
  matriz<double> operator[](int) const ;
  matriz<double> & operator[](int);
  matriz<double> operator()(const matriz<double> &,int=0);//para usar act o dact
  int axon(int);
  matriz<neural_l> operator+(neural_l);
};

ostream & operator<<(ostream &,const neural_l &);

matriz<neural_l> red(const matriz<int> &,double (*f)(double),double (*g)(double)); //Crea red, recibe vector columna donde cada dimension es capa (excepto la primera que son valores de input) asi que size -1 seran las capas y cada numero cantidad de neuronas.Rcda que primer y ultimo valor es input y output. (no se crea la capa de "input")
matriz<double> forward(const matriz<neural_l> &,const matriz<double> &); //red, valores de entrada. escupe valores de salida.Cada fila un dato, cada columna componente de cada dato.
matriz<double> train(matriz<neural_l> &,const matriz<double> &,const matriz<double> &,matriz<double> (*derror)(const matriz<double> &,const matriz<double> &),double=0.5);//red,input, output esperados, derivada de error,learning rate. (el error se calculará por fuera, pues esto vomita el resultado)

//funcion sigmoide
double sigm(double);

//Su derivada
double dsigm(double);

//error cuad medio
double e_cuad_m(const matriz<double> &,const matriz<double> &);

//su derivada

matriz<double> d_e_cuad_m(const matriz<double> &,const matriz<double> &);

#endif
