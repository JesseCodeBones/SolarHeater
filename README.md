# Solar Heater Simulator

## build (ubuntu)
### install gtest
```bash
apt-get clean && apt-get update -y && apt-get install wget libgtest-dev python3 ninja-build gcovr  clang-tidy clang-format curl cmake build-essential nodejs npm -y --no-install-recommends
```
```bash
cd /usr/src/gtest && cmake CMakeLists.txt && make && cd ./lib && cp *.a /usr/lib && ln -s /usr/lib/libgtest.a /usr/local/lib/libgtest.a && ln -s /usr/lib/libgtest_main.a /usr/local/lib/libgtest_main.a
```
### build and execute
```bash 
cmake -B build -DENABLE_CLANG_TIDY=1 -DENABLE_WERROR=1 -G Ninja && cmake --build build --parallel --target solar_heater && ./build/solar_heater
```
### run test
```bash
cmake --build build --parallel --target cov_html
```
### coverage report path
`./build/cov_html/index.html`
