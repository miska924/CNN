class Tenzor {
    public:
        Tenzor(int n = 0, int m = 0, int p = 3);
        Tenzor(const string& path);
        ~Tenzor();
        void read(const string& path);
        void write(const string& path) const;
        int n() const;
        int m() const;
        int p() const;
        vector<vector<int> >& operator[](int x);
        const vector<vector<int> >& operator[](int x) const;
    private:
        vector<vector<vector<int> > > a;
};