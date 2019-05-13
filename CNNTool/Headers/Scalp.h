class Scalp {
    public:
        Scalp(int h = 0, int w = 0, bool rnd = false);
        ~Scalp();
        void read(std::ifstream& in);
        void write(std::ofstream& out);
        std::vector<double>& operator[](int x);
        const std::vector<double>& operator[](int x) const;
        int w() const;
        int h() const;
    private:
        std::vector<std::vector<double> > a;
};
