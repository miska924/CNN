Scalp::Scalp(int h, int w, bool rnd) {
    a.assign(h, vector<double>(w));
    if (rnd) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                a[i][j] = double(rand() % 20000 - 10000) / 10000;
            }
        }
    }
}

Scalp::~Scalp() {
    a.clear();
}

vector<double>& Scalp::operator[](int x) {
    return a[x];
}

const vector<double>& Scalp::operator[](int x) const {
    return a[x];
}

int Scalp::h() const {
    return a.size();
}

int Scalp::w() const {
    if (a.empty()) {
        return 0;
    }
    return a[0].size();
}

void Scalp::read(ifstream& in) {
    int n, m;
    in >> n >> m;
    a.assign(n, vector<double>(m));
    for (int i = 0; i < h(); ++i) {
        for (int j = 0; j < w(); ++j) {
            in >> a[i][j];
        }
    }
}

void Scalp::write(ofstream& out) {
    out << "   " << h() << " " << w() << "\n";
    for (int i = 0; i < h(); ++i) {
        out << "    ";
        for (int j = 0; j < w(); ++j) {
            if (a[i][j] > 0) {
                out << "+";
            } else {
                out << "-";
            }
            out << fixed << setprecision(7) << abs(a[i][j]) << " ";
        }
        out << "\n";
    }
}