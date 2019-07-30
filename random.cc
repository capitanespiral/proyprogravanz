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

matriz<double> dat_func(double (*f)(double),double first,double last,int pts){
  matriz<double> res(pts,2);
  double sep=(last-first)/pts;
  for(int i=0;i<res.fila();++i){
    res(i,0)=first;res(i,1)=(*f)(first);
    first+=sep;
  }
  return res;
}

/*matriz<double> distr(double (*f)(double),double minf,double maxf,double minx,double maxx,char c,int pts){
  rdom random;
  double x,y,eval;
  matriz<double> res(pts,2);
  int cont=0; //cuantos puntos llevo
  if(c=='f'){
    while(cont<pts){
      x=random.drand(maxx,minx);
      y=random.drand(maxf+(maxf-minf)/10,minf-(maxf-minf)/10);
      eval=(*f)(x);
      if(y<eval){
	res(cont,0)=x;res(cont,1)=(*f)(x);
	++cont;
      }
    }
  }
  return res;
}

//Como hacerlo para histograma? si tengo una funcion que ordene dentro de la matriz de menor a mayor basta armarlo ordenadito luego de tener todos los datos!. Seria genial hacerlo todo dentro del mismo while pero no veo como :C
matriz<double> distr(double (*f)(double),double minf,double maxf,double minx,double maxx,int barr,int pts){
  rdom random;
  double ampli=(maxx-minx)/barr;
  double x,y,eval;
  matriz<doubl;
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
}*/

