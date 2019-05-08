Tenzor::Tenzor(int n, int m, int p) {
    a.assign(n, vector<vector<int> >(m, vector<int>(p)));
}

Tenzor::Tenzor(const string& path) {
    (*this).read(path);
}

Tenzor::~Tenzor() {
    a.clear();
}

void Tenzor::read(const string& path) {
    ifstream in(path.c_str());
    int x, y, z;
    in >> x >> y >> z;
    a.assign(x, vector<vector<int> >(y, vector<int>(z)));
    for (int k = 0; k < z; ++k) {
        for (int i = 0; i < x; ++i) {
            for (int j = 0; j < y; ++j) {
                in >> a[i][j][k];
            }
        }
    }
    in.close();
}

void Tenzor::write(const string& path) const {
    ofstream out(path.c_str());
    out << n() << " " << m() << " " << p() << "\n";
    for (int k = 0; k < p(); ++k) {
        out << "\n";
        for (int i = 0; i < n(); ++i) {
            for (int j = 0; j < m(); ++j) {
                out << a[i][j][k] << " ";
            }
            out << "\n";
        }
    }
    out.flush();
    out.close();
}

int Tenzor::n() const {
    return a.size();
}

int Tenzor::m() const {
    if (a.empty()) {
        return 0;
    }
    return a[0].size();
}

int Tenzor::p() const {
    if (a.empty() || a[0].empty()) {
        return 0;
    }
    return a[0][0].size();
}

vector<vector<int> >& Tenzor::operator[](int x) {
    return a[x];
}

const vector<vector<int> >& Tenzor::operator[](int x) const {
    return a[x];
}
