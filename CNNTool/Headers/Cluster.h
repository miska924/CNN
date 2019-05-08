class Cluster {
    public:
        Cluster(int sz = 0, const Layer& lay = Layer());
        ~Cluster();
        void read(ifstream& in);
        void write(ofstream& out);
        int size() const;
        Layer& operator[](int x);
        const Layer& operator[](int x) const;
    private:
        vector<Layer> a;
};