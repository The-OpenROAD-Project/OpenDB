FROM centos:centos7 AS builder
LABEL maintainer "Abdelrahman Hosny <abdelrahman_hosny@brown.edu>"

# Install Development Environment
RUN yum group install -y "Development Tools"
RUN yum install -y wget git

# Install epel repo
RUN wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
RUN yum install -y epel-release-latest-7.noarch.rpm

# Install dev and runtime dependencies
RUN yum install -y tcl-devel qt3-devel itcl-devel tcl tk ksh libstdc++ qt3 itcl iwidgets blt tcllib bwidget

COPY . /OpenDB
WORKDIR /OpenDB/zrouter/src

# Build
RUN make clean
RUN make
RUN make install

FROM centos:centos7 AS runner

RUN yum update -y && yum install -y wget git

# Install epel repo
RUN wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
RUN yum install -y epel-release-latest-7.noarch.rpm

RUN yum update -y && yum install -y ksh tcl tk libstdc++ qt3 iwidgets blt tcllib bwidget

ENV RELEASE=/opt/OpenDB/zrouter
ENV PATH=$RELEASE/bin:$RELEASE/x86-64-linux/zrouter/bin:$RELEASE/msh/dist/i386-linux/bin/:$PATH \
    LD_LIBRARY_PATH=$RELEASE/msh/dist/i386-linux/lib:/lib64:/usr/lib64:/lib:/usr/lib:$LD_LIBRARY_PATH \
    TCL_LIBRARY=/usr/lib64/tcl8.5 \
    TK_LIBRARY=/usr/lib64/tk8.5 \
    BLT_LIBRARY=/usr/share/tcl8.5/blt2.4 \
    MPT_ARCH=64 \
    ADS_LICENSE=ADS_2006 \
    NO_LICENSE_TOKEN=dimitris_friend \
    ADS_NO_LICENSE= \
    MSHCONFIG=$RELEASE/msh.conf

COPY --from=builder /OpenDB /opt/OpenDB
RUN chmod -R ogu+r /opt/OpenDB
