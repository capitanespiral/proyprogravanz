#include "neural.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <dirent.h>


int main(int argc,char* argv[]){
  ifstream archivix(argv[1]);
  vector<double> v,w;double x;
  while(archivix>>x)
    v.push_back(x);
  archivix.close();
  ifstream archiviy(argv[2]);
  while(archiviy>>x)
    w.push_back(x);
  archiviy.close();
  matriz<double> xs(v,2);
  matriz<double> ys(w,1);
  vector<int> h={2,4,6,1};
  matriz<int> soonred(h,1);
  matriz<neural_l> malla=red(soonred,sigm,dsigm);
  matriz<double> res,ver,check,res_check;double error;
  int i=0;
  while(true){
    DIR* dir = opendir(("evolucion_red_"+to_string(i)).c_str());
    if(dir) {++i;closedir(dir);continue;}
    else break;
  }
  string carpeta="evolucion_red_"+to_string(i);
  string xx=argv[1];string yy=argv[2];
  system(("mkdir "+carpeta).c_str());
  system(("paste "+xx+" "+yy+"> ./"+carpeta+"/datosjuntos_"+to_string(i)+".dat").c_str());
  ofstream archivo("./"+carpeta+"/errorcito_"+to_string(i)+".dat");
  for(int i=0;i<=2500;++i){
    if(i%25==0){
      check=mat_rand(1000,2,-1,1);
      res_check=forward(malla,check);
      string a=to_string(i);
      if(i<10) a="000"+a;
      else if(10<=i && i<100) a="00"+a;
      else if(100<=i && i<1000) a="0"+a;
      ofstream archivote("./"+carpeta+"/Estado_"+a+".dat");
      for(int j=0;j<check.fila();++j){
	archivote<<check(j,0)<<' '<<check(j,1)<<' '<<res_check(j,0)<<endl;
      }
      archivote.close();
    }
    res=train(malla,xs,ys,d_e_cuad_m,atof(argv[3]));
    if(i%100==0){
      ver=cat(ys,res);
      cout<<ver<<endl;
    }
    error=e_cuad_m(res,ys);
    archivo<<i<<' '<<error<<endl;
  }
  archivo.close();
  return 0;
}
