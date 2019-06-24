#include "neural.h"

neural_l::neural_l(){}

neural_l::neural_l(int in, int cel, double (*f)(double),double (*g)(double)){
  c_neu=cel;inp=in;
  B=mat_rand(1,c_neu,1,-1);//normalizado
  W=mat_rand(inp,c_neu,1,-1);//normalizado tambien
  act=(*f);//función de activación
  dact=(*g);//su derivada
}

neural_l neural_l::operator=(const neural_l &n){
  if(this!=&n){
    c_neu=n.c_neu;
    inp=n.inp;
    B=n.B;
    W=n.W;
    act=n.act;
    dact=n.dact;
  }
  return *this;
}


ostream & neural_l::print(ostream &os) const{
  os<<'['<<this->inp<<','<<this->c_neu<<','<<(this->W)<<(this->B)<<']';
  return os;
}

matriz<double> neural_l::operator[](int i) const{
  if(i==0) return B;
  else return W;
}

matriz<double> & neural_l::operator[](int i){
  if(i==0) return B;
  else return W;
}

matriz<double> neural_l::operator()(const matriz<double> &m,int a){
  matriz<double> res(m);
  if(a==0){
    for(int i=0;i<m.fila();++i){
      for(int j=0;j<m.colu();++j){
	res(i,j)=(*act)(m(i,j));
      }
    }
  }
  else{
    for(int i=0;i<m.fila();++i){
      for(int j=0;j<m.colu();++j){
	res(i,j)=(*dact)(m(i,j));
      }
    }
  }
  return res;
}

int neural_l::axon(int a){
  if(a==0) return inp;
  else return c_neu;
}

matriz<neural_l> neural_l::operator+(neural_l n){
  if(this->c_neu==n.inp){
    neural_l defaul;
    matriz<neural_l> res(2,1,defaul);
    res(0,0)=(*this);
    res(1,0)=n;
    return res;
  }
  else{
    neural_l defaul;
    matriz<neural_l> res(1,1,defaul);
    cout<<"Mala concatenacion de capas!"<<endl;
    return res;
  }
}

ostream & operator<<(ostream &os,const neural_l &n){return n.print(os);}

matriz<neural_l> red(const matriz<int> &m,double (*f)(double),double (*g)(double)){
  neural_l defaul;matriz<neural_l> res(m.fila()-1,1,defaul);
  for(int i=0;i<m.fila()-1;++i){
    neural_l n(m(i,0),m(i+1,0),(*f),(*g));
    res(i,0)=n;
  }
  return res;
}

matriz<double> forward(const matriz<neural_l> &n,const matriz<double> &x){
  matriz<double> temp=x*n(0,0)[1];//uso los pesos, resulta matriz con cada fila resultados de un dato (columna cantidad de neuronas que trabajaron)
  temp=temp.fila(n(0,0)[0]);//Sumo las b's a cada fila y luego aplico la funcion de activacion.
  temp=n(0,0)(temp);
  for(int i=1;i<n.fila();++i){
    temp=temp*n(i,0)[1];
    temp=temp.fila(n(i,0)[0]);
    temp=n(i,0)(temp);
  }
  return temp; //La devuelvo en formato como se recibio, fila por dato y columna por dimension de dato
}

matriz<double> train(matriz<neural_l> &n,const matriz<double> &x,const matriz<double> &y,double (*derror)(const matriz<double> &,const matriz<double> &),double paso){
  vector<double> erro;double err;//Para guardar cada error
  //Forward
  matriz<double> temp;matriz<matriz<double>> vals(3,n.fila()+1,temp);//matriz donde guardare los W,z's y los a's (resultados de suma ponderada +b y de activarlos) respectivamente (filas). El primer elemento son los X (en tercera fila)
  temp=x*n(0,0)[1];vals(2,0)=x;
  temp=temp.fila(n(0,0)[0]);vals(0,1)=n(0,0)[1];vals(1,1)=temp;
  temp=n(0,0)(temp);vals(2,1)=temp;
  //Los output quedan traspuestos, entonces los opero así nomas
  for(int i=1;i<n.fila();++i){
    temp=temp*n(i,0)[1];
    temp=temp.fila(n(i,0)[0]);vals(0,i+1)=n(i,0)[1];vals(1,i+1)=temp;
    temp=n(i,0)(temp);vals(2,i+1)=temp;
  }
  //Backward
  clean(temp);matriz<matriz<double>> deltas(1,n.fila(),temp);
  //Calculo primera delta (de la ultima capa)
  deltas(0,n.fila()-1)=(*derror)(y,vals(2,vals.colu()-1))*n(n.fila()-1,0)(vals(1,vals.colu()-1),1);
  //Modifico valores de output capa
  n(n.fila()-1,0)[0]=n(n.fila()-1,0)[0]-paso*prom(deltas(0,n.fila()-1));
  n(n.fila()-1,0)[1]=n(n.fila()-1,0)[1]-paso*(vals(2,n.fila()-1).tras()*deltas(0,n.fila()-1));
  for(int i=n.fila()-2;i>=0;--i){//i es justamente la capa
    deltas(0,i)=mul_ele(deltas(0,i+1)*vals(0,i+2).tras(),n(i,0)(vals(1,i+1),1));
    n(i,0)[0]=n(i,0)[0]-paso*prom(deltas(0,i));
    n(i,0)[1]=n(i,0)[1]-paso*(vals(2,i).tras()*deltas(0,i));
  }
  return vals(2,vals.colu()-1);
}


