#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

const tuple<float, float, float, float> tetraeder(const int& a)
{
	float V=pow(a,3)*sqrt(2)/12;
	float A=pow(a,2)*sqrt(3);
	float ri=a*sqrt(6)/12;
	float ru=a*sqrt(6)/4;

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
  cout << formatResults(tetraeder(a));
  return 0;
}