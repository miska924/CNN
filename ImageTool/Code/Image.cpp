#include "Paint.cpp"

Image::Image(const string& path) {
    read(path);
}

Image::Image(int n, int m, int p, const vector<int>& color) {
    vector<int> vec = color;
    vec.resize(p);
    a.assign(n, vector<vector<int> >(m, vec));
}

Image::~Image() {
    a.clear();
}

void Image::read(const string& path) {
    rgb8_image_t img;
    jpeg_read_image(path.c_str(), img);
    a.assign(img.height(), vector<vector<int> >(img.width(), vector<int>(3, 0)));
    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            a[j][i][0] = int(semantic_at_c<0>(*(view(img).at(i, j))));
            a[j][i][1] = int(semantic_at_c<1>(*(view(img).at(i, j))));
            a[j][i][2] = int(semantic_at_c<2>(*(view(img).at(i, j))));
        }
    }
}

void Image::write(const string& path) const {
    rgb8_image_t img;
    if (a.empty()) {
        img = rgb8_image_t(0, 0);
    } else {
        img = rgb8_image_t(a[0].size(), a.size());
        for (int i = 0; i < img.width(); ++i) {
            for (int j = 0; j < img.height(); ++j) {
                assert(p() == 3 || p() == 1);
                if (p() == 3) {
                    semantic_at_c<0>(*(view(img).at(i, j))) = a[j][i][0];
                    semantic_at_c<1>(*(view(img).at(i, j))) = a[j][i][1];
                    semantic_at_c<2>(*(view(img).at(i, j))) = a[j][i][2];
                } else {
                    semantic_at_c<0>(*(view(img).at(i, j))) = a[j][i][0];
                    semantic_at_c<1>(*(view(img).at(i, j))) = a[j][i][0];
                    semantic_at_c<2>(*(view(img).at(i, j))) = a[j][i][0];
                }
            }
        }
    }
    jpeg_write_view(path.c_str(), view(img));
}

int Image::n() const {
    return a.size();
}

int Image::m() const {
    if (a.empty()) {
        return 0;
    }
    return a[0].size();
}

int Image::p() const {
    if (a.empty() || a[0].empty()) {
        return 0;
    }
    return a[0][0].size();
}

vector<vector<int> >& Image::operator[](int x) {
    return a[x];
}

const vector<vector<int> >& Image::operator[](int x) const {
    return a[x];
}

void Image::filter(const Tenzor& kernel) {
    assert(kernel.p() == p());
    Image res(n(), m(), 1);
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            for (int dx = (-kernel.n() / 2); dx < (-kernel.n() / 2 + kernel.n()); dx++) {
                for (int dy = (-kernel.m() / 2); dy < (-kernel.m() / 2 + kernel.m()); dy++) {
                    for (int k = 0; k < p(); ++k) {
                        if ((i + dx >= 0 && i + dx < n()) && (j + dy >= 0 && j + dy < m())) {
                            res[i][j][0] += kernel[kernel.n() / 2 + dx][kernel.m() / 2 + dy][k] * a[i + dx][j + dy][k];
                        } else {
                            res[i][j][0] += kernel[kernel.n() / 2 + dx][kernel.m() / 2 + dy][k] * a[i][j][k];
                        }
                    }
                }
            }
        }
    }
    (*this) = res;
    normalize();
}

void Image::normalize() {
    int mn = HEAD_INF, mx = -HEAD_INF;
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            for (int k = 0; k < p(); ++k) {
                mn = min(mn, a[i][j][k]);
                mx = max(mx, a[i][j][k]);
            }
        }
    }
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            for (int k = 0; k < p(); ++k) {
                a[i][j][k] = (a[i][j][k] - mn) * (256) / (mx - mn + 1);
            }
        }
    }
}

void Image::pool(int d) {
    for (int i = 0; i * d < n(); ++i) {
        for (int j = 0; j * d < m(); ++j) {
            vector<int> mx(p(), -HEAD_INF);
            for (int di = 0; di < d; ++di) {
                for (int dj = 0; dj < d; ++dj) {
                    if (i * d + di < n() && j * d + dj < m()) {
                        for (int k = 0; k < p(); ++k) {
                            mx[k] = max(mx[k], a[i * d + di][j * d + dj][k]);
                        }
                    }
                }
            }
            a[i][j] = mx;
        }
    }
    a.resize((n() + d - 1) / d);
    for (int i = n() - 1; i >= 0; --i) {
        a[i].resize((m() + d - 1) / d);
    }
}

void Image::mono() {
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            double sum = 0;
            for (int k = 0; k < p(); ++k) {
                sum += a[i][j][k];
            }
            sum /= p();
            a[i][j] = vector<int>(1, sum);
        }
    }
}

vector<int> Image::vec() {
    mono();
    vector<int> res;
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            res.push_back(a[i][j][0]);
        }
    }
    return res;
}