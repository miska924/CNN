const double CNN_INF = 1e18;

Layer::Layer(const Image& im) {
    a.assign(3, Scalp(im.n(), im.m()));
    for (int k = 0; k < 3; ++k) {
        for (int i = 0; i < h(); ++i) {
            for (int j = 0; j < w(); ++j) {
                a[k][i][j] = im[i][j][k];
            }
        }
    }
}

Layer::Layer(int sz, int h, int w, bool rnd) {
    a.assign(sz, Scalp(h, w, rnd));
    for (int i = 0; i < sz; ++i) {
        a[i] = Scalp(h, w, rnd);
    }
}

Layer::~Layer() {
    a.clear();
}

int Layer::size() const {
    return a.size();
}

int Layer::h() const {
    if (!a.empty()) {
        return a[0].h();
    }
    return 0;
}

int Layer::w() const {
    if (!a.empty()) {
        return a[0].w();
    }
    return 0;
}

Scalp& Layer::operator[](int x) {
    return a[x];
}

const Scalp& Layer::operator[](int x) const{
    return a[x];
}

void Layer::transform(const Layer& lay) {
    assert(lay.size() == size());
    Layer res(1, h(), w());
    for (int k = 0; k < size(); ++k) {
        for (int i = 0; i < h(); ++i) {
            for (int j = 0; j < w(); ++j) {
                for (int di = (-lay.h() / 2); di < (-lay.h() / 2 + lay.h()); ++di) {
                    for (int dj = (-lay.w() / 2); dj < (-lay.w() / 2 + lay.w()); ++dj) {
                        if ((0 <= i + di && i + di < h()) && (0 <= j + dj && j + dj < w())) {
                            res[0][i][j] += a[k][i + di][j + dj] * lay[k][lay.h() / 2 + di][lay.w() / 2 + dj];
                        } else {
                            res[0][i][j] += a[k][i][j] * lay[k][lay.h() / 2 + di][lay.w() / 2 + dj];
                        }
                    }
                }
            }
        }
    }
    (*this) = res;
}

void Layer::transform(const Cluster& cl) {
    Layer res(cl.size(), h(), w());
    for (int i = 0; i < cl.size(); ++i) {
        Layer cur = (*this);
        cur.transform(cl[i]);
        res[i] = cur[0];
    }
    (*this) = res;
}

std::vector<Layer> Layer::pool(bool plg) {
    std::vector<Layer> pl;
    if (plg) {
        pl = std::vector<Layer>(2, Layer(size(), (h() + 1) / 2, (w() + 1) / 2));
        Layer res(size(), (h() + 1) / 2, (w() + 1) / 2);
        for (int k = 0; k < res.size(); ++k) {
            for (int i = 0; i < res.h(); ++i) {
                for (int j = 0; j < res.w(); ++j) {
                    res[k][i][j] = -CNN_INF;
                    for (int x = 0; x < 2; ++x) {
                        for (int y = 0; y < 2; ++y) {
                            if (i * 2 + x < h() && j * 2 + y < w()) {
                                if (res[k][i][j] < a[k][i * 2 + x][j * 2 + y]) {
                                    res[k][i][j] = a[k][i * 2 + x][j * 2 + y];
                                    pl[0][k][i][j] = i * 2 + x;
                                    pl[1][k][i][j] = j * 2 + y;
                                }
                            }
                        }
                    }
                }
            }
        }
        (*this) = res;
    } else {
        pl = std::vector<Layer>(2, Layer(size(), h(), w()));
        for (int k = 0; k < size(); ++k) {
            for (int i = 0; i < h(); ++i) {
                for (int j = 0; j < w(); ++j) {
                    pl[0][k][i][j] = i;
                    pl[1][k][i][j] = j;
                }
            }
        }
    }
    return pl;
}

std::vector<double> Layer::vec() {
    std::vector<double> res;
    for (int k = 0; k < size(); ++k) {
        for (int i = 0; i < h(); ++i) {
            for (int j = 0; j < w(); ++j) {
                res.push_back(a[k][i][j]);
            }
        }
    }
    return res;
}

void Layer::fill(const std::vector<double>& vec) {
    assert(int(vec.size()) == size() * h() * w());
    for (int i = 0; i < int(vec.size()); ++i) {
        a[i / w() / h()][i / w() % h()][i % w()] = vec[i];
    }
}

void Layer::apply(double (*f)(double)) {
    for (int k = 0; k < size(); ++k) {
        for (int i = 0; i < h(); ++i) {
            for (int j = 0; j < w(); ++j) {
                a[k][i][j] = f(a[k][i][j]);
            }
        }
    }
}

void Layer::read(std::ifstream& in) {
    int sz;
    in >> sz;
    a.assign(sz, Scalp());
    for (int i = 0; i < size(); ++i) {
        a[i].read(in);
    }
}

void Layer::write(std::ofstream& out) {
    out << "  " << size() << "\n";
    for (int i = 0; i < size(); ++i) {
        a[i].write(out);
    }
}
