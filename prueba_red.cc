#include "neural.h"
#include <iostream>
#include <cmath>
#include <fstream>

double sigm(double x){
	double a=1./(1+exp(-x));
	return a;
}

double dsigm(double x){
	double a= exp(-x)/((1+exp(-x))*(1+exp(-x)));
	return a;
}

double error(const matriz<double> &y_e,const matriz<double> &y_o){
	matriz<double> res=y_e-y_o;
	double acum=0;
	for(int i=0;i<res.fila();++i){
		for(int j=0;j<res.colu();++j){
			acum+=res(i,j)*res(i,j);
	}
}
	acum/=res.fila();
	return acum;

}

int main(){
  vector<double> v={2,4,5,2,1,-2};
vector<double> w={1,2,3,4,5,6};
matriz<double> h(w,2);
  matriz<double> m(v,2);
  //cout<<m<<endl;
  /*cout<<"yes"<<endl;
  matriz<neural_l> poto=red(m,activa);*/
 // neural_l andale(2,3,activa,activa);
//neural_l poto(3,2,activa1,activa);
  /*cout<<andale<<endl;
cout<<andale[0]<<endl;
	andale(andale[0]);
cout<<andale[0]<<endl;
  cout<<"YES"<<endl;
  cout<<poto<<endl;
	cout<<poto(1,0)[1]<<endl;
poto(1,0)(poto(1,0)[1]);
cout<<poto(1,0)[1]<<endl;
cout<<andale.axon(0)<<endl;
cout<<andale.axon(1)<<endl;*/
//matriz<neural_l> n=andale+poto;
//cout<<n<<endl;
//cout<<forward(n,m)<<endl;
//for(int i=0;i<10;++i){
//matriz<double> a=train(n,m,h,error);
//cout<<a<<endl;}
ofstream archivo("sigmo.dat");
for(double i=-20;i<20;i+=0.1){
	archivo<<i<<' '<<sigm(double(i))<<' '<<dsigm(double(i))<<endl;
}
  return 0;
}
