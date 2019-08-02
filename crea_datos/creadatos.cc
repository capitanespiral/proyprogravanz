#include "random.hh"

using namespace std;
//Archivo para crear datos y probar las redes

double paraboloide(double,double);

int main(int argc,char* argv[]){
  matriz<double> parabol=dat_func2(paraboloide,-5.,5.,0.5);
  //Imprimo cuantos datos terminaron siendo
  cout<<parabol.fila()<<endl;
  matriz<double> ys;
  //Divido en inputs y outputs la funcion
  matriz<double> xs=split(parabol,ys,1);
  env_archivo("paraboloide_x.dat",xs);
  env_archivo("paraboloide_y.dat",ys);
  return 0;
}

double paraboloide(double a,double b){
  return a*a+b*b;
}
