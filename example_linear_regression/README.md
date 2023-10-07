# Linear regression example using mlpack

### Dataset URL: https://archive.ics.uci.edu/dataset/294/combined+cycle+power+plant

# Using CLI
## Train on data and generate a model:
```
cd installdir/bin
./mlpack_linear_regression --training_file ~/mlpack/data/CCPP/train.csv -v -M lr.xml
```
## Test the model:
### Preprocess data
```
cut -d, -f1-4 ~/mlpack/data/CCPP/test.csv > test_input.csv
```
### Run testing
```
./mlpack_linear_regression --input_model_file lr.xml --test_file test_input.csv --output_predictions_file output.csv
```
### Check output file
```
# The below command may not show output in correct format
cat output.csv
# Use the below command to see output in correct format
awk '{printf "%g\n", $1}' output.csv
```

# Compiling C++ code
## Using armadillo library installed using package manager
### Compile lr_train.cpp
```
g++ lr_train.cpp -o lr_train -O2 -I mlpack-4.2.0/build/installdir/include -larmadillo
```
### Compile lr_predict.cpp
```
g++ lr_predict.cpp -o lr_predict -O2 -I mlpack-4.2.0/build/installdir/include -larmadillo
```
## Without installing armadillo using package manager (not preferred)
### Compile lr_train.cpp
```
g++ lr_train.cpp -o lr_train -O2 -I mlpack-4.2.0/build/installdir/include -DARMA_DONT_USE_WRAPPER -lblas -llapack
```
### Compile lr_predict.cpp
```
g++ lr_predict.cpp -o lr_predict -O2 -I mlpack-4.2.0/build/installdir/include -DARMA_DONT_USE_WRAPPER -lblas -llapack
```
# Run
## Training
```
./lr_train <path_to_csv_file>
```
## Prediction
```
./lr_predict <path_to_model_xml>
```
