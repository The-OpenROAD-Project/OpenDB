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

function ck_Lic_exp {
typeset da="$1"

typeset i CMON=$(date '+%m')
typeset i CDATE=$(date '+%d')
typeset i CYEAR=$(date '+%Y')

typeset i LWMON=$(echo $da | awk -F\- '{print $2}')
typeset i LDATE=$(echo $da | awk -F\- '{print $1}')
typeset i LYEAR=$(echo $da | awk -F\- '{print $3}')
month2num $LWMON
if [[ $CYEAR -gt $LYEAR ]] ; then
echo "Expired"
fi

if [[ $CYEAR -eq $LYEAR ]] && [[ $CMON -gt $lmon ]] ; then
print "Expired"
fi

if [[ $CYEAR -eq $LYEAR ]] && [[ $CMON -eq $lmon ]] && [[ $CDATE -gt $LMON ]]
then
print "Expired"
fi

if [[ $CYEAR -eq $LYEAR ]] && [[ $(($LMON - $CMON)) == "1" ]] ; then
        typeset i ncount="1"
        while [[ -z $(cal $CMON $CYEAR | tail -6 | tail -$ncount) ]] ; do
        ncount=$(($ncount + 1))
        done
        LASTDATE=$(cal $CMON $CYEAR | tail -6 | tail -$ncount | awk '{print $NF}')
        typeset i rdate=$(($LASTDATE - $CDATE + $LDATE))
        echo "$rdate"
fi

if [[ $CYEAR -eq $LYEAR ]] && [[ $(($LMON - $CMON)) == "0" ]] && [[ $CDATE -le $LDATE ]]; then
        typeset i rdate=$(($LDATE - $CDATE))
        echo "$rdate"
fi

if [[ $(($LYEAR - $CYEAR)) == "1" ]] ; then
        if [[ $LMON == "1" && $CMON == "12" ]] ; then
        typeset i ncount="1"
                while [[ -z $(cal $CMON $CYEAR | tail -6 | tail -$ncount) ]] ; do
                ncount=$(($ncount + 1))
        done
        LASTDATE=$(cal $CMON $CYEAR | tail -6 | tail -$ncount | awk '{print $NF}')
        typeset i rdate=$(($LASTDATE - $CDATE + $LDATE))
        echo "$rdate"

        fi
fi
#echo "Okay"
}

# #######################################
# Top of Tool
# #######################################

TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        BTOP=$(pwd | awk -F\/ '{print $NF}')
	TOP=$(pwd | awk -F/$BTOP '{print $1}')
fi

ADSTOP=`grep "ads_top " "$TOP/conf/msh.conf" | awk '{print $2}'`
ADSLIC=`grep "ads_lic " "$TOP/conf/msh.conf" | awk '{print $2}'`
ADSPORT=`grep "ads_port " "$TOP/conf/msh.conf" | awk '{print $2}'`

# echo "ADSTOP = $ADSTOP; ADSLIC = $ADSLIC; ADSPORT = $ADSPORT"
# #######################################
# Verify Successful Installation
# #######################################
if [[ $ADSLIC = @('""'|''|'#') ]] || [[ $ADSPORT = @('""'|''|'#') ]] ; then
        print "Athena License have not been configured! Please run configure from install directory"
        exit 1
fi

# #############################################
# Verify Hostid of server against license file
# #############################################

if [[ -e $TOP/conf/ads_license.dat ]] ; then
local_host=$(/bin/hostname)
	if [[ "$local_host" == "$ADSLIC" ]] ; then
	machine_hostid=$($TOP/license/bin/lmhostid | tail -1 | awk -F\" '{print $2}' | tr "[:upper:]" "[:lower:]")
	license_hostid=$(grep "^SERVER" "$TOP/conf/ads_license.dat" | awk '{print $3}' | tr "[:upper:]" "[:lower:]")
	license_hostname=$(grep "^SERVER" "$TOP/conf/ads_license.dat" | awk '{print $2}')
		if [[ "$machine_hostid" != "$license_hostid" ]] ; then
		print "Hostid of Server and License File do not match"
		print "Hostid in License File = "$license_hostid""
		print "Hostid of $ADSLIC is "$machine_hostid""
		exit 1
		fi
	else
	print "Please rerun script from server $ADSLIC"
	exit 1
	fi
else
	print "License file ads_license.dat can not be found at $TOP/conf/ads_license"
	exit 1
fi

# ##################################################
# Verify localhost against server from license file
# ##################################################

license_host_file=$(grep "^SERVER" "$TOP/conf/ads_license.dat" | awk '{print $2}')
if [[ "$license_host_file" != "$ADSLIC" ]] ; then
	print "Server Name in configuration and license file do not match!"
	print "    License Server in Configuration = "$ADSLIC""
	print "    License Server in License File = "$license_host_file""
	exit 1
fi

# ################
# Expiration Check
# ################

feature_date_list=$(grep "^FEATURE" "$TOP/conf/ads_license.dat" | awk '{print $5}' | sort | uniq)
for fdate in $feature_date_list ; do
check_license_out=$(ck_Lic_exp "$fdate")
if [[ "$check_license_out" == "Expired" ]] ; then
print "Software License have expired!"
print ""
cat "$TOP/conf/ads_license.dat"
exit 1
fi
done

# ##################
# License Startup
# ##################
ISDAEMON=$($TOP/license/bin/lmstat -c "$ADSPORT@$ADSLIC" | grep UP)
lic_log=$(grep "^ads_log" "$TOP/conf/msh.conf" | awk '{print $2}')
if [[ $ISDAEMON == "" ]] ; then
	
	if [[ -e "$lic_log" && ! -d "$lic_log" ]] ; then
		log_dir=$(dirname "$lic_log")
		logfile=$(echo "$lic_log" | awk -F\/ '{print $NF}')
		cdate=$(date '+%m%d%H%M')
		mv $log_dir/$logfile $log_dir/$logfile-$cdate
		$TOP/license/bin/lmgrd -c $ADSTOP/conf/ads_license.dat -l $lic_log
		sleep 4
	else
		log_dir=$(dirname "$lic_log")
		logfile=$(echo "$lic_log" | awk -F\/ '{print $NF}')
		if [[ -d "$log_dir" ]] ; then
		$TOP/license/bin/lmgrd -c $ADSTOP/conf/ads_license.dat -l $lic_log
		else
		mkdir $log_dir
		$TOP/license/bin/lmgrd -c $ADSTOP/conf/ads_license.dat -l $lic_log
		sleep 4
		fi
	
	fi
	
	ISDAEMON2=$($TOP/license/bin/lmstat -c "$ADSPORT@$ADSLIC" | grep UP)

	if [[ $ISDAEMON2 = '' ]]
	then
		printf "\nUnknown Problem starting License Daemon\n\n"
		cat $TOP/conf/ads_license.dat
		exit 1
	else
		$TOP/license/bin/lmstat -a -c $ADSPORT@$ADSLIC
	fi
else

	printf "\nAthena license daemon is already running!\n\n"
	$TOP/license/bin/lmstat -a -c "$ADSPORT@$ADSLIC"
		
fi
