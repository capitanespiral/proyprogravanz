#ifndef MATRIZ_H
#define MATRIZ_H
#include<iostream>
#include<vector>
#include<cmath>

using namespace std;
typedef unsigned int uint;

//Falta su MAP!

//Clase de matrices con templates
template <class T>
class matriz{
 private://Elementos privados, cantidad de filas, columnas, y vector que almacena los elementos.
  int f,c;
  vector<T> mat;
 public:
  matriz();//Constructor por defecto
  matriz(int,int,T=0);//filas,columnas y con que llenarlo
  matriz(int,const vector<T>&);//fila, y vector con elementos
  matriz(const vector<T>&,int);//vector con elementos y columnas
  matriz(vector<vector<T>>);//armado con vector de vectores
  matriz(const matriz&);//constructor de copia
  matriz operator=(const matriz&);//Asignacion con =
  T& operator()(int,int);//sobrecarga que me entrega el elemento i,j (para sobreescribir)
  T operator()(int,int) const;//Lo mismo pero para observar
  int fila() const {return f;}//Funcion que me devuelve la cantidad de filas
  int colu() const {return c;}//Lo mismi pero con cantidad de columnas
  matriz fila(int) const;//Entrega la fila i como nueva matriz (de 1xn)
  matriz colu(int) const;//Entrega la columna i como nueva matriz (de nx1)
  matriz tras() const;//Entrega la traspuesta
  matriz colu(const matriz &);//Sumo a cada columna la matriz columna
	matriz fila(const matriz &); //same con fila
};

/////////////////CONSTRUCTORES//////////////////

//Constructor por defecto, matriz 1x1 con un cero.
template <class T>
matriz<T>::matriz(): f(1),c(1),mat({0}){}

//Constructor de tamaño axb relleno de h
template <class T>
matriz<T>::matriz(int a,int b, T h){
  f=a;
  c=b;
  for(int i=0;i<a*b;++i)
    mat.push_back(h);
}

//Constructor de fila a con los elementos de v.
template <class T>
matriz<T>::matriz(int a,const vector<T> &v){
  //Si la cantidad de elementos no completa la matriz (dada la cantidad de filas deseadas), se rellena con ceros (y se avisa)
  if(v.size()%a!=0){
    cout<<"Cuidado, incorrecta cantidad de elementos, se rellenará con ceros"<<endl;
    vector<T> h=v;
    while(h.size()%a!=0)
      h.push_back(0);
    f=a;c=h.size()/a;mat=h;
  }
  else{
    f=a;c=v.size()/a;mat=v;
  } 
}

//Constructor de vector con elementos y columna
template <class T>
matriz<T>::matriz(const vector<T> &v,int a){
  if(v.size()%a!=0){
    cout<<"Cuidado, incorrecta cantidad de elementos, se rellenará con ceros"<<endl;
    vector<T> h=v;
    while(h.size()%a!=0)
      h.push_back(0);
    c=a;f=h.size()/a;mat=h;
  }
  else{
    c=a;f=v.size()/a;mat=v;
  } 
} 

//Constructor de vector de vectores, se asume que la cantidad de vectores son las filas y la cantidad de elementos por vector las columnas
//Se tienen que entregar la cantidad de elementos justa.
template <class T>
matriz<T>::matriz(vector<vector<T>> v){
  vector<T> h;
  for(uint i=0;i<v.size();++i)
    for(uint j=0;j<v[i].size();++j)
      h.push_back(v[i][j]);
  f=v.size();
  c=v[0].size();
  mat=h;
}

//Construtor de copia
template <class T>
matriz<T>::matriz(const matriz<T> &m){
  f=m.f;
  c=m.c;
  mat=m.mat;
}

/////////////////////////////SOBRECARGAS Y METODOS//////////////////////////////////

//Asignacion con igual (deep copy)
template <class T>
matriz<T> matriz<T>::operator=(const matriz<T> &m){
  if(this!=&m){
    f=m.f;
    c=m.c;
    mat=m.mat;
  }
  return *this;
}
  
//Aqui defino la sobrecarga de (), esta me permite tener la matriz con un solo vector.
template <class T>
T& matriz<T>::operator()(int a,int b){
  return mat[c*(a)+(b)];
}

template <class T>
T matriz<T>::operator()(int a,int b) const{
  return mat[c*(a)+(b)];
}

//Entrego como nueva matriz de 1xc la fila a.
template <class T>
matriz<T> matriz<T>::fila(int a) const{
  matriz<T> m(1,c);
  for(int i=0;i<c;++i)
    m(0,i)=(*this)(a,i);
  return m;
}

//Lo mismo pero con la columna (matriz de fx1)
template <class T>
matriz<T> matriz<T>::colu(int a) const{
  matriz<T> m(f,1);
  for(int i=0;i<f;++i)
    m(i,0)=(*this)(i,a);
  return m;
}

//Devuelvo la traspuesta
template <class T>
matriz<T> matriz<T>::tras() const{
  matriz<T> m(c,f);
  for(int i=0;i<f;++i){
    for(int j=0;j<c;++j){
      m(j,i)=(*this)(i,j);
    }
  }
  return m;
}

/////////////////////////OPERACIONES ARITMETICAS/////////////////////////

//Suma entre matrices, si las dimensiones no son correctas se entrega matriz vacia y un mensaje avisando.
template <class T>
matriz<T> operator+(const matriz<T> &m, const matriz<T> &n){
  if(m.fila() == n.fila() && m.colu() == n.colu() ){
    matriz<T> res(m.fila(),m.colu());
    for(int i=0;i<res.fila();++i){
      for(int j=0;j<res.colu();++j){
	res(i,j)=m(i,j)+n(i,j);
      }
    }
    return res;
  }
  else{
    cout<<"Por favor, suma/resta matrices de las mismas dimensiones"<<endl;
    matriz<T> res;
    return res;
  }
}

//Multiplicacion por un escalar se acepta de otro tipo pero se transformo al tipo que guarda la matriz. Luego la definicion conmutativa
template <class T,class U>
  matriz<T> operator*(U a,const matriz<T> &m){
  matriz<T> res(m.fila(),m.colu());
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m.colu();++j){
      res(i,j)=m(i,j)*T(a);
    }
  }
  return res;
}

template <class T,class U>
matriz<T> operator*(const matriz<T> &m,U a){
  matriz<T> res(m.fila(),m.colu());
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m.colu();++j){
      res(i,j)=m(i,j)*T(a);
    }
  }
  return res;
}

//Division por escalar (usando la multiplicacion por escalar), solo valido como matriz/escalar.
template <class T,class U>
matriz<T> operator/(const matriz<T> &m,U a){
  matriz<T> res=(1/T(a))*m;
  return res;
}

//Resta entre matrices, usando multiplicacion por escalar.
template <class T>
matriz<T> operator-(const matriz<T> &m, const matriz <T> &n){
  matriz<T> res=m+(T(-1)*n);
  return res;
}

//Multiplicacion entre matrices. De no tener las dimensiones correctas se entrega mensaje y retorna una matriz nula.
template <class T>
matriz<T> operator*(const matriz<T> &m,const matriz<T> &n){
  if(m.colu() == n.fila()){
    matriz<T> res(m.fila(),n.colu());
    T acum;
    int k=-1;
    do{
      ++k;
      for(int i=0;i<m.fila();++i){
	acum=0;
	for(int j=0;j<n.fila();++j){
	  acum+=m(i,j)*n(j,k);
	}
	res(i,k)=acum;
      }
    }
    while(k!=n.colu()-1);
    return res;
  }
  else{
    cout<<"Por favor multiplica dos matrices con las dimensiones correcta"<<endl;
    matriz<T> res;
    return res;
  }
}

///////////////////////////VARIOS/////////////////////////

//Impresion de las matrices
template <class T>
ostream & operator<<(ostream &os,const matriz<T> &m){
  os<<'(';
  for(int i=0;i<m.fila()-1;++i){
    for(int j=0;j<m.colu()-1;++j){
      os<<m(i,j)<<' ';
    }
    os<<m(i,m.colu()-1);
    os<<endl;
    os<<' ';
  }
  for(int j=0;j<m.colu()-1;++j)
    os<<m(m.fila()-1,j)<<' ';
  os<<m(m.fila()-1,m.colu()-1)<<')';
  return os;
}

//Concatena dos matrices ya sea por columnas o filas. Recibe las dos matrices y un indicador tipo char (por defecto busca concatenar filas)
template <class T>
matriz<T> cat(const matriz<T> &m,const matriz<T> &n,char a='f'){
  int f,c;
  //Si escogo filas y se cumple que su cantidad de filas es la misma
  if(a=='f' && m.fila()==n.fila()){
    f=m.fila();c=m.colu()+n.colu();
    matriz<T> res(f,c,m(0,0));
    for(int i=0;i<m.fila();++i){
      for(int j=0;j<m.colu();++j)
	res(i,j)=m(i,j);
      for(int h=0;h<n.colu();++h)
	res(i,h+m.colu())=n(i,h);
    }
    return res;
  }
  //Si no escojo filas se asume columna, y si ademas la cantidad de columnas son iguales.
  else if(a!='f' && m.colu()==n.colu()){
    f=m.fila()+n.fila();c=m.colu();
    matriz<T> res(f,c,m(0,0));
    for(int i=0;i<m.colu();++i){
      for(int j=0;j<m.fila();++j)
	res(j,i)=m(j,i);
      for(int h=0;h<n.fila();++h)
	res(h+m.fila(),i)=n(h,i);
    }
    return res;
  }
  //Si ninguna se cumple, entrego mensaje de error y matriz nula.
  else{
    cout<<"Error al tratar de concatenar matrices, dimensiones no correctas"<<endl;
    matriz<T> res;
    return res;}
}

//De estas dos funciones, si la matriz es nula te dice que es tridiagonal, lol
//Funcion que revisa si es tridiagonal o no.
template <class T>
bool tridiag(const matriz<T> &m){
  if(m.fila()!=m.colu()) return false;
  else{
    for(int i=0;i<m.fila();++i){
      for(int j=0;j<m.colu();++j){
	if(i==j || (i+1)==j || (j+1)==i) continue;
	if(m(i,j)!=0) {
	  return false;
	}
      }
    }
  }
  return true;
}

//Funcion que revisa si es tridiagonal periodica o no
template <class T>
bool tridiag_p(const matriz<T> &m){
    if(m.fila()!=m.colu()) return false;
  else{
    for(int i=0;i<m.fila();++i){
      for(int j=0;j<m.colu();++j){
	if(i==j || (i+1)==j || (j+1)==i || (i==0 && j==m.colu()-1) || (j==0 && i==m.fila()-1)) continue;
	if(m(i,j)!=0) {
	  return false;
	}
      }
    }
  }
  return true;
}

//Algoritmo de thomas para matrices tridiagonales (de no estar seguro, puedo revisar con la funcion de arriba.//Eficientizar (ifs, j's, contenedor)
template <class T>
matriz<T> thomas(const matriz<T> &m,const vector<T> &v){//Recibo matriz tridiagonal,vector de ctes.
  if(m.fila()==int(v.size())){
    T c,d,temp;
    int j=0;
    matriz<T> cont(2,v.size());
    c=m(0,1)/m(0,0);d=v[0]/m(0,0);    
    cont(0,0)=c;cont(1,0)=d;
    for(int i=1;i<m.fila();++i){
      temp=(m(i,i)-m(i,i-1)*cont(0,i-1));
      c=m(i,i+1)/temp;
      d=(v[i]-m(i,i-1)*cont(1,i-1))/temp;
      cont(0,i)=c;cont(1,i)=d;
    }
    matriz<T> res(v.size(),1);
    res(m.fila()-1,0)=cont(1,m.fila()-1);
    for(int n=m.fila()-2;n>=0;--n){
      res(n,0)=cont(1,n)-cont(0,n)*res(n+1,0);
    }
    return res;
  }
  else{
    cout<<"Faltan o sobran datos en el vector de constantes"<<endl;
    matriz<T> res;return res;
  }
}

//Algoritmo para matrices tridiagonales periodicas,ahlberg-nilsen-walsh. En un futuro eficientizarlo (segun el paper)
template <class T>
matriz<T> ANW(const matriz<T> &m,const vector<T> &v){
  matriz<T> temp(5,m.fila()-1);
  T ci,di,dj,div; //debo resolver dos matrices tridiagonales, pero solo varia el "vector respuesta" asi que ci sirve para ambos.
  ci=m(0,1)/m(0,0);di=v[0]/m(0,0);dj=m(0,m.colu()-1)/m(0,0);
  temp(0,0)=ci;temp(1,0)=di;temp(2,0)=dj;
  for(int i=1;i<m.fila()-1;++i){
    div=m(i,i)-ci*m(i,i-1);
    ci=m(i,i+1)/div;
    di=(v[i]-di*m(i,i-1))/div;
    dj=(m(i,m.colu()-1)-dj*m(i,i-1))/div; //el primer termino es cero excepto al final (en otro momento eficientizalo)
    temp(0,i)=ci;temp(1,i)=di;temp(2,i)=dj;
  }
  //barrido hacia atras
  //Fila 3 y 4 los q y los s respectivamente
  temp(3,m.fila()-2)=di;
  temp(4,m.fila()-2)=dj;
  for(int i=m.fila()-3;i>=0;--i){
    temp(3,i)=temp(1,i)-temp(0,i)*temp(3,i+1);
    temp(4,i)=temp(2,i)-temp(0,i)*temp(4,i+1);
  }
  //Ahora y tengo todos los q y s en teoria.Reutilizo dj para xn
  dj=(v[v.size()-1]-m(m.fila()-1,0)*temp(3,0)-m(m.fila()-1,m.colu()-2)*temp(3,m.fila()-2))/(m(m.fila()-1,m.colu()-1)-m(m.fila()-1,0)*temp(4,0)-m(m.fila()-1,m.colu()-2)*temp(4,m.fila()-2));
  matriz<T> res(v.size(),1);
  for(int i=0;i<m.fila()-1;++i)
    res(i,0)=temp(3,i)-dj*temp(4,i);
  res(v.size()-1,0)=dj;
  return res;
}

//Calcular modulo de cada fila (eventualmente para columnas tambien)
template <class T>
matriz<T> mod(const matriz<T> &m){
  matriz<T> res(m.fila(),1);
  T acum;
  for(int i=0;i<m.fila();++i){
    acum=0;
    for(int j=0;j<m.colu();++j){
      acum+=m(i,j)*m(i,j);
    }
    res(i,0)=sqrt(acum);
  }
  return res;
}

//Mismo para para una fila o columna en especifico (por ahora solo fila)
template <class T>
T mod(const matriz<T> &m,int a){
  T acum=0;
  for(int i=0;i<m.colu();++i)
    acum+=m(a,i)*m(a,i);
  return sqrt(acum);
}

//Obtener max de una matriz (mas adelante el minimo)
template <class T>
T max(const matriz<T> &m){
  T valorcillo=m(0,0);
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m.colu();++j)
      if(m(i,j)>valorcillo) valorcillo=m(i,j);
  }
  return valorcillo;
}

//Limpia la matriz y la llena de lo que uno quiera
template <class T>
void clean(matriz<T> &m,T a=0){
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m.colu();++j){
      m(i,j)=a;
    }
  }
}

//aplica cierta funcion a cada elemento de la matriz
template <class T>
matriz<T> map(T (*f)(T),const matriz<T> &m){
  matriz<T> res(m);
  for(int i=0;i<m.fila();++i){
    for(int j=0;j<m.colu();++j)
      res(i,j)=(*f)(m(i,j));
  }
  return res;
}

//Suma vector columna en cada columna
template <class T>
matriz<T> matriz<T>::colu(const matriz<T> &m){
  matriz<T> res(*this);
  for(int i=0;i<res.fila();++i){
    for(int j=0;j<res.colu();++j)
      res(i,j)=res(i,j)+m(i,0);
  }
  return res;
}

//Suma vector fila en cada fila
template <class T>
matriz<T> matriz<T>::fila(const matriz<T> &m){
  matriz<T> res(*this);
  for(int i=0;i<res.fila();++i){
    for(int j=0;j<res.colu();++j)
      res(i,j)=res(i,j)+m(0,j);
  }
  return res;
}

template<class T>
matriz<T> mul_ele(const matriz<T> &m,const matriz<T> &n){
	matriz<T> res;
	if(m.fila()==n.fila() && m.colu()==n.colu()){
		res=m;
		for(int i=0;i<res.fila();++i){
    			for(int j=0;j<res.colu();++j)
      				res(i,j)=res(i,j)*n(i,j);
  		}
	}
	else{
		cout<<"No son iguales las matrices"<<endl;
	}
	return res;
}

//Devuelve matriz fila de los promedios de las columnas
template<class T>
matriz<T> prom(const matriz<T> &m){
	matriz<T> res(1,m.colu());
	T acum=0;	
	for(int i=0;i<m.colu();++i){
		for(int j=0;j<m.fila();++j){
			acum+=m(j,i);			
		}
		res(0,i)=acum;acum=0;
	}
	return res;
}

#endif
