#include <bits/stdc++.h>
#include "PerceptronTool.h"

using namespace std;

int main() {
    Perceptron net(vector<int>{3, 10, 3});
    while (true) {
        vector<double> vec(3), need(3);
        cout << " in:   ";
        for (int i = 0; i < 3; ++i) {
            cin >> vec[i];
            need[i] = vec[i] / 10;
        }
        vector<double> res = net.run(vec);
        cout << "out:   ";
        for (int i = 0; i < 3; ++i) {
            cout << res[i] * 10;
            if (i < 2) cout << " ";
            else       cout << endl;
        }
        while()
        net.teach(vec, need);
    }
    return 0;
}