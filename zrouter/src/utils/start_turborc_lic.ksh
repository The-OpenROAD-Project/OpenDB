#!/bin/ksh -p

unset ENV

function month2num {
        typeset month=$1
        case $month in
        jan )
                lmon=1;;
        feb )
                lmon=2;;
        mar )
                lmon=3;;
        apr )
                lmon=4;;
        may )
                lmon=5;;
        jun )
                lmon=6;;
        jul )
                lmon=7;;
        aug )
                lmon=8;;
        sep )
                lmon=9;;
        oct )
                lmon=10;;
        nov )
                lmon=11;;
        dec )
                lmon=12;;
        * )
                print "Error Month to numeric conversion"
        esac
}

function check_hostid {
        HOSTSID=$($TOP/license/bin/lmhostid | tail -1 | awk -F\" '{print $2}')
        LICID=$(grep SERVER $ADSTOP/ads_license.dat | awk '{print $3}')
                if [[ $HOSTSID != $LICID ]]
                then
                print "Hostid of server and hostid of license file do not match!"
                exit 1
                fi
}

function ck_Lic_exp {

integer CMON=$(date '+%m')
integer CDATE=$(date '+%d')
integer CYEAR=$(date '+%Y')

LMON=$(echo $da | awk -F\- '{print $2}')
integer LDATE=$(echo $da | awk -F\- '{print $1}')
integer LYEAR=$(echo $da | awk -F\- '{print $3}')
month2num $LMON
if [[ $CYEAR -eq $LYEAR ]] && [[ $CMON -gt $lmon ]]
then
print "License is expired"
exit 1
fi

if [[ $CYEAR -eq $LYEAR ]] && [[ $CMON -eq $lmon ]] && [[ $CDATE -gt $LMON ]]
then
print "License is expired"
exit 1
fi

if [[ $CYEAR -eq $LYEAR ]] && [[ $CMON -le $lmon ]] && [[ $CDATE -gt $LMON ]]
then
mdiff=$(($lmon - $CMON))
        if [[ $mdiff -eq 1 ]]
        then
                dlist=$(cal $CMON $$CYEAR | tail -6)
                mdate=$(echo $dlist | awk '{print $NF}')
                rdate=$(($mdate - $CDATE + $LDATE))
                print "TurboRC license will expire in $rdate"
        fi
        if [[ $mdiff -eq 0 ]]
        then
                rdate=$(($CDATE - $LDATE))
                print "TurboRC license will expire in $rdate"
        fi
fi
}


TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        BTOP=$(pwd | awk -F\/ '{print $NF}')
	TOP=$(pwd | awk -F/$BTOP '{print $1}')
fi

ADSTOP=$(grep ads_top $TOP/conf/msh.conf | awk '{print $2}')
ADSLIC=$(grep ads_lic $TOP/conf/msh.conf | awk '{print $2}')
ADSPORT=$(grep ads_port $TOP/conf/msh.conf | awk '{print $2}')

if [[ $ADSLIC = @('""'|''|'#') ]] || [[ $ADSPORT = @('""'|''|'#') ]]
then
        print "TurboRC License have not been configured! Please run configure from install directory"
        exit 1
fi
ISDAEMON=$($TOP/license/bin/lmstat -c $ADSPORT@$ADSLIC | grep UP)
if [[ $ISDAEMON = '' ]]
then
        if [[ $(hostname) = $ADSLIC ]]
	then
		print ""
		$TOP/license/bin/lmgrd -c $ADSTOP/ads_license.dat -l $ADSLOG
		sleep 4
	else
		print "Please run script from server $ADSLIC"
		exit 1
	fi

	ISDAEMON2=$($TOP/license/bin/lmstat -c $ADSPORT@$ADSLIC | grep UP)
	if [[ $ISDAEMON2 = '' ]]
	then
		check_hostid

		EXPDATE=$(grep turborc $ADSTOP/ads_license.dat | awk '{print $5}')

		for da in $EXPDATE
		do
			ck_Lic_exp $da
		done

		printf "\nUnknown Problem starting License Daemon\n\n"
#		cat $TOP/license/ads_license.dat
		exit 1
	else
		$TOP/license/bin/lmstat -a -c $ADSPORT@$ADSLIC
	fi
else

	printf "\nTurboRC license daemon is already running!\n\n"
	$TOP/license/bin/lmstat -a -c $ADSPORT@$ADSLIC
		
fi

