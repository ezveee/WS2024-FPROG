#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

auto wuerfel = [](int a)
{
    int V = pow(a, 3);
    int A = 6 * pow(a, 2);
    float d = a * sqrt(3);
    float ri = a / 2.0;
    float ru = a / 2.0 * sqrt(3);

    return make_tuple(V, A, d, ri, ru);
};

auto formatResults = [](const tuple<int, int, float, float, float>& results)
{
    stringstream output;
    output << "Volumen:" << get<0>(results)
           << " Oberfläche:" << get<1>(results)
           << " Diagonale:" << get<2>(results)
           << " Inkreisradius:" << get<3>(results)
           << " Umkreisradius:" << get<4>(results) << endl;
    return output.str();
};

int main(int argc, char *argv[])
{
    int a = 2;
    auto result = wuerfel(a);
    cout << formatResults(result);

    return 0;
}
