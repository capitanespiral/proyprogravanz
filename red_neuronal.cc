#include "neural.hh"

int main(int argc,char* argv[]){
  //Mensaje por si fltan variables
  if(argc<4 || argc>5) {cout<<"Introduzca datos en x, datos en y (esperados) y ratio de aprendizaje, y opcional red al final"<<' '<<argc<<endl;return 0;}
  //Variables generales
  int i,j;double a;matriz<double> x,y;char over;bool check=false;char normal='n';neural_l defaul;matriz<neural_l> malla(1,1,defaul);
  //Guardo ambos archivos en matrices
  x=rec_archivo(argv[1],2,a);
  y=rec_archivo(argv[2],1,a);
  //Por si es necesario normalizar
  if(max(y)>1 || min(y)<0) normal='s';
  //Creo la malla, aquí se puede cambiar la funcion activadora (y su derivada claramente) si se desea.
  if(argc==4){
    vector<int> estruc={2,4,6,6,6,4,1};
    matriz<int> soonred(estruc,1);
    malla=red(soonred,sigm,dsigm);
  }
  else{
    malla=red_prehecha(argv[4],sigm,dsigm);
  }
  
  //Creo carpeta de tal nombre sin sobreescribir ninguna anterior
  //Y archivo donde guardaré la evolucion del error
  string carpeta=crea_carpeta("evolucion_red",i);
  //Si es que queremos estudiar además el sobrefitteo
  cout<<"¿Desea estudiar también overfitting? (s o n)?"<<endl;cin>>over;
  //Sin estudio de sobrefiteo
  if(over=='n'){
    //Creo archivo de ambos datos pegados para poder graficar mejor
    system(("paste "+string(argv[1])+" "+string(argv[2])+"> ./"+carpeta+"/datosjuntos.dat").c_str());
    //Datos para entrenar
    cout<<"¿Cuantas iteraciones?"<<endl;cin>>i;
    cout<<"¿Desea chequear los datos? (s o n)"<<endl;cin>>over;
    if(over=='s'){
      check=true;
      cout<<"¿Cada cuantas iteraciones?"<<endl;cin>>j;
    }
    //Entreno la red
    entrena_guarda(malla,x,y,atof(argv[3]),carpeta,i,check,j,normal);
    //si deseo guardar la red
    cout<<"¿Guardar la red? (s o n)"<<endl;cin>>over;
    if(over=='s') guarda_red("./"+carpeta+"/red.nn",malla);
    cout<<"Listo!, todo guardado en "+carpeta<<endl;
    return 0;
  }
  //Con estudio de sobrefiteo
  else if(over=='s'){
    matriz<double> xs,ys;
    //Revuelvo azarozamente los datos (sin perder relacion x con y)
    //Además separo un 30% en xs e ys para estudiar el sobrefiteo.
    revuelve(x,y);separa(x,y,xs,ys,30);
    //Envio las matrices a archivos, los junto (para train y try) y borro los temporales.
    env_archivo("TempX",x);env_archivo("TempY",y);
    env_archivo("TempXS",xs);env_archivo("TempYS",ys);
    system(("paste TempX TempY > ./"+carpeta+"/datosjuntos_train.dat").c_str());
    system(("paste TempXS TempYS > ./"+carpeta+"/datosjuntos_try.dat").c_str());
    system(("cat ./"+carpeta+"/datosjuntos_train.dat ./"+carpeta+"/datosjuntos_try.dat > ./"+carpeta+"/datosjuntos.dat").c_str());
    system("rm TempX TempY TempXS TempYS");
    //Datos para entrenar
    cout<<"¿Cuantas iteraciones?"<<endl;cin>>i;
    cout<<"¿Desea chequear los datos? (s o n)"<<endl;cin>>over;
    if(over=='s'){
      check=true;
      cout<<"¿Cada cuantas iteraciones?"<<endl;cin>>j;
    }
    //Entreno considerando overfitting
    entrena_guarda_overf(malla,x,y,xs,ys,atof(argv[3]),carpeta,i,check,j,normal);
    //si deseo guardar la red
    cout<<"¿Guardar la red? (s o n)"<<endl;cin>>over;
    if(over=='s') guarda_red("./"+carpeta+"/red.nn",malla);
    cout<<"Listo!, todo guardado en "+carpeta<<endl;
    return 0;
  }
  //Si nada calza
  else {cout<<"Opción no existente, se saldrá del programa"<<endl;return 0;}
  return 0;
}
