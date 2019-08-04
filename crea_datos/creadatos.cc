#include "random.hh"

using namespace std;
//Archivo para crear datos y probar las redes

double paraboloide(double,double);
double iter_bessel_dec(double ,int ,int =-1);
double bessel_2d(double,double);
  

int main(int argc,char* argv[]){
  /*matriz<double> parabol=dat_func2(paraboloide,-5.,5.,0.5);
  //Imprimo cuantos datos terminaron siendo
  cout<<parabol.fila()<<endl;
  matriz<double> ys;
  //Divido en inputs y outputs la funcion
  matriz<double> xs=split(parabol,ys,1);
  env_archivo("paraboloide_x.dat",xs);
  env_archivo("paraboloide_y.dat",ys);*/
  matriz<double> bessel=dat_func2(bessel_2d,-10.,10.,0.8);
  env_archivo("bessel.dat",bessel);
  matriz<double> y;
  matriz<double> x=split(bessel,y,1);
  env_archivo("bessel_x.dat",x);
  env_archivo("bessel_y.dat",y);
  
  return 0;
}

double paraboloide(double a,double b){
  return a*a+b*b;
}

//Funcion de bessel segunda tarea
double iter_bessel_dec(double x,int n,int N){
  //Variable donde se almacenara el valor final
  double Jn;
  //Establezco condicion si x=0
  if(x==0){
    if(n==0)
      Jn=1;
    else
      Jn=0;
  }
  //De no ser ese el caso, itero
  else{
    //Primero fijo desde donde partir, de no indicarse N, este entra negativo y se autoasigna
    //De ser menor que 20 se ajusta a 20
    if(N<0)
      N=n+int(sqrt(10*n)+1);
    if(N<20)
      N=20;
    //Asigno los valores iniciales Ji_1=J_(i+1) y Ji_2=J_(i+2)
    double Ji_1=1;double Ji_2=1;
    //Creo la variable donde se guarda el valor de cada iteracion
    double Ji;
    //sum como resultado de la seria para normalizar, parte en uno pues contiene a Ji_1 o Ji_2 iniciales.
    double sum=1;
    //Realizo la iteracion desde N hasta cero, guardando en sum el valor de J_0 y 2*Jn con n par distinto de cero
    for(int i=N;i>=0;--i){
      Ji=(2*(i+1)/x)*Ji_1-Ji_2;
      Ji_2=Ji_1;
      if(i%2==0){
	if(i==0)
	  sum+=Ji;
	else
	  sum+=2*Ji;
      }
      //Al encontrar el n que busco lo almaceno
      if(i==n){
	Jn=Ji;
      }
      Ji_1=Ji;
    }
    //Finalmente divido el Jn que tenia por la suma (aqui normalizamos)
    Jn/=sum;
  }
  //Devuelvo tal valor como la evaluacion de Jn en el x (funcion de bessel)
  return Jn;
}

//Para el formato que me acepta la funcion que crea las matrices (y radial para que sea en 2d)
double bessel_2d(double a,double b){
  double r=sqrt(a*a+b*b);
  return iter_bessel_dec(r,0);
}
