#include "neural.h"
#include <iostream>


double activa(double x){return x*x;}

int main(){
  vector<int> v={2,4,5,2,1};
  matriz<int> m(v,1);
  cout<<m<<endl;
  cout<<"yes"<<endl;
  matriz<neural_l> poto=red(m,activa);
  neural_l andale(2,3,activa);
  cout<<andale<<endl;
  cout<<"YES"<<endl;
  cout<<poto<<endl;
  return 0;
}
