#ifndef _NEURAL
#define _NEURAL
#include "matriz.hh"
#include "random.hh"
#include <iostream>
#include <algorithm>
#include <string>
#include <dirent.h>

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
  neural_l(const matriz<double> &,const matriz<double>&,double (*f)(double),double (*g)(double));
  neural_l operator=(const neural_l &);
  ostream & print(ostream &) const;
  //Para entregar B o W, (o reemplazarlo)
  matriz<double> operator[](int) const ;
  matriz<double> & operator[](int);
  matriz<double> operator()(const matriz<double> &,int=0);//para usar act o dact
  //Axon para saber numero de entradas y outputs
  int axon(int);
  matriz<neural_l> operator+(neural_l);
};

ostream & operator<<(ostream &,const neural_l &);

matriz<neural_l> red(const matriz<int> &,double (*f)(double),double (*g)(double)); //Crea red, recibe vector columna donde cada dimension es capa (excepto la primera que son valores de input) asi que size -1 seran las capas y cada numero cantidad de neuronas.Rcda que primer y ultimo valor es input y output. (no se crea la capa de "input"). Recibe tambien la funcion de activacion y la derivada.

matriz<double> forward(const matriz<neural_l> &,const matriz<double> &); //red, valores de entrada. escupe valores de salida.Cada fila un dato, cada columna componente de cada dato.

matriz<double> train(matriz<neural_l> &,const matriz<double> &,const matriz<double> &,matriz<double> (*derror)(const matriz<double> &,const matriz<double> &),double);//red,input, output esperados, derivada de error,learning rate. (el error se calculará por fuera, pues esto vomita el resultado)

//funcion sigmoide
double sigm(double);

//Su derivada
double dsigm(double);

//error cuad medio
double e_cuad_m(const matriz<double> &,const matriz<double> &);

//su derivada

matriz<double> d_e_cuad_m(const matriz<double> &,const matriz<double> &);

//Desordena los datos preservandolos (y sus respectivos outputs esperados)
void revuelve(matriz<double> &,matriz<double> &);

//hace una particion de las primeras dos matrices en las segundas dos segun el porcentaje indicado
void separa(matriz<double> &,matriz<double> &,matriz<double> &,matriz<double> &,int=40);

//Crea la carpeta con el nombre stringnumero sin sobreescribir otras carpetas y devuelve el nombre de la carpeta creada (por eso paso int por referencia, asi ese será necesariamente el que logró usar)
string crea_carpeta(string,int &);

//Crea red a partir del archivo string, asignando f y g omo act y dact.
matriz<neural_l> red_prehecha(string,double (*f)(double),double (*g)(double));

//Guardo red en archivo llamado string.
void guarda_red(string,const matriz<neural_l> &);

//Devuelve matriz normalizada, recibe la a normalizar, el nuevo valor minimo y el maximo. (Puedo usar esta misma para desnormalizar) al final recibe columna a normalizar.
matriz<double> normaliza_col(const matriz<double> &,double,double,int);

//Matriz de dos columnas guardando puntos de un cuadrado desde min a max avanzando a paso.(Para la imagenes de convergencia)
matriz<double> matriz_cuad(double,double,double);

void entrena_guarda(matriz<neural_l> &,const matriz<double> &,const matriz<double> &,double,string ,int ,bool = true,int = 25);

void entrena_guarda_overf(matriz<neural_l> &,const matriz<double> &x,const matriz<double> &,const matriz<double> &,const matriz<double> &,double,string,int,bool,int);
#endif
