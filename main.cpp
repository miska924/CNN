#include <bits/stdc++.h>
#include "CNNTool.h"

vector<pair<Image, vector<double> > > vec;
vector<int> who;

inline void get(string s) {
    for (int test = 0;; ++test) {
        ifstream check(string("tests/") + s + "/" + to_string(test) + ".jpg");
        if (!check) {
            check.close();
            break;
        }
        check.close();
        cerr << s << ": " << test << endl;
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
        who.push_back(who.size());
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
    net.write("CNNtemp");
    cerr << net.arch() << endl;
    get("one");
    get("zero");
    cout << "number of tests: " << vec.size() << endl;

    // for (int i = 0; i < int(vec.size()); ++i) {
    //     vec[i].first.show();
    // }
    random_shuffle(who.begin(), who.end());
    pair<Image, vector<double> > last;
    int loop = 0;
    while (true) {
        vector<double> res;
        int cnt = 0;
        while (true) {
            cnt++;
            double mx = 0, sum = 0;
            random_shuffle(who.begin(), who.begin() + loop);
            for (int i = 0; i < loop; ++i) {
                res = net.run(vec[who[i]].first);
                double err = abs(res[0] - vec[who[i]].second[0]) +
                             abs(res[1] - vec[who[i]].second[1]);
                mx = max(mx, err);
                sum += err;
            }
            if (mx > 0.1) {
                for (int i = 0; i < loop; ++i) {
                    if (i % 10 == 0) {
                        net.teach(last.first, last.second, 0.01);
                    }
                    res = net.run(vec[who[i]].first);
                    double err = abs(res[0] - vec[who[i]].second[0]) +
                                 abs(res[1] - vec[who[i]].second[1]);
                    if (err > 0.1) {
                        net.teach(vec[who[i]].first, vec[who[i]].second, 0.01);
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
        net.write("CNNtemp");
        if (loop == int(vec.size())) {
            Image im;
            im.paint();

            res = net.run(im);
            cout << int(res[1] > res[0])
                 << " precision: " << abs(res[0] - res[1]) << endl;
            int got;
            cin >> got;
            if (got == -2) {
                break;
            } else if (got == -1) {
                continue;
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
            who.push_back(who.size());
            put();
        }
        last = vec[who[loop]];
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
