#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;


const tuple<int, int, float, float, float> wuerfel(const int& providedA)
{
	int V = pow(providedA, 3);
	int A = 6*pow(providedA, 2);
	float d = providedA*sqrt(3);
	float ri = providedA/2;
	float ru = providedA/2*sqrt(3);

	return make_tuple(V, A, d, ri, ru);
}

const string formatResults(const tuple<int, int, float, float, float>& providedResults)
{
  stringstream output;
  output << "Volumen:" << get<0>(providedResults)
  	<< " Oberfläche:"<< get<1>(providedResults)
	<< " Diagonale:" << get<2>(providedResults)
	<< " Inkreisradius:" << get<3>(providedResults)
	<< " Umkreisradius:" << get<4>(providedResults) << endl;
  return output.str();
}

int main(int argc, char *argv[])
{
  int a = 2;
  cout << formatResults(wuerfel(a));
  return 0;
}