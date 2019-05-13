CNN::CNN() {
    a.clear();
    p = Perceptron();
}

CNN::~CNN() {
    a.clear();
    p = Perceptron();
}

CNN::CNN(const std::string& path) {
    (*this).read(path);
}

CNN::CNN(const std::vector<int>& cl, std::vector<int> pt) {
    int cur = 3;
    a.assign(cl.size(), Cluster());
    for (int i = 0; i < int(a.size()); ++i) {
        a[i] = Cluster(cl[i], Layer(cur, 3, 3, true));
        for (int j = 0; j < cl[i]; ++j) {
            a[i][j] = Layer(cur, 3, 3, true);
        }
        cur = cl[i];
    }
    reverse(pt.begin(), pt.end());
    clear();
    pt.push_back(out.back().size() * out.back().h() * out.back().w());
    reverse(pt.begin(), pt.end());
    p = Perceptron(pt, true);
}

void CNN::read(const std::string& path) {
    std::   ifstream inp(path.c_str());
    int sz;
    inp >> sz;
    a.assign(sz, Cluster());
    for (int i = 0; i < sz; ++i) {
        a[i].read(inp);
    }
    p.read(inp);
    inp.close();
    clear();
}

void CNN::write(const std::string& path) {
    std::ofstream out(path.c_str());
    out << a.size() << "\n";
    for (int i = 0; i < int(a.size()); ++i) {
        a[i].write(out);
    }
    p.write(out);
    out.close();
}

double CNN_adapting_func(double x) {
    return 1.0 / (1.0 + exp(-x));
    return std::max(0.0, x);
}

double CNN_adapting_funcs(double x) {
    return CNN_adapting_func(x) * (1.0 - CNN_adapting_func(x));
    if (x >= 0)
        return 1;
    else
        return 0;
}

void CNN::clear() {
    in.assign(a.size() + 1, Layer());
    out.assign(a.size() + 1, Layer());
    delta.assign(a.size() + 1, Layer());
    pl.assign(2, std::vector<Layer>(a.size() + 1, Layer()));
    int winsz = 32;
    in[0] = out[0] = delta[0] = Layer(3, winsz, winsz);
    for (int i = 0; i < int(a.size()); ++i) {
        in[i + 1] = delta[i + 1] = Layer(a[i].size(), winsz, winsz);
        if (i < int(a.size()) - 1) {
            winsz /= 2;
        }
        pl[0][i + 1] = pl[1][i + 1] = out[i + 1] = Layer(a[i].size(), winsz, winsz);
    }
}

std::vector<double> CNN::run(const Image& im) {
    Layer lay(im);
    in[0] = lay;
    lay.apply(CNN_adapting_func);
    out[0] = lay;
    for (int i = 0; i < int(a.size()); ++i) {
        lay.transform(a[i]);
        in[i + 1] = lay;
        if (i < int(a.size()) - 1) {
            std::vector<Layer> pp = lay.pool();
            pl[0][i + 1] = pp[0];
            pl[1][i + 1] = pp[1];
            lay.apply(CNN_adapting_func);
        }
        // lay.apply(CNN_adapting_func);
        out[i + 1] = lay;
    }
    return p.run(lay.vec());
}

std::string CNN::arch() const {
    std::string s = "{{";
    for (int i = 0; i < int(a.size()); ++i) {
        s += std::to_string(a[i].size()) + ", ";
    }
    if (a.size()) {
        s.pop_back();
        s.pop_back();
    }
    std::string add = p.arch();
    for (int i = 0; i < int(add.size()); ++i) {
        if (add[i] == ',' || add[i] == '}') {
            add = add.substr(i + 1, int(add.size()) - i - 1);
            break;
        }
    }
    if (add == "") {
        add = "{}";
    } else if (!add.empty() && add[0] == ' ') {
        add = std::string("{") + add.substr(1, int(add.size()) - 1);
    }
    s += "}, " + add + "}";
    return s;
}

void CNN::teach(const Image& im, const std::vector<double>& correct, double n) {
    clear();
    std::vector<double> got = run(im);
    p.teach(out.back().vec(), correct, n, 1);
    delta.back().fill(p.d());
    for (int lay = int(a.size()) - 1; lay >= 0; --lay) {
        for (int k1 = 0; k1 < out[lay].size(); ++k1) {
            for (int i = 0; i < out[lay].h(); ++i) {
                for (int j = 0; j < out[lay].w(); ++j) {
                    for (int k = 0; k < in[lay + 1].size(); ++k) {
                        for (int di = -a[lay][k].h() / 2; di < -a[lay][k].h() / 2 + a[lay][k].h(); ++di) {
                            for (int dj = -a[lay][k].w() / 2; dj < -a[lay][k].w() / 2 + a[lay][k].w(); ++dj) {
                                if ((0 <= i + di && i + di < out[lay].h()) &&
                                    (0 <= j + dj && j + dj < out[lay].w())) {
                                        a[lay][k][k1][a[lay][k].h() / 2 + di][a[lay][k].w() / 2 + dj] +=
                                            delta[lay + 1][k][i][j] * n * out[lay][k1][i + di][j + dj];
                                }
                            }
                        }
                    }
                }
            }
        }
        if (lay > 0) {
            for (int k1 = 0; k1 < out[lay].size(); ++k1) {
                for (int i = 0; i < out[lay].h(); ++i) {
                    for (int j = 0; j < out[lay].w(); ++j) {
                        double diff = 0;
                        for (int k = 0; k < in[lay + 1].size(); ++k) {
                            int begdi = a[lay][k].h() / 2 - a[lay][k].h() + 1;
                            int begdj = a[lay][k].w() / 2 - a[lay][k].w() + 1;
                            for (int di = begdi; di < begdi + a[lay][k].h(); ++di) {
                                for (int dj = begdj; dj < begdj + a[lay][k].w(); ++dj) {
                                    if ((0 <= i + di && i + di < out[lay].h()) &&
                                        (0 <= j + dj && j + dj < out[lay].w())) {
                                            diff += delta[lay + 1][k][i + di][j + dj] *
                                                    a[lay][k][k1][begdi + a[lay][k].h() - di - 1][begdj + a[lay][k].w() - dj - 1];
                                    }
                                }
                            }
                        }
                        int x = int(pl[0][lay][k1][i][j] + 0.5);
                        int y = int(pl[1][lay][k1][i][j] + 0.5);
                        delta[lay][k1][x][y] = diff * CNN_adapting_funcs(in[lay][k1][x][y]);
                    }
                }
            }
        }
    }
}

int CNN::h() const {
    if (!in.empty()) {
        return in[0].h();
    }
    return 0;
}

int CNN::w() const {
    if (!in.empty()) {
        return in[0].w();
    }
    return 0;
}

int CNN::outsz() const {
    return p.szout();
}
