# Base
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
  gcc \
  g++\
  clang \
  clang-format \
  make \
  cmake \
  git \
  gdb \
  && rm -rf /var/lib/apt/lists/*

RUN useradd -s /bin/bash -m builder
USER builder

RUN git config --global user.name kota-kota
RUN git config --global user.email ko.lespaul0119@gmail.com
