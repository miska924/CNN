class Tenzor {
    public:
        Tenzor(int n = 0, int m = 0, int p = 3);
        Tenzor(const std::string& path);
        ~Tenzor();
        void read(const std::string& path);
        void write(const std::string& path) const;
        int n() const;
        int m() const;
        int p() const;
        std::vector<std::vector<int> >& operator[](int x);
        const std::vector<std::vector<int> >& operator[](int x) const;
    private:
        std::vector<std::vector<std::vector<int> > > a;
};
