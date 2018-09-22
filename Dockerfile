FROM ubuntu:18.04
MAINTAINER Martin Elias <melias122@gmail.com>

RUN apt-get update
RUN apt-get install -y bash build-essential cmake clang-format qt5-default
