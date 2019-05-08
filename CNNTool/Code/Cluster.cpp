Cluster::Cluster(int sz, const Layer& lay) {
    a.assign(sz, lay);
}

Cluster::~Cluster() {
    a.clear();
}

int Cluster::size() const {
    return a.size();
}

Layer& Cluster::operator[](int x) {
    return a[x];
}

const Layer& Cluster::operator[](int x) const {
    return a[x];
}

void Cluster::read(ifstream& in) {
    int sz;
    in >> sz;
    a.assign(sz, Layer());
    for (int i = 0; i < sz; ++i) {
        a[i].read(in);
    }
}

void Cluster::write(ofstream& out) {
    out << " " << a.size() << "\n";
    for (int i = 0; i < size(); ++i) {
        a[i].write(out);
    }
}