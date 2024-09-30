#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

const tuple<int, int, float> quader(const int& providedA, const int& providedB, const float& providedC)
{
	int V = providedA * providedB * providedC;
  int A = 2 * (providedA * providedB + providedA * providedC + providedB * providedC);
  float d = sqrt(pow(providedA,2)+pow(providedB,2)+pow(providedC,2));
  
  return make_tuple(V, A, d);
}

const string formatResults(const tuple<int, int, float>& providedResults)
{
  stringstream output;
  output << "Volumen:" << get<0>(providedResults)
    << " Oberfläche:" << get<1>(providedResults)
    << " Diagonale:" << get<2>(providedResults) << endl;
  return output.str();
}

int main(int argc, char *argv[])
{
  int a = 2;
  int b = 3;
  int c = 4;
  cout << formatResults(quader(a, b, c));
  return 0;
}