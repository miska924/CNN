class Image {
    public:
        Image(int n = 0, int m = 0, int p = 3, const std::vector<int>& color = std::vector<int>(3));
        Image(const std::string& path);
        ~Image();
        void paint(int d = 32);
        void show();
        void read(const std::string& path);
        void write(const std::string& path) const;
        int n() const;
        int m() const;
        int p() const;
        std::vector<std::vector<int> >& operator[](int x);
        const std::vector<std::vector<int> >& operator[](int x) const;
        void filter(const Tenzor& kernel);
        void normalize();
        void pool(int d);
        void mono();
        std::vector<int> vec();
    private:
        std::vector<std::vector<std::vector<int> > > a;
};
