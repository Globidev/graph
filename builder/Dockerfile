FROM gcc:6.1

MAINTAINER guillaume.depardon@navendis.com

RUN apt-get update -y && apt-get install -y \
    cmake \
    inotify-tools

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

# Osmium
ENV OSMIUM_ARCHIVE_URL  https://github.com/osmcode/libosmium/archive/v2.7.2.tar.gz
ENV OSMIUM_SRC_DIR      /osmium
ENV OSMIUM_INCLUDE_DIR  ${OSMIUM_SRC_DIR}/include

RUN mkdir ${OSMIUM_SRC_DIR} &&\
    curl -L ${OSMIUM_ARCHIVE_URL} | tar xz --strip-components=1 -C ${OSMIUM_SRC_DIR}

# Silicon
ENV IOD_ARCHIVE_URL     https://github.com/matt-42/iod/archive/master.tar.gz
ENV IOD_SRC_DIR         /iod
ENV SILICON_ARCHIVE_URL https://github.com/matt-42/silicon/archive/master.tar.gz
ENV SILICON_SRC_DIR     /silicon-src
ENV SILICON_PREFIX_DIR  /silicon
ENV SILICON_INCLUDE_DIR ${SILICON_PREFIX_DIR}/include

RUN mkdir ${SILICON_SRC_DIR} ${SILICON_PREFIX_DIR} ${IOD_SRC_DIR} &&\
    curl -L ${IOD_ARCHIVE_URL} | tar xz --strip-components=1 -C ${IOD_SRC_DIR} &&\
    curl -L ${SILICON_ARCHIVE_URL} | tar -xz --strip-components=1 -C ${SILICON_SRC_DIR} &&\
    cd ${IOD_SRC_DIR} &&\
    mkdir build && cd build &&\
    cmake .. \
        -DCMAKE_PREFIX_PATH=${SILICON_PREFIX_DIR} \
        -DCMAKE_INSTALL_PREFIX=${SILICON_PREFIX_DIR} \
        -DBOOST_ROOT=${BOOST_PREFIX_DIR} &&\
    CPATH=${BOOST_INCLUDE_DIR} make -j4 install > iod.build &&\
    cd ${SILICON_SRC_DIR} &&\
    mkdir build && cd build &&\
    cmake .. \
        -DCMAKE_PREFIX_PATH=${SILICON_PREFIX_DIR} \
        -DCMAKE_INSTALL_PREFIX=${SILICON_PREFIX_DIR} &&\
    make -j4 install > silicon.build &&\
    rm -rf ${IOD_SRC_DIR} ${SILICON_SRC_DIR}

# Libmicrohttpd
ENV MICROHTTPD_ARCHIVE_URL  http://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-latest.tar.gz
ENV MICROHTTPD_SRC_DIR      /microhttpd-src
ENV MICROHTTPD_PREFIX       /microhttpd
ENV MICROHTTPD_INCLUDE_DIR  ${MICROHTTPD_PREFIX}/include
ENV MICROHTTPD_LIB_DIR      ${MICROHTTPD_PREFIX}/lib

RUN mkdir ${MICROHTTPD_SRC_DIR} ${MICROHTTPD_PREFIX} &&\
    curl -L ${MICROHTTPD_ARCHIVE_URL} | tar xz --strip-components=1 -C ${MICROHTTPD_SRC_DIR} &&\
    cd ${MICROHTTPD_SRC_DIR} &&\
    ./configure --prefix=${MICROHTTPD_PREFIX} &&\
    make -j4 install > microhttpd.build &&\
    rm -rf ${MICROHTTPD_SRC_DIR}

# Hardplacing the default script in the image
ADD watch.sh /scripts/watch.sh

# User specific configuration
ARG UID=1000
ARG USER=docker
RUN id ${USER} || useradd -u ${UID} -m ${USER}

WORKDIR /app

CMD ["/scripts/watch.sh"]
