FROM ubuntu

WORKDIR /usr/src/

RUN apt-get update && apt-get upgrade -y && \
	apt-get install -yq \
	gcc \
	e2fslibs-dev \
	pkg-config \
	libfuse-dev \
	build-essential \
	wget \
	meson \
	udev

RUN wget -O "fuse.tar.xz" "https://github.com/libfuse/libfuse/releases/download/fuse-3.5.0/fuse-3.5.0.tar.xz" && \
	tar -xf fuse.tar.xz && \
	rm -f fuse.tar.xz && mv fuse* fuse

RUN cd fuse && \
	mkdir build && cd build && \
	meson .. && meson configure -D prefix=/usr -D libdir=lib && \
	ninja install && \
	cd /usr/src

ADD Makefile .
ADD hello.c .

RUN make
