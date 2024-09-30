#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

auto dodekaeder = [](const int a)
{
    float V = pow(a, 3) * (15 + 7 * sqrt(5)) / 4.0;
    float A = 3 * pow(a, 2) * sqrt(5 * (5 + 2 * sqrt(5)));
    float ri = a * sqrt(10 * (25 + 11 * sqrt(5))) / 20.0;
    float ru = a * sqrt(3) * (1 + sqrt(5)) / 4.0;

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
    auto result = dodekaeder(a);
    cout << formatResults(result);

    return 0;
}
