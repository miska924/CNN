class Image {
    public:
        Image(int n = 0, int m = 0, int p = 3, const vector<int>& color = vector<int>(3));
        Image(const string& path);
        ~Image();
        void paint(int d = 32);
        void show();
        void read(const string& path);
        void write(const string& path) const;
        int n() const;
        int m() const;
        int p() const;
        vector<vector<int> >& operator[](int x);
        const vector<vector<int> >& operator[](int x) const;
        void filter(const Tenzor& kernel);
        void normalize();
        void pool(int d);
        void mono();
        vector<int> vec();
    private:
        vector<vector<vector<int> > > a;
};