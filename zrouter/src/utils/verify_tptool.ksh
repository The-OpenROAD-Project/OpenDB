#!/bin/ksh -p

function die {
        echo "verify_tptool.ksh: $*" 1>&2
        exit 1
}

appl=$(echo $1)
if [[ -z "$appl" ]] ; then
print "Usage: verify_tptool.ksh $configured_third-party-tool"
print "Third-party-tool must be configure for usage under $product"
exit 1
fi


TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        TOP=$(dirname $(pwd))
fi
fapp1=`grep $appl $TOP/conf/msh.conf  | grep "_feat" | awk -F\" '{print $2}'`
TAPP_TOP=`grep "$appl"_top $TOP/conf/msh.conf | awk '{print $2}'`
TAPP_LIC=`grep "$appl"_lic $TOP/conf/msh.conf | awk '{print $2}'`
TAPP_PORT=`grep "$appl"_port $TOP/conf/msh.conf | awk '{print $2}'`

if [[ $TAPP_TOP = @('""'|''|'#') ]] || [[ $TAPP_LIC = @('""'|''|'#') ]] || [[ $TAPP_PORT = @('""'|''|'#') ]]
then
        print ""$appl" have not been configured! Please run configure.ksh from install directory"
        exit 1
fi
ISDAEMON=`$TOP/license/bin/lmstat -c $TAPP_PORT@$TAPP_LIC | grep UP `
if [[ $ISDAEMON = '' ]]
then
        print "License DAEMON at $TAPP_PORT@$TAPP_LIC is down"
        exit 1
fi
ISLIC=`$TOP/license/bin/lmstat -a -c $TAPP_PORT@$TAPP_LIC | grep "$fapp1":`
if [[ $ISLIC = '' ]]
then
        print "License for Application do not exist"
        exit 1
fi

echo "$ISDAEMON"
echo "$ISLIC"

echo "$appl is configured for usage under $product"
echo "  "$appl"_top $TAPP_TOP"
echo "  "$appl"_lic $TAPP_LIC"
echo "  "$appl"_port $TAPP_PORT"
