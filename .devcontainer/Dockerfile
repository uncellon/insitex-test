FROM ubuntu:noble
LABEL Description="Ubuntu build environment"

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8
SHELL ["/bin/bash", "-c"]

RUN apt-get update && \
    apt install -y cmake ninja-build git \
                   build-essential \
                   clang clangd \
                   gdb

WORKDIR /root