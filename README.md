~~Required by Armadillo: `sudo apt-get install libblas-dev liblapack-dev`~~

# Install armadillo using package manager
```
sudo apt-get install libarmadillo-dev
```
# Install doxygen (optional)
```
sudo apt-get install doxygen graphviz
```
# Clone mlpack:
```
git clone https://github.com/mlpack/mlpack.git
# checkout the latest branch
git checkout 4.2.0
```
or download the archive from [Github repo](https://github.com/mlpack/mlpack/releases/tag/4.2.0)
# Build mlpack:
```
mkdir build && cd build
cmake -DDOWNLOAD_DEPENDENCIES=ON -DCMAKE_INSTALL_PREFIX=./installdir -DDEBUG=ON ..
make install
```
