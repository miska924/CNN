class Perceptron {
    public:
        Perceptron();
        Perceptron(std::ifstream& in);
        Perceptron(const std::vector<Matrix>& a);
        Perceptron(const std::vector<int>& pt, bool rnd = false);
        void read(std::ifstream& in);
        void write(std::ofstream& out);
        std::vector<double> run(const std::vector<double>& input);
        void teach(const std::vector<double>& input, const std::vector<double>& correct, double n = 1e-3, int steps = 1);
        size_t size() const;
        size_t szin() const;
        size_t szout() const;
        std::vector<double> d();

        Matrix& operator[](int x);
        const Matrix& operator[](int x) const;
        std::string arch() const;

    private:
        void change(double n);
        double f(double x);
        double fs(double x);
        std::vector<Matrix> vec;
        std::vector<std::vector<double> > out;
        std::vector<std::vector<double> > in;
        std::vector<std::vector<double> > delta;
};
