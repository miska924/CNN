class Scalp {
    public:
        Scalp(int h = 0, int w = 0, bool rnd = false);
        ~Scalp();
        void read(ifstream& in);
        void write(ofstream& out);
        vector<double>& operator[](int x);
        const vector<double>& operator[](int x) const;
        int w() const;
        int h() const;
    private:
        vector<vector<double> > a;
};