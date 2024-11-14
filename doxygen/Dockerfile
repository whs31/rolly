FROM ubuntu:20.04

LABEL maintainer="Dmitriy Ryazantsev <riazantsev.dl@edu.spbstu.ru>"
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install -y --no-install-recommends doxygen graphviz python3 python3-pip git python3-dev python3-setuptools
RUN ln -s /usr/bin/python3 /usr/bin/python

RUN pip3 install --upgrade pip Jinja2 pygments
RUN git clone https://github.com/maybe-unused/m.css.git /mcss
RUN chmod 777 /mcss
RUN mkdir /public
RUN chmod 777 /public