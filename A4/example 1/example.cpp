#include <iostream>
#include <cmath>
#include <tuple>
#include <sstream>

using namespace std;

auto curriedQuader = [](int a)
{
    return [a](int b)
    {
        return [a, b](float c)
        {
            int V = a * b * c;
            int A = 2 * (a * b + a * c + b * c);
            float d = sqrt(pow(a, 2) + pow(b, 2) + pow(c, 2));
            return make_tuple(V, A, d);
        };
    };
};

auto formatResults = [](const tuple<int, int, float>& results)
{
    stringstream output;
    output << "Volumen:" << get<0>(results)
           << " Oberfläche:" << get<1>(results)
           << " Diagonale:" << get<2>(results) << endl;
    return output.str();
};

int main(int argc, char *argv[])
{
    int a = 2;
    int b = 3;
    float c = 4.0f;
    
    // auto quader = curriedQuader(a);
    // auto result = quader(b)(c);
    auto result = curriedQuader(a)(b)(c);
    cout << formatResults(result);
    
    return 0;
}
