#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

auto tetraeder = [](int a)
{
    float V = pow(a, 3) * sqrt(2) / 12;
    float A = pow(a, 2) * sqrt(3);
    float ri = a * sqrt(6) / 12;
    float ru = a * sqrt(6) / 4;

    return make_tuple(V, A, ri, ru);
};

auto formatResults = [](const tuple<float, float, float, float>& results)
{
    stringstream output;
    output << "Volumen:" << get<0>(results)
           << " Oberfläche:" << get<1>(results)
           << " Inkreisradius:" << get<2>(results)
           << " Umkreisradius:" << get<3>(results) << endl;
    return output.str();
};

int main(int argc, char *argv[])
{
    int a = 2;
    auto result = tetraeder(a);
    cout << formatResults(result);

    return 0;
}
