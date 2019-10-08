#!/bin/ksh -p

TOP=$(dirname $(dirname $0))

bunzip2 -c $TOP/design/E_milos_tar.bz2 | tar xvf -
