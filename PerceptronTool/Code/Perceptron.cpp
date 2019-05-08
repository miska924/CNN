Perceptron::Perceptron() {
    vec.clear();
    in.clear();
    out.clear();
    delta.clear();
}

Perceptron::Perceptron(ifstream& in) {
    (*this).read(in);
}

Perceptron::Perceptron(const vector<int>& pt, bool rnd) {
    vec.assign(int(pt.size()) - 1, Matrix());
    for (int i = 0; i < int(pt.size()) - 1; ++i) {
        vec[i] = Matrix(pt[i], pt[i + 1], rnd);
    }
    in.assign(pt.size(), vector<double>(0));
    out.assign(pt.size(), vector<double>(0));
    delta.assign(pt.size(), vector<double>(0));
    for (int i = 0; i < int(pt.size()); ++i) {
        in[i] = out[i] = delta[i] = vector<double>(pt[i]);
    }
}

Perceptron::Perceptron(const vector<Matrix>& a) {
    in.clear();
    out.clear();
    delta.clear();
    vec = a;
    for (int i = 0; i < int(a.size()); ++i) {
        in.push_back(vector<double>(a[i].n));
        out.push_back(vector<double>(a[i].n));
        delta.push_back(vector<double>(a[i].n));
    }
    if (!a.empty()) {
        in.push_back(vector<double>(a.back().m));
        out.push_back(vector<double>(a.back().m));
        delta.push_back(vector<double>(a.back().m));
    }
}

vector<double> Perceptron::run(const vector<double>& input) {
    Matrix a(input);
    in[0] = input;
    for (int i = 0; i < a.n; ++i) {
        for (int j = 0; j < a.m; ++j) {
            a[i][j] = f(a[i][j]);
        }
    }
    out[0] = a[0];
    for (int i = 0; i < int(vec.size()); ++i) {
        a *= vec[i];
        in[i + 1] = a[0];
        for (int i = 0; i < a.n; ++i) {
            for (int j = 0; j < a.m; ++j) {
                a[i][j] = f(a[i][j]);
            }
        }
        out[i + 1] = a[0];
    }
    return a[0];
}

string Perceptron::arch() const {
    string s = "{";
    for (int i = 0; i < int(vec.size()); ++i) {
        s += to_string(vec[i].n) + ", ";
    }
    s += to_string(vec.back().m) + "}";
    return s;
}

double difference(const vector<double>& a, const vector<double>& b) {
    assert(a.size() == b.size());
    double res = 0;
    for (int i = 0; i < int(a.size()); ++i) {
        res += abs(a[i] - b[i]); 
    }
    return res;
}

void Perceptron::change(double n) {
    for (int i = int(vec.size()) - 1; i >= 0; --i) {
        for (int j = 0; j < vec[i].n; ++j) {
            for (int k = 0; k < vec[i].m; ++k) {
                vec[i][j][k] += delta[i + 1][k] * n * out[i][j];
            }
        }
        for (int j = 0; j < vec[i].n; ++j) {
            double diff = 0;
            for (int k = 0; k < vec[i].m; ++k) {
                diff += delta[i + 1][k] * vec[i][j][k];
            }
            // if (false && i == 0) {
            //     delta[i][j] = diff * double(int(in[i][j] > 0));
            // } else {
                delta[i][j] = diff * fs(in[i][j]);
            // }
        }
    }
}

void Perceptron::teach(const vector<double>& input, const vector<double>& correct, double n, int steps) {
    for (int step = 0; step < steps; step++) {
        vector<double> output = run(input);
        for (int j = 0; j < int(output.size()); ++j) {
            delta.back()[j] = (correct[j] - output[j]) * fs(in.back()[j]);
        }
        change(n);
    }
}

vector<double> Perceptron::d() {
    return delta[0];
}

size_t Perceptron::szin() const {
    return vec[0].n;
}

size_t Perceptron::szout() const {
    return vec.back().m;
}

size_t Perceptron::size() const {
    return vec.size() + 1;
}

double Perceptron::f(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double Perceptron::fs(double x) {
    return f(x) * (1.0 - f(x));
}

Matrix& Perceptron::operator[](int x) {
    assert(x < int(vec.size()));
    return vec[x];
}

const Matrix& Perceptron::operator[](int x) const {
    assert(x < int(vec.size()));
    return vec[x];
}

void Perceptron::read(ifstream& in) {
    int n;
    in >> n;
    vector<int> sz(n);
    for (int i = 0; i < n; ++i) {
        in >> sz[i];
    }
    vector<Matrix> vec(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        vec[i] = Matrix(sz[i], sz[i + 1]);
        in >> vec[i];
    }
    (*this) = Perceptron(vec);
}

void Perceptron::write(ofstream& out) {
    int n = size();
    out << n << "\n";
    for (int i = 0; i < n - 1; ++i) {
        out << vec[i].n << " ";
    }
    out << vec[n - 2].m << "\n";
    for (int i = 0; i < n - 1; ++i) {
        out << vec[i] << "\n";
    }
    out.flush();
}