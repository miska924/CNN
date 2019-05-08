class Matrix {
    public:
        int n, m;

        Matrix();
        Matrix(int n_);
        Matrix(int n_, int m_, bool rnd = false);
        Matrix(const vector<double>& v);
        Matrix(const vector<vector<double> >& v);
        ~Matrix();

        vector<double>& operator[](int x);
        const vector<double>& operator[](int x) const;

        Matrix operator*(const Matrix& a);
        Matrix operator*=(const Matrix& a);
        Matrix operator^(int a);
        Matrix operator^=(int a);

    private:
        std::vector<std::vector<double> > vec;
};

istream& operator>>(istream& in, Matrix &a);
ostream& operator<<(ostream& out, const Matrix& a);