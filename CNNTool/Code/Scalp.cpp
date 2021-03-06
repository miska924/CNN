Scalp::Scalp(int h, int w, bool rnd) {
    a.assign(h, std::vector<double>(w));
    if (rnd) {
        // std::cout << "." << std::endl;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                a[i][j] = double(rand() % 200 - 100) / 100.0;
            }
        }
    }
}

Scalp::~Scalp() {
    a.clear();
}

std::vector<double>& Scalp::operator[](int x) {
    return a[x];
}

const std::vector<double>& Scalp::operator[](int x) const {
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

void Scalp::read(std::ifstream& in) {
    int n, m;
    in >> n >> m;
    a.assign(n, std::vector<double>(m));
    for (int i = 0; i < h(); ++i) {
        for (int j = 0; j < w(); ++j) {
            in >> a[i][j];
        }
    }
}

void Scalp::write(std::ofstream& out) {
    out << "   " << h() << " " << w() << "\n";
    for (int i = 0; i < h(); ++i) {
        out << "    ";
        for (int j = 0; j < w(); ++j) {
            out << std::setprecision(7) << std::fixed << a[i][j] << " ";
        }
        out << "\n";
    }
}
