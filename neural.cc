#include "neural.h"

neural_l::neural_l(){}

neural_l::neural_l(int in, int cel){
  c_neu=cel;inp=in;
  B=mat_rand(c_neu,1,1,-1);//normalizado
  W=mat_rand(c_neu,inp,1,-1);//normalizado tambien
}

neural_l neural_l::operator=(const neural_l &n){
  if(this!=&n){
    c_neu=n.c_neu;
    inp=n.inp;
    B=n.B;
    W=n.W;
  }
  return *this;
}

matriz<neural_l> red(const matriz<int> &m){
  neural_l defaul;matriz<neural_l> res(m.fila()-1,1,defaul);
  for(int i=0;i<m.fila()-1;++i){
    neural_l n(m(i,0),m(i+1,0));
    res(i,0)=n;
  }
  return res;
}

ostream & neural_l::print(ostream &os) const{
  os<<'['<<this->inp<<','<<this->c_neu<<','<<(this->W)<<(this->B).tras()<<']';
  return os;
}

int neural_l::cant_n(){return c_neu;}

matriz<double> neural_l::operator[](int i) const{
  if(i==0) return B;
  else return W;
}

matriz<double> & neural_l::operator[](int i){
  if(i==0) return B;
  else return W;
}

ostream & operator<<(ostream &os,const neural_l &n){return n.print(os);}


matriz<double> forward(const matriz<neural_l> &n,const matriz<double> &x,double (*f)(double)){
  matriz<double> temp=n(0,0)[1]*x.tras();//uso los pesos, resulta matriz con cada columna resultados de un dato (fila cantidad de neuronas que trabajaron)
  temp=map((*f),temp.colu(n(0,0)[0])); //Sumo las b's a cada columna y luego aplico la funcion de activacion.
  //Los output quedan traspuestos, entonces los opero así nomas
  for(int i=1;i<n.fila();++i){
    temp=n(i,0)[1]*temp;
    temp=map((*f),temp.colu(n(i,0)[0]));
  }
  return temp.tras() //La devuelvo en formato como se recibio, fila por dato y columna por dimension de dato
}

