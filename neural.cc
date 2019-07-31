#include "neural.hh"

///////////FUNCIONES INTERNAS////////////////////////

neural_l::neural_l(){}

neural_l::neural_l(int in, int cel, double (*f)(double),double (*g)(double)){
  c_neu=cel;inp=in;
  B=mat_rand(1,c_neu,1,-1);//normalizado y random
  W=mat_rand(inp,c_neu,1,-1);//lo mismo
  act=(*f);//función de activación
  dact=(*g);//su derivada
}

neural_l::neural_l(const matriz<double> &b,const matriz<double> &w,double (*f)(double),double (*g)(double)){
  c_neu=b.colu();inp=w.fila();
  B=b;W=w;
  act=(*f);dact=(*g);
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

//pego dos capas, resulta una matriz contenedora de estas
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



//////////////////FUNCIONES EXTERNAS/////////////////////7

//recibe matriz columna
matriz<neural_l> red(const matriz<int> &m,double (*f)(double),double (*g)(double)){
  neural_l defaul;matriz<neural_l> res(m.fila()-1,1,defaul);
  for(int i=0;i<m.fila()-1;++i){
    neural_l n(m(i,0),m(i+1,0),(*f),(*g));
    res(i,0)=n;
  }
  return res;
}

//Calculo hacia adelante
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

matriz<double> train(matriz<neural_l> &n,const matriz<double> &x,const matriz<double> &y,matriz<double> (*derror)(const matriz<double> &,const matriz<double> &),double paso){
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
  deltas(0,n.fila()-1)=mul_ele((*derror)(vals(2,vals.colu()-1),y),n(n.fila()-1,0)(vals(1,vals.colu()-1),1));
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

//Desordena (asumo fila cada dato) un tercio de cambios de la cantidad de datos
void revuelve(matriz<double> &x,matriz<double> &y){
  if(x.fila()==y.fila()){
    matriz<double> tempx(1,x.colu());
    matriz<double> tempy(1,y.colu());
    rdom cambio;
    int a,b;
    for(int i=0;i<(x.fila()/3)+1;++i){
      a=cambio.irand(x.fila()-1,0);
      b=cambio.irand(x.fila()-1,0);
      for(int j=0;j<x.colu();++j){
	tempx(0,j)=x(a,j);x(a,j)=x(b,j);x(b,j)=tempx(0,j);	
      }
      for(int j=0;j<y.colu();++j){
	tempy(0,j)=y(a,j);y(a,j)=y(b,j);y(b,j)=tempy(0,j);
      }
    }
  }
  else
    cout<<"No tienes la misma cantidad de datos"<<endl;
}

//Para separar la muestra total
void separa(matriz<double> &x,matriz<double> &y,matriz<double> &xs,matriz<double> &ys,int porc){
   if(x.fila()==y.fila()){
     rdom cambio;cambio.upd();int a;int i=0;
     int cant=porc*x.fila()/100;
     matriz<double> tempxs(cant,x.colu());
     matriz<double> tempys(cant,y.colu());
     matriz<double> tempx(x.fila()-cant,x.colu());
     matriz<double> tempy(x.fila()-cant,y.colu());
     vector<int> v;
     while(i<porc*x.fila()/100){
       a=cambio.irand(x.fila()-1,0);
       if(find(v.begin(), v.end(), a) != v.end()) continue;
       else {
	 v.push_back(a);
	 for(int j=0;j<x.colu();++j) tempxs(i,j)=x(a,j);
	 for(int j=0;j<y.colu();++j) tempys(i,j)=y(a,j);
	 ++i;
       }
     }
     a=0;
     for(int i=0;i<x.fila();++i){
       if(find(v.begin(), v.end(), i) != v.end()) continue;
       else {
       for(int j=0;j<x.colu();++j)
	 tempx(a,j)=x(i,j);
       for(int j=0;j<y.colu();++j)
	 tempy(a,j)=y(i,j);
       ++a;
       }
     }
     x=tempx;y=tempy;
     xs=tempxs;ys=tempys;
   }
   else
     cout<<"No tienes la misma cantidad de datos"<<endl;
}



double sigm(double x){
  double a=1./(1+exp(-x));
  return a;
}

double dsigm(double x){
  double a= exp(-x)/((1+exp(-x))*(1+exp(-x)));
  return a;
}

double e_cuad_m(const matriz<double> &y_o,const matriz<double> &y_e){
  matriz<double> res=y_o-y_e;
  double acum=0;
  for(int i=0;i<res.fila();++i){
    for(int j=0;j<res.colu();++j){
      acum+=res(i,j)*res(i,j);
    }
  }
  acum/=0.5*res.fila();
  return acum;
}

matriz<double> d_e_cuad_m(const matriz<double> &y_o,const matriz<double> &y_e){
  return (y_o-y_e);
}

string crea_carpeta(string s, int &a){
  int i=0;
  while(true){
    DIR* dir = opendir((s+to_string(i)).c_str());
    if(dir) {++i;closedir(dir);continue;}
    else break;
  }
  string carpeta=s+to_string(i);
  system(("mkdir "+carpeta).c_str());
  a=i;
  return carpeta;
}

matriz<neural_l> red_prehecha(string s,double (*f)(double),double (*g)(double)){
  ifstream archivo(s);
  int capas,i,numerito;vector<int> v;
  archivo>>capas;i=0;
  while(i<=capas){
    archivo>>numerito;v.push_back(numerito);++i;
  }
  neural_l defaul;
  matriz<neural_l> redd(capas,1,defaul);
  matriz<double> B;
  double numerote;vector<double> w;bool flag=true;i=0;
  while(archivo>>numerote){
    w.push_back(numerote);
    if(flag && w.size()==v[i+1]){
      matriz<double> F(1,w);B=F;
      w.clear();flag=!flag;
    }
    else if(w.size()==v[i]*v[i+1]){
      matriz<double> W(v[i],w);
      neural_l capa(B,W,f,g);
      redd(i,0)=capa;
      w.clear();flag=!flag;++i;  
    }
  }
  archivo.close();
  return redd;
}

void guarda_red(string s,const matriz<neural_l> &m){
  ofstream archivo(s);
  archivo<<m.fila()<<' '<<m(0,0).axon(0);
  for(int i=0;i<m.fila();++i)
    archivo<<' '<<m(i,0).axon(1);
  archivo<<endl;
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m(i,0).axon(1);++j){
      archivo<<m(i,0)[0](0,j)<<' ';//imprimo B
    }
    archivo<<endl;
    for(int j=0;j<m(i,0).axon(0);++j){
      for(int h=0;h<m(i,0).axon(1);++h){
	archivo<<m(i,0)[1](j,h)<<' ';
      }
      archivo<<endl;
    }
  }
  archivo.close();
}

matriz<double> normaliza_col(const matriz<double> &m,double Min,double Max,int a){
  matriz<double> res(m);
  double minn=min(m.colu(a));
  double maxx=max(m.colu(a));
  for(int i=0;i<m.fila();++i){
    res(i,a)=Min+(m(i,a)-minn)*(Max-Min)/(maxx-minn);
  }
  return res;
}

matriz<double> matriz_cuad(double min,double max,double paso){
  int cant=((max-min)/paso+1);
  double minx=min;
  double temp=min;
  matriz<double> res(cant*cant,2);
  int i=0;
  while(i<res.fila()){
    res(i,0)=minx;res(i,1)=min;
    ++i;min+=paso;
    if(min>max){min=temp;minx+=paso;}
  }
  return res;
}

//Para saber a que orden de magnitud pertenece
int pot_10(int a){
  int i=0;
  while(a>=10){a=a/10.;++i;}
  return i;
}

//Recibe malla neuronal, datos x, datos y,string de nombre de quien recibe el error, cuantas iteraciones, si checkear o no, y de chequear cada cuanto.
void entrena_guarda(matriz<neural_l> &malla,const matriz<double> &x,const matriz<double> &y,double learn_rate,string carpeta,int itera,bool check,int cada_cuanto){
  ofstream errorcito("./"+carpeta+"/error.dat");
  matriz<double> res;double errorcillo;
  if(check){
    matriz<double> res_check;
    double maxx=max(x);double minn=min(x);
    matriz<double> check=matriz_cuad(minn,maxx,0.1);
    for(int i=0;i<=itera;++i){
      if(i%cada_cuanto==0){
	res_check=forward(malla,check);
	string cant_ceros;
	if(pot_10(itera)>pot_10(i)){
	  cant_ceros="0";
	  for(int j=1;j<(pot_10(itera)-pot_10(i));++j)
	    cant_ceros=cant_ceros+"0";
	}
	ofstream estado("./"+carpeta+"/Estado_"+cant_ceros+to_string(i)+".dat");
	for(int j=0;j<check.fila();++j){
	  estado<<check(j,0)<<' '<<check(j,1)<<' '<<res_check(j,0)<<endl;
	  if(check(j+1,0)!=check(j,0)) estado<<endl;
	}
	estado.close();
      }
      res=train(malla,x,y,d_e_cuad_m,learn_rate);
      errorcillo=e_cuad_m(res,y);
      cout<<errorcillo<<endl;
      errorcito<<i<<' '<<errorcillo<<endl;
    }
    errorcito.close();
  }
  else{
    for(int i=0;i<=itera;++i){
      res=train(malla,x,y,d_e_cuad_m,learn_rate);
      errorcillo=e_cuad_m(res,y);
      cout<<errorcillo<<endl;
      errorcito<<i<<' '<<errorcillo<<endl;
    }
  }
}
