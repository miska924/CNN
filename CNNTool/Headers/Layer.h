class Layer {
    public:
        Layer(const Image& im);
        Layer(int sz = 0, int h = 0, int w = 0, bool rnd = false);
        ~Layer();
        int size() const;
        int h() const;
        int w() const;
        void read(std::ifstream& in);
        void write(std::ofstream& out);
        void transform(const Cluster& cl);
        void transform(const Layer& lay);
        std::vector<Layer> pool(bool plg = true);
        void apply(double (*f)(double));
        std::vector<double> vec();
        void fill(const std::vector<double>& vec);
        Scalp& operator[](int x);
        const Scalp& operator[](int x) const;
    private:
        std::vector<Scalp> a;
};
