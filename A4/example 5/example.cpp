#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

auto ikosaeder = [](const int a)
{
    float V = 5 * pow(a, 3) * (3 + sqrt(5)) / 12;
    float A = 5 * pow(a, 2) * sqrt(3);
    float ri = a * sqrt(3) * (3 + sqrt(5)) / 12;
    float ru = a / 4.0 * sqrt(2 * (5 + sqrt(5)));

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
    auto result = ikosaeder(a);
    cout << formatResults(result);

    return 0;
}
