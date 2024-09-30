#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

const tuple<float, float, float, float> oktaeder(const int& a)
{
	float V=pow(a,3)*sqrt(2)/3;
	float A=2*pow(a,2)*sqrt(3);
	float ri=a*sqrt(6)/6;
	float ru=a*sqrt(2)/2;

  return make_tuple(V, A, ri, ru);
}

const string formatResults(const tuple<float, float, float, float>& providedResults)
{
  stringstream output;
  output << "Volumen:" << get<0>(providedResults)
  	<< " Oberfläche:"<< get<1>(providedResults)
	  << " Inkreisradius:" << get<2>(providedResults)
	  << " Umkreisradius:" << get<3>(providedResults) << endl;
  return output.str();
}

int main(int argc, char *argv[])
{
  int a = 2;
  cout << formatResults(oktaeder(a));
  return 0;
}