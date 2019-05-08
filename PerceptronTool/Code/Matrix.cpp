Matrix::Matrix() {
    n = m = 0;
    vec.clear();
}

Matrix::Matrix(int n_ ) {
    n = m = n_;
    vec.assign(n, vector<double>(n, 0));
    for (int i = 0; i < n; ++i) {
        vec[i][i] = 1;
    }
}

Matrix::Matrix(int n_, int m_, bool rnd) {
    n = n_;
    m = m_;
    vec.assign(n, vector<double>(m, 0));
    if (rnd) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                vec[i][j] = double(rand() % 20000 - 10000) / 1000;
            }
        }
    }
}

Matrix::Matrix(const vector<double>& v) {
    n = 1;
    m = v.size();
    vec.resize(1, vector<double>(0));
    vec[0] = v;
}

// int Matrix::n() const {
//     return vec.size();
// }

// int Matrix::m() const {
//     if (!vec.empty()) {
//         return vec[0].size();
//     }
//     return 0;
// }

Matrix::Matrix(const vector<vector<double> >& v) {
    if (v.empty()) {
        n = m = 0;
        vec.clear();
    }
    n = v.size();
    m = v[0].size();
    vec.resize(v.size(), vector<double>(0));
    for (int i = 0; i < int(v.size()); ++i) {
        vec[i] = v[i];
    }
}

Matrix::~Matrix() {
    vec.clear();
    n = m = 0;
}

vector<double>& Matrix::operator[](int x) {
    assert(x < int(vec.size()));
    return vec[x];
}

const vector<double>& Matrix::operator[](int x) const {
    assert(x < int(vec.size()));
    return vec[x];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(m == a.n);
    Matrix res(n, a.m);
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < a.m; ++k) {
                res[i][k] += vec[i][j] * a[j][k];
            }
        }
    }
    return res;
}

Matrix Matrix::operator*=(const Matrix& a) {
    return (*this) = (*this) * a;
}

Matrix Matrix::operator^(int a) {
    assert(n == m);
    if (a == 0) {
        return Matrix(n);
    }
    Matrix res = (*this) ^ (a / 2);
    res *= res;
    if (a & 1) {
        res *= *this;
    }
    return res;
}

Matrix Matrix::operator^=(int a) {
    return (*this) = (*this) ^ a;
}

istream& operator>>(istream& in, Matrix& a) {
    for (int i = 0; i < a.n; ++i) {
        for (int j = 0; j < a.m; ++j) {
            if (!(in >> a[i][j])) {
                a[i][j] = double(rand() % 1000) / 500 - 1;
            }
        }
    }
    return in;
}

ostream& operator<<(ostream& out, const Matrix& a) {
    for (int i = 0; i < a.n; ++i) {
        for (int j = 0; j < a.m; ++j) {
            out << a[i][j] << " ";
        }
        out << '\n';
    }
    out.flush();
    return out;
}