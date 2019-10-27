class CNN { // Convolutional Neural Network (CNN)
public:
  CNN(const std::vector<int>& cl
     ,const std::vector<int> pt);
  CNN(const std::string& fileName);

  void clear();
  void read(const std::string& path);
  void write(const std::string& path);

  std::vector<double> run(const Image& im);
  void teach(const Image& im
            ,const std::vector<double>& correct
            ,double n = 0.1);
  
  int h() const;
  int w() const;

  int outsz() const;
  std::string arch() const;

private:
  std::vector<Cluster> a;
  std::vector<Layer> in, out, delta;
  std::vector<std::vector<Layer> > pl;
  Perceptron p;
};
