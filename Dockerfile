FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y \
	build-essential \
	cmake \
	libboost-all-dev

COPY . /root/build_dir
WORKDIR /root/build_dir

RUN cmake . && make
RUN ./hash_file test/test_file out
RUN printf "res:\n" && cat out && printf "\n\n"
