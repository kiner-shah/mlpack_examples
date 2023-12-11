- Dataset info - https://paperswithcode.com/dataset/tweebank
- Data - https://raw.githubusercontent.com/Oneplus/Tweebank/dev/en-ud-tweet-train.conllu
- CoNLL format - https://universaldependencies.org/format.html
- Word list: https://norvig.com/ngrams/word.list
- POS tags list: https://universaldependencies.org/u/pos/index.html

### Download data
```
wget "https://raw.githubusercontent.com/Oneplus/Tweebank/dev/en-ud-tweet-train.conllu" -O data.txt
```
### Run preprocessing
```
g++ preprocess_conll.cpp -o preprocess -fsanitize=address -std=c++20
```
### Run training
```
g++ -O3 utils.cpp hmm_train.cpp -o hmm_train -fsanitize=address -I mlpack-4.2.0/build/installdir/include -larmadillo
```
### Run prediction
```
g++ -O3 utils.cpp hmm_predict.cpp -o hmm_predict -fsanitize=address -I mlpack-4.2.0/build/installdir/include -larmadillo
``` 
