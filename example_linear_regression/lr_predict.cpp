#include <mlpack.hpp>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <model_file>" << '\n';
        return -1;
    }

    using namespace mlpack;

    LinearRegression lr;
    // Second parameter is the XML element in the Cereal model file i.e. an NVP (Name-Value Pair)
    if (!data::Load(argv[1], "model", lr))
    {
        std::cerr << "Model loading failed\n";
        return 1;
    }

    // Try prediction from user inputs
    double temp, pressure, humidity, vacuum;
    std::cout << "Temperature (celcius): ";
    std::cin >> temp;
    std::cout << "Ambient Pressure (millibar): ";
    std::cin >> pressure;
    std::cout << "Relative Humidity (%): ";
    std::cin >> humidity;
    std::cout << "Exhaust vacuum (cm Hg): ";
    std::cin >> vacuum;

    arma::colvec point{temp, vacuum, pressure, humidity};
    arma::mat points{point};

    arma::rowvec predictions;
    lr.Predict(points, predictions);

    std::cout << "Net hourly electrical energy output (MW): ";
    for (auto x : predictions)
    {
        std::cout << x << std::endl;
    }
}
