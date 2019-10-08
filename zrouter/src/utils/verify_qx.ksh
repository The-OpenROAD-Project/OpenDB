#!/bin/ksh -p

TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        TOP=$(dirname $(pwd))
fi

QXTOP=`grep qx_top $TOP/conf/msh.conf | awk '{print $2}'`
QXLIC=`grep qx_lic $TOP/conf/msh.conf | awk '{print $2}'`
QXPORT=`grep qx_port $TOP/conf/msh.conf | awk '{print $2}'`

if [[ $QXTOP = @('""'|''|'#') ]] || [[ $QXLIC = @('""'|''|'#') ]] || [[ $QXPORT = @('""'|''|'#') ]]
then
        print "QX have not been configured! Please run configure from install directory"
        exit 1
fi
ISDAEMON=`$TOP/license/bin/lmstat -c $QXPORT@$QXLIC | grep UP `
if [[ $ISDAEMON = '' ]]
then
        print "License DAEMON at $QXPORT@$QXLIC is down"
        exit 1
fi
ISLIC=`$TOP/license/bin/lmstat -a -c $QXPORT@$QXLIC | grep Fire_Ice_Cell:`
if [[ $ISLIC = '' ]]
then
        print "License for Application do not exist"
        exit 1
fi


echo "$ISDAEMON"
echo "$ISLIC"

echo "QX point tools from Cadence is configured for TurboRC"
echo "  qx_top $QXTOP           # Top of Cadence QX tree"
echo "  qx_lic $QXLIC                    # QX license server"
echo "  qx_port $QXPORT                  # QX license port"
