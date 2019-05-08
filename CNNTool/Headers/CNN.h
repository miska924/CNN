class CNN { // Convolutional Neural Network (CNN)
    public:
        CNN(const vector<int>& cl, const vector<int> pt);
        CNN();
        ~CNN();
        CNN(const string& path);
        void clear();
        void read(const string& path);
        void write(const string& path);
        vector<double> run(const Image& im);
        void teach(const Image& im, const vector<double>& correct, double n = 0.1);
        int h() const;
        int w() const;
        int outsz() const;
        string arch() const;
    private:
        vector<Cluster> a;
        vector<Layer> in, out, delta;
        vector<vector<Layer> > pl;
        Perceptron p;
};
