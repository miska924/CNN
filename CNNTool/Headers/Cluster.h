class Cluster {
    public:
        Cluster(int sz = 0, const Layer& lay = Layer());
        ~Cluster();
        void read(std::ifstream& in);
        void write(std::ofstream& out);
        int size() const;
        Layer& operator[](int x);
        const Layer& operator[](int x) const;
    private:
        std::vector<Layer> a;
};
