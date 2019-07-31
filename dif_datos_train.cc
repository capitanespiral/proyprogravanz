#include "neural.hh"

int main(int argc,char* argv[]){
  if(argc!=6) {cout<<"Introduzca datos y cant. de columnas en x, datos y cant. de columnas en y (esperados) y ratio de aprendizaje"<<' '<<argc<<endl;return 0;}
  //Variables generales
  int i,j;double a;matriz<double> x,y;char over;vector<int> estruc={atoi(argv[2]),4,6,atoi(argv[4])};bool check=false;
  //Guardo ambos archivos
  x=rec_archivo(argv[1],atoi(argv[2]),a);
  y=rec_archivo(argv[3],atoi(argv[4]),a);
  //Creo la malla, aquí se puede cambiar la funcion activadora (y su derivada claramente) si se desea.
  matriz<int> soonred(estruc,1);
  matriz<neural_l> malla=red(soonred,sigm,dsigm);
  //Creo carpeta de tal nombre sin sobreescribir ninguna anterior
  //Y archivo donde guardaré la evolucion del error
  string carpeta=crea_carpeta("evolucion_red",i);
  
  /////AHORA HACER FUNCIONES QUE GUARDEN Y PROCESEN
					  
  cout<<"¿Desea estudiar también overfitting? (s o n)?"<<endl;cin>>over;
  //Sin estudio de sobrefiteo
  if(over=='n'){
    //Creo archivo de ambos datos pegados para poder graficar mejor
    system(("paste "+string(argv[1])+" "+string(argv[3])+"> ./"+carpeta+"/datosjuntos.dat").c_str());
    cout<<"¿Cuantas iteraciones?"<<endl;cin>>i;
    cout<<"¿Desea chequear los datos? (s o n)"<<endl;cin>>over;
    if(over=='s'){
      check=true;
      cout<<"¿Cada cuantas iteraciones?"<<endl;cin>>j;
    }
    entrena_guarda(malla,x,y,atof(argv[5]),carpeta,i,check,j);
    cout<<"Listo!, guardado en "+carpeta<<endl;
    return 0;
  }
  //Con estudio de sobrefiteo
  /*else if(over=='s'){
    matriz<double> xs,ys;
    //Revuelvo azarozamente los datos (sin perder relacion x con y)
    //Además separo un 30% en xs e ys para estudiar el sobrefiteo.
    revuelve(x,y);separa(x,y,xs,ys,30);
    //Envio las matrices a archivos, los junto (para train y try) y borro los temporales.
    env_archivo("TempX",x);env_archivo("TempY",y);
    env_archivo("TempXS",xs);env_archivo("TempYS",ys);
    system(("paste TempX TempY > ./"+carpeta+"/datosjuntos_train"+to_string(i)+".dat").c_str());
    system(("paste TempXS TempYS > ./"+carpeta+"/datosjuntos_try"+to_string(i)+".dat").c_str());
    system(("rm TempX TempY TempXS TempYS").c_str());
  }

  else {cout<<"Opción no existente, se saldrá del programa"<<endl;return 0;}
  */
  return 0;
}
