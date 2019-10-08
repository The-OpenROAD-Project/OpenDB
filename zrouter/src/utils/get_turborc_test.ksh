#!/bin/ksh -p

TOP=$(dirname $(dirname $0))

gunzip -c $TOP/design/turborc_tar.gz | tar xvf -
