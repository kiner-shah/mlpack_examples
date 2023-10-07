#include <mlpack.hpp>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <csv_training>" << '\n';
        return -1;
    }

    using namespace mlpack;

    // Load training data
    std::cout << "Training file to load: " << argv[1] << '\n';
    arma::mat predictors;
    arma::field<std::string> headers;
    bool load_status = predictors.load(arma::csv_name(argv[1], headers, arma::csv_opts::trans));
    if (!load_status)
    {
        std::cerr << "File " << argv[1] << " couldn't be loaded\n";
        return 1;
    }
    std::cout << "File loaded. Headers are: " << headers << '\n';
    // Column-major matrix -> last row contains the responses
    arma::rowvec responses = predictors.row(predictors.n_rows - 1);
    predictors.shed_row(predictors.n_rows - 1);

    // Train model
    std::cout << "Training LR model...\n";
    LinearRegression lr(predictors, responses);
    std::cout << "Computed error: " << lr.ComputeError(predictors, responses) << '\n';

    // Save model to model.xml
    std::cout << "Saving LR model...\n";
    if (!data::Save("model.xml", "model", lr))
    {
        std::cerr << "Couldn't save the model to model.xml file\n";
        return 1;
    }
    std::cout << "Saved LR model to model.xml\n";
    return 0;
}
