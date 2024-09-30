#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

const tuple<float, float, float, float> ikosaeder(const int& a)
{
	float V=5*pow(a,3)*(3+sqrt(5))/12;
	float A=5*pow(a,2)*sqrt(3);
	float ri=a*sqrt(3)*(3+sqrt(5))/12;
	float ru=a/4.0*sqrt(2*(5+sqrt(5)));

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
  cout << formatResults(ikosaeder(a));
  return 0;
}