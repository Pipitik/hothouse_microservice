FROM ubuntu

WORKDIR /server
COPY source/ source/
COPY CMakeLists.txt conanfile.py .

RUN apt update -yq && apt install -yq \
    g++ \
    cmake \
    python3-pip
    
RUN pip install --break-system-packages conan==1.62
RUN conan profile new --detect linux_gcc && \
    conan profile update settings.compiler.libcxx=libstdc++11 linux_gcc
RUN conan install --install-folder=build --profile=linux_gcc --build=missing .
RUN conan build --build-folder=build .