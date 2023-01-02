FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

COPY ./kitware.list ./llvm.list /etc/apt/sources.list.d/
COPY ./kitware-archive-keyring.gpg ./llvm-snapshot.gpg /usr/share/keyrings/

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    clang-15 \
    clang-format-15 \
    clang-tidy-15 \
    clang-tools-15 \
    clangd-15 \
    cmake \
    cmake-curses-gui \
    doxygen \
    g++-12 \
    graphviz \
    lld-15 \
    lldb-15 \
    python3-pip

USER vscode

RUN pip3 install --no-cache-dir --user cmakelang
