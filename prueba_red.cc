#include "neural.hh"
#include <cmath>


int main(int argc,char* argv[]){
  double x;
  matriz<double> xs=rec_archivo(string(argv[1]),2,x);
  matriz<double> ys=rec_archivo(string(argv[2]),1,x);
  revuelve(xs,ys);
  matriz<double> X,Y;
  separa(xs,ys,X,Y);
  vector<int> h={2,4,6,1};
  matriz<int> soonred(h,1);
  matriz<neural_l> malla=red(soonred,sigm,dsigm);
  matriz<double> res,res_,ver,check,res_check;double error,error_;
  check=matriz_cuad(-1,1,0.05);
  int i;
  string xx=argv[1];string yy=argv[2];
  string carpeta=crea_carpeta("evolucion_red",i);
  //system(("mkdir "+carpeta).c_str());
  system(("paste "+xx+" "+yy+"> ./"+carpeta+"/datosjuntos_"+to_string(i)+".dat").c_str());//Falta arreglar esto sobre el overfitting
  ofstream archivo("./"+carpeta+"/errorcito_train_"+to_string(i)+".dat");
  ofstream archivo_("./"+carpeta+"/errorcito_try_"+to_string(i)+".dat");
  for(int i=0;i<=3000;++i){
    if(i%25==0){
      //check=mat_rand(1000,2,-1,1); probemos con una NO random
      res_check=forward(malla,check);
      string a=to_string(i);
      if(i<10) a="000"+a;
      else if(10<=i && i<100) a="00"+a;
      else if(100<=i && i<1000) a="0"+a;
      ofstream archivote("./"+carpeta+"/Estado_"+a+".dat");
      for(int j=0;j<check.fila();++j){
	archivote<<check(j,0)<<' '<<check(j,1)<<' '<<res_check(j,0)<<endl;
	if(check(j+1,0)!=check(j,0)) archivote<<endl;
      }
      archivote.close();
    }
    res=train(malla,xs,ys,d_e_cuad_m,atof(argv[3]));
    res_=forward(malla,X);
    if(i%100==0){
      ver=cat(ys,res);
      cout<<ver<<endl;
    }
    error=e_cuad_m(res,ys);
    error_=e_cuad_m(res_,Y);
    archivo<<i<<' '<<error<<endl;
    archivo_<<i<<' '<<error_<<endl;
  }
  archivo.close();
  archivo_.close();
  return 0;
}
