#ifndef _NEURAL
#define _NEURAL
#include "matriz.hh"
#include "random.hh"
#include <iostream>
#include <algorithm>
#include <string>
#include <dirent.h>

/////////PROYECTO DE PROGRA///////////

using namespace std;

class neural_l{//clase de capas neuronales
 private:
  int c_neu,inp;//cant de neuronas en capa y cant de inputs
  matriz<double> ctes;//vector columna de las cts
  matriz<double> pesos;//matriz de pesos
  double (*act)(double);//func activacion
  double (*dact)(double);//derivada func activacion
 public:
  neural_l();
  neural_l(int,int,double (*f)(double),double (*g)(double));//Crea capa, con inputs y cantidad de celulas (que sera el output de esta capa),y la funcion de activacion con su derivada
  neural_l(const matriz<double> &,const matriz<double>&,double (*f)(double),double (*g)(double));//Constructor recibiendo matriz de ctes, de pesos y funcion de activacion con su derivada (hecho para leer redes prehechas)
  neural_l operator=(const neural_l &);
  //Para imprimir
  ostream & print(ostream &) const;
  //Para entregar ctes o pesos, (o reemplazarlo)
  matriz<double> operator[](int) const ;
  matriz<double> & operator[](int);
  matriz<double> operator()(const matriz<double> &,int=0);//para usar act o dact sobre la matriz entregada
  //Axon para saber numero de entradas y outputs (neuronas capa anterior (o input original para primera capa) y cant de neuronas en la misma capa)
  int axon(int);
  //Suma de capas neuronales (las pega)
  matriz<neural_l> operator+(neural_l);
};

//Para imprimir
ostream & operator<<(ostream &,const neural_l &);

matriz<neural_l> red(const matriz<int> &,double (*f)(double),double (*g)(double)); //Crea red, recibe vector columna donde cada dimension es capa (excepto la primera que son valores de input) asi que size -1 seran las capas y cada numero cantidad de neuronas.Rcda que primer y ultimo valor es input y output. (no se crea la capa de "input"). Recibe tambien la funcion de activacion y la derivada.

matriz<double> forward(const matriz<neural_l> &,const matriz<double> &); //Usa la red, recibe esta, valores de entrada. escupe valores de salida.Cada fila un dato, cada columna componente de cada dato.

//Entrena la red, un recorrido forward y un backward
matriz<double> train(matriz<neural_l> &,const matriz<double> &,const matriz<double> &,matriz<double> (*derror)(const matriz<double> &,const matriz<double> &),double);//red (por referencia para modificarla),input, output esperados, derivada de error,learning rate. (el error se calculará por fuera, pues esto vomita el resultado de un forward y un backward)

//funcion sigmoide (activacion)
double sigm(double);

//Su derivada
double dsigm(double);

//error cuad medio (para calcular error)
double e_cuad_m(const matriz<double> &,const matriz<double> &);

//su derivada, pensada para calcular con toodos los resultados obtenidos en un iteracion
matriz<double> d_e_cuad_m(const matriz<double> &,const matriz<double> &);

//Crea red a partir del archivo string, asignando f y g omo act y dact.
matriz<neural_l> red_prehecha(string,double (*f)(double),double (*g)(double));

//Guardo red en archivo llamado string.
void guarda_red(string,const matriz<neural_l> &);

//Matriz de dos columnas guardando puntos de un cuadrado desde min a max avanzando a paso.(Para la imagenes de convergencia)
matriz<double> matriz_cuad(double,double,double);

//Para saber la pot de 10 de un entero
int pot_10(int);

//Crea la carpeta con el nombre stringnumero sin sobreescribir otras carpetas y devuelve el nombre de la carpeta creada (por eso paso int por referencia, asi ese será necesariamente el que logró usar)
string crea_carpeta(string,int &);

//Funcion que encapsula mucho de los trabajos, es la central que hace el trabajo, pues guarda archivos de error y entrena la red hasta cierto punto, tambien permite "checkear" la evolucion de la red (proband la red en una matriz cuadrada). Además normaliza si los valores esperados (y) se escapan de lo permitido por la func de activacion
void entrena_guarda(matriz<neural_l> &,const matriz<double> &,const matriz<double> &,double,string ,int ,bool ,int,char);

//Desordena los datos preservandolos sus respectivos outputs esperados
void revuelve(matriz<double> &,matriz<double> &);

//hace una particion de las primeras dos matrices en las segundas dos segun el porcentaje indicado
void separa(matriz<double> &,matriz<double> &,matriz<double> &,matriz<double> &,int=40);

//Equivalente a entrena_guarda pero para cuando quiero estudiar el posible overfitting también
void entrena_guarda_overf(matriz<neural_l> &,const matriz<double> &x,const matriz<double> &,const matriz<double> &,const matriz<double> &,double,string,int,bool,int,char);


#endif
