FROM gcc:6.1

MAINTAINER guillaume.depardon@navendis.com

# Boost
ENV BOOST_ARCHIVE_URL   http://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.bz2/download
ENV BOOST_BUILD_DIR     /boost-build
ENV BOOST_PREFIX_DIR    /boost
ENV BOOST_INCLUDE_DIR   ${BOOST_PREFIX_DIR}/include
ENV BOOST_LIB_DIR       ${BOOST_PREFIX_DIR}/lib

RUN mkdir ${BOOST_BUILD_DIR} &&\
    curl -L ${BOOST_ARCHIVE_URL} | tar xj --strip-components=1 -C ${BOOST_BUILD_DIR} &&\
    cd ${BOOST_BUILD_DIR} &&\
    ./bootstrap.sh --prefix=${BOOST_PREFIX_DIR} \
        --with-libraries=system \
        --with-libraries=date_time \
        --with-libraries=regex \
        --with-libraries=program_options \
        --with-libraries=serialization &&\
    ./b2 -j4 install > boost.build &&\
    rm -rf ${BOOST_BUILD_DIR}

# cpp-netlib
ENV CPP_NETLIB_ARCHIVE_URL  http://downloads.cpp-netlib.org/0.12.0/cpp-netlib-0.12.0-final.tar.gz
ENV CPP_NETLIB_SRC_DIR      /cpp-netlib-src
ENV CPP_NETLIB_BUILD_DIR    /cpp-netlib-build

RUN mkdir ${CPP_NETLIB_SRC_DIR} ${CPP_NETLIB_BUILD_DIR} &&\
    curl -L ${CPP_NETLIB_ARCHIVE_URL} | tar xz --strip-components=1 -C ${CPP_NETLIB_SRC_DIR} &&\
    cd ${CPP_NETLIB_BUILD_DIR} &&\
    apt-get update -y && apt-get install -y cmake &&\
    cmake -DBOOST_ROOT=${BOOST_PREFIX_DIR} ${CPP_NETLIB_SRC_DIR} &&\
    make -j4 install &&\
    cp -r ${CPP_NETLIB_SRC_DIR}/deps/asio/asio/include/asio /usr/include/ &&\
    rm -rf ${CPP_NETLIB_SRC_DIR} ${CPP_NETLIB_BUILD_DIR}
    # see https://groups.google.com/forum/#!topic/cpp-netlib/0g4aRaREco8

# osmium
ENV OSMIUM_ARCHIVE_URL  https://github.com/osmcode/libosmium/archive/v2.7.2.tar.gz
ENV OSMIUM_SRC_DIR      /osmium
ENV OSMIUM_INCLUDE_DIR  ${OSMIUM_SRC_DIR}/include

RUN mkdir ${OSMIUM_SRC_DIR} &&\
    curl -L ${OSMIUM_ARCHIVE_URL} | tar xz --strip-components=1 -C ${OSMIUM_SRC_DIR}

# inotifywait dependencies
RUN apt-get update -y && apt-get install -y \
    inotify-tools

# Hardplacing the default script in the image
ADD watch.sh /scripts/watch.sh

# User specific configuration
ARG UID=1000
ARG USER=docker
RUN id ${USER} || useradd -u ${UID} -m ${USER}

WORKDIR /app

CMD ["/scripts/watch.sh"]
