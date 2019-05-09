#include <bits/stdc++.h>
#include "CNNTool.h"

vector<pair<Image, vector<double> > > vec;

inline void get(string s) {
    for (int test = 0;; ++test) {
        cerr << s << ": " << test << endl;
        ifstream check(string("tests/") + s + "/" + to_string(test) + ".jpg");
        if (!check) {
            check.close();
            break;
        }
        check.close();
        Image im(string("tests/") + s + "/" + to_string(test) + ".jpg");
        vec.push_back(
            pair<Image, vector<double> > {
                im,
                vector<double> {
                    double(int(s == "zero")),
                    double(int(s == "one"))
                }
            }
        );
    }
}

void put() {
    int zero, one, none;
    zero = one = none = 0;
    for (int i = 0; i < int(vec.size()); ++i) {
        string s;
        if (vec[i].second[0] > 0.5) {
            vec[i].first.write(string("tests/zero/") +
                            to_string(zero++) + ".jpg");
        } else if (vec[i].second[1] > 0.5) {
            vec[i].first.write(string("tests/one/" ) +
                            to_string(one++ ) + ".jpg");
        } else {
            vec[i].first.write(string("tests/none/") +
                            to_string(none++) + ".jpg");
        }
    }
}

inline int teach(string netfile) {
    srand(time(NULL));
    CNN net(netfile);
    cerr << net.arch() << endl;
    get("one");
    get("zero");
    cout << "number of tests: " << vec.size() << endl;

    // for (int i = 0; i < int(vec.size()); ++i) {
    //     vec[i].first.show();
    // }
    // random_shuffle(vec.begin(), vec.end());
    pair<Image, vector<double> > last;
    int loop = 0;
    while (true) {
        vector<double> res;
        int cnt = 0;
        while (true) {
            cnt++;
            double mx = 0, sum = 0;
            // random_shuffle(vec.begin(), vec.begin() + loop);
            for (int i = 0; i < loop; ++i) {
                res = net.run(vec[i].first);
                double err = abs(res[0] - vec[i].second[0]) +
                             abs(res[1] - vec[i].second[1]);
                mx = max(mx, err);
                sum += err;
            }
            if (mx > 0.1) {
                for (int i = 0; i < loop; ++i) {
                    if (i % 10 == 0) {
                        net.teach(last.first, last.second, 0.01);
                    }
                    res = net.run(vec[i].first);
                    double err = abs(res[0] - vec[i].second[0]) +
                                 abs(res[1] - vec[i].second[1]);
                    if (err > 0.1) {
                        net.teach(vec[i].first, vec[i].second, 0.01);
                    }
                }
                if (cnt % 10 == 0) {
                    cerr << fixed << setprecision(7)
                         << "\n#" << loop << "." << cnt
                         << "\nmax error    : " << mx
                         << "\naverage error: " << sum / (loop) << endl;
                }
            } else {
                break;
            }
        }
        net.write("CNNswap");
        if (loop == int(vec.size())) {
            Image im;
            im.paint();

            res = net.run(im);
            cout << int(res[1] > res[0])
                 << " precision: " << abs(res[0] - res[1]) << endl;
            int got;
            cin >> got;
            if (got == 10) {
                break;
            }
            vec.push_back(
                pair<Image, vector<double> > {
                    im,
                    vector<double> {
                        double(int(got == 0)),
                        double(int(got == 1))
                    }
                }
            );
            put();
        }
        last = vec[loop];
        loop = min(loop + 1, int(vec.size()));
    }
    net.write(netfile);
    return 0;
}

void run(string netfile) {
    CNN net(netfile);
    cout << "0-1 DETECTOR\narch: " << net.arch() << endl;
    while (true) {
        Image im;
        im.paint();
        vector<double> res = net.run(im);
        cout << int(res[1] > res[0])
             << " precision: " << abs(res[0] - res[1]) << endl;
    }
}

int main() {
    // teach("CNN_01_boosted");
    run("CNN_01_boosted");
    return 0;
}
