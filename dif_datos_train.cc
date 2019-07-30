#include "neural.hh"

int main(int argc,char* argv[]){
  if(argc!=4) {cout<<"Introduzca datos y cant. de columnas en x, datos y cant. de columnas en y (esperados) y ratio de aprendizaje"<<endl;return 0;}
  //Variables generales
  int i;double a;matriz<double> x,y;char over;vector<int> estruc={atoi(argv[2]),4,6,atoi(argv[4])};
  //Guardo ambos archivos
  x=rec_archivo(argv[1],atoi(argv[2]),a);
  y=rec_archivo(argv[3],atoi(argv[4]),a);
  //Creo la malla, aquí se puede cambiar la funcion activadora (y su derivada claramente) si se desea.
  matriz<int> soonred(h,1);
  matriz<neural_l> malla=red(soonred,sigm,dsigm);
  //Creo carpeta de tal nombre sin sobreescribir ninguna anterior
  string carpeta=crea_carpeta("evolucion_red",i);

  /////AHORA HACER FUNCIONES QUE GUARDEN Y PROCESEN
					  
  cout<<"¿Desea estudiar también overfitting? (s o n)?"<<endl;cin>>over;
  //Sin estudio de sobrefiteo
  if(over=='n'){
    
  
  }
  //Con estudio de sobrefiteo
  else if(over=='s'){
    matriz<double> xs,ys;
    //Revuelvo azarozamente los datos (sin perder relacion x con y)
    //Además separo un 30% en xs e ys para estudiar el sobrefiteo.
    revuelve(x,y);separa(x,y,xs,ys,30);
  }

  else {cout<<"Opción no existente, se saldrá del programa"<<endl;return 0;}
  
  return 0;
}
