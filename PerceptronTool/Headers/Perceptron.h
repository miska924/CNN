class Perceptron {
    public:
        Perceptron();
        Perceptron(ifstream& in);
        Perceptron(const vector<Matrix>& a);
        Perceptron(const vector<int>& pt, bool rnd = false);
        void read(ifstream& in);
        void write(ofstream& out);
        vector<double> run(const vector<double>& input);
        void teach(const vector<double>& input, const vector<double>& correct, double n = 1e-3, int steps = 1);
        size_t size() const;
        size_t szin() const;
        size_t szout() const;
        vector<double> d();

        Matrix& operator[](int x);
        const Matrix& operator[](int x) const;
        string arch() const;
    
    private:
        void change(double n);
        double f(double x);
        double fs(double x);
        vector<Matrix> vec;
        vector<vector<double> > out;
        vector<vector<double> > in;
        vector<vector<double> > delta;
};