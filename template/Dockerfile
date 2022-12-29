FROM conanio/clang14-ubuntu16.04 as place

#-- Install latest cmake
RUN pip install cmake --upgrade

ADD ./conanfile.txt /app/
WORKDIR /app/build

USER root
# Build dependencies
RUN conan install .. -b missing -s compiler.libcxx=libstdc++11

ADD . /app/
WORKDIR /app/build

# Build and Test Application
RUN cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . && ctest .

WORKDIR /app/build/bin

CMD ./template
