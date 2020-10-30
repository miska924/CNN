#include "Paint.cpp"

Image::Image(const std::string& path) {
    read(path);
}

Image::Image(int n, int m, int p, const std::vector<int>& color) {
    std::vector<int> vec = color;
    vec.resize(p);
    a.assign(n, std::vector<std::vector<int> >(m, vec));
}

Image::~Image() {
    a.clear();
}

void Image::read(const std::string& path) {
    boost::gil::rgb8_image_t img;
    //boost::gil::jpeg_read_image(path.c_str(), img);
    boost::gil::read_image(path.c_str(), img, boost::gil::jpeg_tag());
    a.assign(img.height(), std::vector<std::vector<int> >(img.width(), std::vector<int>(3, 0)));
    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            a[j][i][0] = int(boost::gil::semantic_at_c<0>(*(view(img).at(i, j))));
            a[j][i][1] = int(boost::gil::semantic_at_c<1>(*(view(img).at(i, j))));
            a[j][i][2] = int(boost::gil::semantic_at_c<2>(*(view(img).at(i, j))));
        }
    }
}

void Image::write(const std::string& path) const {
    boost::gil::rgb8_image_t img;
    if (a.empty()) {
        img = boost::gil::rgb8_image_t(0, 0);
    } else {
        img = boost::gil::rgb8_image_t(a[0].size(), a.size());
        for (int i = 0; i < img.width(); ++i) {
            for (int j = 0; j < img.height(); ++j) {
                assert(p() == 3 || p() == 1);
                if (p() == 3) {
                    boost::gil::semantic_at_c<0>(*(view(img).at(i, j))) = a[j][i][0];
                    boost::gil::semantic_at_c<1>(*(view(img).at(i, j))) = a[j][i][1];
                    boost::gil::semantic_at_c<2>(*(view(img).at(i, j))) = a[j][i][2];
                } else {
                    boost::gil::semantic_at_c<0>(*(view(img).at(i, j))) = a[j][i][0];
                    boost::gil::semantic_at_c<1>(*(view(img).at(i, j))) = a[j][i][0];
                    boost::gil::semantic_at_c<2>(*(view(img).at(i, j))) = a[j][i][0];
                }
            }
        }
    }
    //boost::gil::jpeg_write_view(path.c_str(), view(img));
    boost::gil::write_view(path.c_str(), view(img), boost::gil::jpeg_tag());
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

std::vector<std::vector<int> >& Image::operator[](int x) {
    return a[x];
}

const std::vector<std::vector<int> >& Image::operator[](int x) const {
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
    int mn = IMAGETOOL_INF, mx = -IMAGETOOL_INF;
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            for (int k = 0; k < p(); ++k) {
                mn = std::min(mn, a[i][j][k]);
                mx = std::max(mx, a[i][j][k]);
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
            std::vector<int> mx(p(), -IMAGETOOL_INF);
            for (int di = 0; di < d; ++di) {
                for (int dj = 0; dj < d; ++dj) {
                    if (i * d + di < n() && j * d + dj < m()) {
                        for (int k = 0; k < p(); ++k) {
                            mx[k] = std::max(mx[k], a[i * d + di][j * d + dj][k]);
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
            a[i][j] = std::vector<int>(1, sum);
        }
    }
}

std::vector<int> Image::vec() {
    mono();
    std::vector<int> res;
    for (int i = 0; i < n(); ++i) {
        for (int j = 0; j < m(); ++j) {
            res.push_back(a[i][j][0]);
        }
    }
    return res;
}
