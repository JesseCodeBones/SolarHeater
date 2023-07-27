FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get clean && apt-get update -y && apt-get install wget libgtest-dev wget python3 ninja-build gcovr doxygen clang-tidy clang-format curl cmake build-essential nodejs npm -y --no-install-recommends
RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && cd ./lib && cp *.a /usr/lib && ln -s /usr/lib/libgtest.a /usr/local/lib/libgtest.a && ln -s /usr/lib/libgtest_main.a /usr/local/lib/libgtest_main.a
#update nodejs
RUN npm install n -g && n stable
RUN npm config set strict-ssl false

# install cspell
RUN npm install -g cspell 

WORKDIR /home/emscripten_sdl2_framework