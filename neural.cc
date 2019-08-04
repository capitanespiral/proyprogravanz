#include "neural.hh"

///////////FUNCIONES INTERNAS////////////////////////

//Constructor por defecto, solo para rellenar mis matrices (que necesitan saber con que)
neural_l::neural_l(){}

neural_l::neural_l(int in, int cel, double (*f)(double),double (*g)(double)){
  c_neu=cel;inp=in;
  ctes=mat_rand(1,c_neu,1,-1);//normalizado y random
  pesos=mat_rand(inp,c_neu,1,-1);//lo mismo
  act=(*f);//función de activación
  dact=(*g);//su derivada
}

neural_l::neural_l(const matriz<double> &b,const matriz<double> &w,double (*f)(double),double (*g)(double)){
  c_neu=b.colu();inp=w.fila();
  ctes=b;pesos=w;
  act=(*f);dact=(*g);
}

neural_l neural_l::operator=(const neural_l &n){
  if(this!=&n){
    c_neu=n.c_neu;
    inp=n.inp;
    ctes=n.ctes;
    pesos=n.pesos;
    act=n.act;
    dact=n.dact;
  }
  return *this;
}

ostream & neural_l::print(ostream &os) const{
  os<<'['<<this->inp<<','<<this->c_neu<<','<<(this->pesos)<<(this->ctes)<<']';
  return os;
}

//0 para ctes otro para pesos
matriz<double> neural_l::operator[](int i) const{
  if(i==0) return ctes;
  else return pesos;
}

matriz<double> & neural_l::operator[](int i){
  if(i==0) return ctes;
  else return pesos;
}

//Opero la matriz entregada con la funcion de activacion o su derivada, en especifico, si a es cero con la funcion de activacion y otro para su derivada
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

//Con a =0 entrego inputs de la capa, otro entrego los outputs (cant de neuronas de la capa)
int neural_l::axon(int a){
  if(a==0) return inp;
  else return c_neu;
}

//Pego dos capas, resulta una matriz contenedora de estas
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


//////////////////FUNCIONES EXTERNAS/////////////////////

////Funciones elementales////

ostream & operator<<(ostream &os,const neural_l &n){return n.print(os);}

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
  temp=temp.fila(n(0,0)[0]);//Sumo las ctes a cada fila y luego aplico la funcion de activacion.
  temp=n(0,0)(temp);
  //Lo mismo pero en bucle
  for(int i=1;i<n.fila();++i){
    temp=temp*n(i,0)[1];
    temp=temp.fila(n(i,0)[0]);
    temp=n(i,0)(temp);
  }
  return temp; //La devuelvo en formato como se recibio, fila por dato y columna por dimension de dato
}

//Entreno a la red, un recorrido forward y uno backward
matriz<double> train(matriz<neural_l> &n,const matriz<double> &x,const matriz<double> &y,matriz<double> (*derror)(const matriz<double> &,const matriz<double> &),double paso){
  matriz<double> temp;matriz<matriz<double>> vals(3,n.fila()+1,temp);//matriz donde guardare los pesos,a's y los b's (resultados de suma ponderada +ctes y de activarlos) respectivamente (filas). El primer elemento son los X, datos de input originales (en tercera fila)
  //Forward, tal como en la funcion anterior pero guardando resultados importantes
  temp=x*n(0,0)[1];vals(2,0)=x;
  temp=temp.fila(n(0,0)[0]);vals(0,1)=n(0,0)[1];vals(1,1)=temp;
  temp=n(0,0)(temp);vals(2,1)=temp;
  for(int i=1;i<n.fila();++i){
    temp=temp*n(i,0)[1];
    temp=temp.fila(n(i,0)[0]);vals(0,i+1)=n(i,0)[1];vals(1,i+1)=temp;
    temp=n(i,0)(temp);vals(2,i+1)=temp;
  }
  //Backward
  //Limpio temp y la uso para crear matriz de matrices donde guardare las deltas (para modificar la red)
  clean(temp);matriz<matriz<double>> deltas(1,n.fila(),temp);
  //Calculo primeras deltas (de la ultima capa)
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

//Funcion de activacion sigmoide
double sigm(double x){
  double a=1./(1+exp(-x));
  return a;
}

//Su derivada
double dsigm(double x){
  double a= exp(-x)/((1+exp(-x))*(1+exp(-x)));
  return a;
}

//Error local
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

//Su derivada
matriz<double> d_e_cuad_m(const matriz<double> &y_o,const matriz<double> &y_e){
  return (y_o-y_e);
}

//////Adicionales para comodidad/////////

//Creo red segun lo que esta en 's'
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

//Guardo red en archivo 's'
void guarda_red(string s,const matriz<neural_l> &m){
  ofstream archivo(s);
  archivo<<m.fila()<<' '<<m(0,0).axon(0);
  for(int i=0;i<m.fila();++i)
    archivo<<' '<<m(i,0).axon(1);
  archivo<<endl;
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m(i,0).axon(1);++j){
      archivo<<m(i,0)[0](0,j)<<' ';//imprimo ctes
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

//Creo matriz cuadrada de min a max avanzando paso en x y en y (para testear como converge la red)
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

//Para saber a que orden de magnitud pertenece, importante para determinar cuantos ceros corresponden en un archivo y asi al listarlos esten en orden
int pot_10(int a){
  int i=0;
  while(a>=10){a=a/10.;++i;}
  return i;
}

//Crea carpeta sin sobreescribir alguna anterior, para poder correr varias veces sin dificultad.
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

//Recibe malla neuronal, datos x, datos y, ratio de aprendizaje, string de nombre de carpeta donde guardar archivos, cuantas iteraciones, si checkear o no,de chequear cada cuanto hacerlo, y si debe normalizar o no.
void entrena_guarda(matriz<neural_l> &malla,const matriz<double> &x,const matriz<double> &y,double learn_rate,string carpeta,int itera,bool check,int cada_cuanto,char normaliza){
  //Creo archivo donde guardo el error y algunas variables generales
  ofstream errorcito("./"+carpeta+"/error.dat");
  matriz<double> res;double errorcillo,miny,maxy;
  matriz<double> tempy(y);
  //Normalizar de ser necesario
  if(normaliza=='s'){
    //Guardo minimos y maximos originales para parametrizar a estos valores luego de usar la red.
    miny=min(y);maxy=max(y);
    tempy=normaliza_todo(y,0,1);
  }
  //Si es que quiero chequear
  if(check){
    //Creo variables importantes para chequear
    matriz<double> res_check;
    double maxx=max(x);double minn=min(x);
    matriz<double> check=matriz_cuad(minn-minn/10.,maxx+maxx/10.,0.1);
    //Loop principal
    for(int i=0;i<=itera;++i){
      //Si toca chequear
      if(i%cada_cuanto==0){
	//Uso la red pero no la entreno
	res_check=forward(malla,check);
	//Para tener la cantidad de ceros necesario (y guardar en orden)
	string cant_ceros;
	if(pot_10(itera)>pot_10(i)){
	  cant_ceros="0";
	  for(int j=1;j<(pot_10(itera)-pot_10(i));++j)
	    cant_ceros=cant_ceros+"0";
	}
	//Creo y guardo el estado actual
	ofstream estado("./"+carpeta+"/Estado_"+cant_ceros+to_string(i)+".dat");
	//De haber normalizado, vuelvo al intervalo original
	if(normaliza=='s') res_check=normaliza_todo(res_check,miny,maxy);
	//Guardo en archivo
	for(int j=0;j<check.fila();++j){
	  estado<<check(j,0)<<' '<<check(j,1)<<' '<<res_check(j,0)<<endl;
	  //Esto es por el formato exigido de pm3d (gnuplot)
	  if(check(j+1,0)!=check(j,0)) estado<<endl;
	}
	estado.close();
      }
      //Entreno la red y guardo el error.
      res=train(malla,x,tempy,d_e_cuad_m,learn_rate);
      errorcillo=e_cuad_m(res,tempy);
      //Lo imprimo para ver evolucion de red en pantalla
      cout<<"Iteracion: "<<i<<" Error -> "<<errorcillo<<endl;
      errorcito<<i<<' '<<errorcillo<<endl;
    }
    errorcito.close();
  }
  //De no querer chequear
  else{
    //Solo entreno
    for(int i=0;i<=itera;++i){
      res=train(malla,x,tempy,d_e_cuad_m,learn_rate);
      errorcillo=e_cuad_m(res,tempy);
      cout<<"Iteracion: "<<i<<" Error -> "<<errorcillo<<endl;
      errorcito<<i<<' '<<errorcillo<<endl;
    }
    errorcito.close();
  }
}

////////////////funciones sobre overfitting///////77

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

//Para separar la muestra total. Asumo x e y originales y las que quedaran con mayor porcentaje de datos. El resto se quedara en y y z.
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

//Same que entrena_guarda pero para overfitting
//Recibe casi lo mismo, se agregan xs e ys (subconjunto de datos originales)
void entrena_guarda_overf(matriz<neural_l> &malla,const matriz<double> &x,const matriz<double> &y,const matriz<double> &xs,const matriz<double> &ys,double learn_rate,string carpeta,int itera,bool check,int cada_cuanto,char normaliza){
  //Creo archivo donde guardo el error y dos variables generales
  ofstream errorcito("./"+carpeta+"/error_train.dat");
  ofstream errorcito_("./"+carpeta+"/error_try.dat");
  matriz<double> tempy(y);matriz<double> tempys(ys);
  matriz<double> res,res_;double errorcillo,errorcillo_,miny,maxy;
  //para usar miny y maxy si hay que renormalizar.
  if(normaliza=='s'){
    miny=min(y);maxy=max(y);
    double minys=min(ys);double maxys=max(ys);
    if(miny>minys) miny=minys;
    if(maxy<maxys) maxy=maxys;
    tempy=normaliza_todo(y,0,1);tempys=normaliza_todo(ys,0,1);
  }
  //Si es que quiero chequear
  if(check){
    //Creo variables importantes para chequear
    matriz<double> res_check;
    double maxx=max(x);double minn=min(x);
    if(maxx<max(xs)) maxx=max(xs);
    if(minn>min(xs)) minn=min(xs);
    matriz<double> check=matriz_cuad(minn,maxx,0.1);
    //Loop principal
    for(int i=0;i<=itera;++i){
      //Si toca chequear
      if(i%cada_cuanto==0){
	//Uso la red pero no la entreno
	res_check=forward(malla,check);
	//Para tener la cantidad de ceros necesario (y guardar en orden)
	string cant_ceros;
	if(pot_10(itera)>pot_10(i)){
	  cant_ceros="0";
	  for(int j=1;j<(pot_10(itera)-pot_10(i));++j)
	    cant_ceros=cant_ceros+"0";
	}
	//Creo y guardo el estado actual
	ofstream estado("./"+carpeta+"/Estado_"+cant_ceros+to_string(i)+".dat");
	//Si se normalizo vuelvo al intervalo original
	if(normaliza=='s') res_check=normaliza_todo(res_check,miny,maxy);
	for(int j=0;j<check.fila();++j){
	  estado<<check(j,0)<<' '<<check(j,1)<<' '<<res_check(j,0)<<endl;
	  if(check(j+1,0)!=check(j,0)) estado<<endl;
	}
	estado.close();
      }
      //Entreno la red y guardo el error. En este caso de del que proviene del entrenamiento y el que proviene de la muestra tomada
      res=train(malla,x,tempy,d_e_cuad_m,learn_rate);
      res_=forward(malla,xs);
      errorcillo=e_cuad_m(res,tempy);
      errorcillo_=e_cuad_m(res_,tempys);
      cout<<"Iteracion: "<<i<<" Error train -> "<<errorcillo<<" Error try -> "<<errorcillo_<<endl;
      errorcito<<i<<' '<<errorcillo<<endl;
      errorcito_<<i<<' '<<errorcillo_<<endl;
    }
    errorcito.close();
    errorcito_.close();
  }
  else{
    //Solo entreno
    for(int i=0;i<=itera;++i){
      res=train(malla,x,tempy,d_e_cuad_m,learn_rate);
      res_=forward(malla,xs);
      errorcillo=e_cuad_m(res,tempy);
      errorcillo_=e_cuad_m(res_,tempys);
      cout<<"Iteracion: "<<i<<" Error train -> "<<errorcillo<<" Error try -> "<<errorcillo_<<endl;
      errorcito<<i<<' '<<errorcillo<<endl;
      errorcito_<<i<<' '<<errorcillo_<<endl;
    }
    errorcito.close();
    errorcito_.close();
  }
}



