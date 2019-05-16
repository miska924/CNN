class Matrix {
    public:
        int n, m;

        Matrix();
        Matrix(int n_);
        Matrix(int n_, int m_, bool rnd = false);
        Matrix(const std::vector<double>& v);
        Matrix(const std::vector<std::vector<double> >& v);
        ~Matrix();

        std::vector<double>& operator[](int x);
        const std::vector<double>& operator[](int x) const;

        Matrix operator*(const Matrix& a);
        Matrix operator*=(const Matrix& a);
        Matrix operator^(int a);
        Matrix operator^=(int a);

    private:
        std::vector<std::vector<double> > vec;
};

std::istream& operator>>(std::istream& in, Matrix &a);
std::ostream& operator<<(std::ostream& out, const Matrix& a);
