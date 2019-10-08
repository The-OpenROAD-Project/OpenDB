#!/bin/ksh -p

TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        TOP=$(dirname $(pwd))
fi

LSFTOP=`grep lsf_top $TOP/conf/msh.conf | awk '{print $2}'`
LSFLIC=`grep lsf_lic $TOP/conf/msh.conf | awk '{print $2}'`
LSFPORT=`grep lsf_port $TOP/conf/msh.conf | awk '{print $2}'`

if [[ "$LSFLIC" == "Pass-Thru" || "LSFPORT" == "Pass-Thru" ]] ; then
	print "LSF configuration is setup for Pass_Thru mode!"
	print "This mode do not support LSF Health diagnostic!"
	exit 1
fi


if [[ $LSFTOP = @('""'|''|'#') ]] || [[ $LSFLIC = @('""'|''|'#') ]] || [[ $LSFPORT = @('""'|''|'#') ]]
then
       	print "LSF have not been configured! Please run configure from install directory"
       	exit 1
fi
ISDAEMON=`$TOP/license/bin/lmstat -c $LSFPORT@$LSFLIC | grep UP `
if [[ $ISDAEMON = '' ]]
then
       	print "License DAEMON at $LSFPORT@$LSFLIC is down"
       	exit 1
fi
ISLIC=`$TOP/license/bin/lmstat -a -c $LSFPORT@$LSFLIC | grep lsf_base:`
if [[ $ISLIC = '' ]]
then
       	print "License for Application do not exist"
       	exit 1
fi

echo "$ISDAEMON"
echo "$ISLIC"

echo "LSF RJE from Platform Computing is configured for TurboRC"
echo "  lsf_top $LSFTOP           # Top of LSF tree"
echo "  lsf_lic $LSFLIC                    # LSF license server"
echo "  lsf_port $LSFPORT                  # LSF license port"
