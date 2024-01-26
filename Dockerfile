FROM ubuntu:latest

RUN mkdir /src

WORKDIR /src

COPY . /src

RUN apt update

RUN apt install -y make g++

CMD [ "bash", "-c", "make && ./ircserv 1337 mouad"]