#include "random.hh"

rdom::rdom(uint a){
  srand(a);
}

void rdom::upd(uint a){
  static int b=0;
  ++b;
  srand(a+b);
}

int rdom::irand(int max,int min){
  return min+(rand() %(max-min+1));
}

double rdom::drand(double max,double min){
  int a=rand();
  return min+double(a)*(max-min)/RAND_MAX;
}

matriz<double> mat_rand(int f,int c,double max,double min){
  matriz<double> res(f,c);
  rdom random;
  static int a=0;
  if(a>0) random.upd();
  for(int i=0;i<res.fila();++i){
    for(int j=0;j<res.colu();++j){
      res(i,j)=random.drand(max,min);
    }
  }
  ++a;
  return res;
}

matriz<double> distr(double (*f)(double),double minf,double maxf,double minx,double maxx,int pts){
  rdom random;
  double x,y,eval;
  matriz<double> res(pts,1);
  int cont=0; //cuantos puntos llevo
  while(cont<pts){
    x=random.drand(maxx,minx);
    y=random.drand(maxf+(maxf-minf)/10,minf-(maxf-minf)/10);
    eval=(*f)(x);
    if(y<eval){
      res(cont,0)=x;
      ++cont;
    }
  }
  return res;
}

matriz<double> noise(const matriz<double> &m,double ruido,int num,bool columna){
  rdom random;
  matriz<double> res(m);
  if(columna){
    for(int i=0;i<m.fila();++i){
      res(i,num)=random.drand(res(i,num)+ruido,res(i,num)-ruido);
    }
    return res;
  }
  else{
    for(int i=0;i<m.colu();++i){
      res(num,i)=random.drand(res(num,i)+ruido,res(num,i)-ruido);
    }
    return res;
  }
}

//de r a r
matriz<double> dat_func1(double (*f)(double),double first,double last,int pts){
  matriz<double> res(pts,2);
  double sep=(last-first)/pts;
  for(int i=0;i<res.fila();++i){
    res(i,0)=first;res(i,1)=(*f)(first);
    first+=sep;
  }
  return res;
}

//Evaluar funcion que recibe dos numeros y entrega numero
matriz<double> dat_func2(double (*f)(double,double),double start,double fin,double paso){
  int cant=((fin-start)/paso+1);
  double minx=start;
  double temp=start;
  matriz<double> res(cant*cant,3);
  int i=0;
  while(i<res.fila()){
    res(i,0)=minx;res(i,1)=start;res(i,2)=(*f)(res(i,0),res(i,1));
    ++i;start+=paso;
    if(start>fin){start=temp;minx+=paso;}
  }
  return res;
}

//Parametrizo cierta columna de una matriz respecto un min o max fijado por mi
matriz<double> normaliza_col(const matriz<double> &m,double Min,double Max,int a){
  matriz<double> res(m);
  double minn=min(m.colu(a));
  double maxx=max(m.colu(a));
  for(int i=0;i<m.fila();++i){
    res(i,a)=Min+(m(i,a)-minn)*(Max-Min)/(maxx-minn);
  }
  return res;
}

//Lo mismo pero para toda la matriz
matriz<double> normaliza_todo(const matriz<double> &m, double Min, double Max){
  matriz<double> res(m);
  double minn=min(m);
  double maxx=max(m);
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m.colu();++j){
      res(i,j)=Min+(m(i,j)-minn)*(Max-Min)/(maxx-minn);
    }
  }
  return res;
}
