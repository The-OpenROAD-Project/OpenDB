#!/bin/ksh -p

function die {
        echo "configure.ksh: $*" 1>&2
        exit 1
}

function opt_setup_pass {
	typeset mytool2="$1"
	typeset lpath=""
	typeset lserver=""
	typeset port=""
	typeset lic_control="setup-pass"
	typeset feat_list="-"
	perl -p -i -e "s/"$mytool2"_top \"\"/"$mytool2"_top "$lpath"/" ../conf/msh.conf
	perl -p -i -e "s/"$mytool2"_port \"\"/"$mytool2"_port $port/" ../conf/msh.conf
	perl -p -i -e "s/"$mytool2"_lic \"\"/"$mytool2"_lic $lserver/" ../conf/msh.conf
	perl -p -i -e "s/"$mytool2"_control \"\"/"$mytool2"_control "$lic_control"/" ../conf/msh.conf
}

function clear_lower {
if [[ -z $(echo $1) ]] ; then
typeset lcount=10
else
typeset lcount="$1"
fi
while [[ "$lcount" -lt "24" ]] ; do
        tput cup "$lcount" 0
        tput el
        (( lcount += 1 ))
done
tput cup 10 0

}

function menu_master {
typeset lmptype="$1"
typeset ltptool="$2"
typeset lcomplete="$3"
typeset ltask1="$4"
tput init
tput setb 3
tput clear
tput cup 0 31
print "$product Installation"
print "________________________________________________________________________________"
print "Installation Components: Athena Software, Multi-Processing, Third Party Tools"
tput cup 3 4
print "Athena Software: Operating Environment, Software License"
tput cup 4 4
print "Multi-Processing: "$lmptype""
tput cup 5 4
print "Third-Party Tools: "$ltptool""
tput cup 7 0
print "Completion: "$lcomplete""
tput cup 8 0
print "Current Task: "$ltask1""
print "________________________________________________________________________________"
tput setb 7
typeset lcount=10
while [[ "$lcount" -lt "24" ]] ; do
	tput cup "$lcount" 0
	tput el
	(( lcount += 1 ))
done
tput cup 10 0

}

function feature_setup {
typeset ltool=$1
typeset port=$2
typeset lserver=$3
typeset feat_list=$(grep $ltool ads.define | awk -F\: '{print $2}')
new_list=""
for feat in $feat_list
do
if [[ ! -z $(../license/bin/lmstat -f $feat -c $port@$lserver | grep Users | awk '{print $6}') ]] ; then
new_list=$(echo $new_list $feat)
fi
done
}

function del_blank_line {
typeset dlist=$(grep '""' ../conf/msh.conf)

for blanks in $dlist ; do
if [[ -z $( echo $blanks | grep '""') ]] ; then 
perl -p -i -e "s/$blanks \"\"//" ../conf/msh.conf
fi
done

mv ../conf/msh.conf ../conf/msh.conf.full
sed -e '/^\t$/d' ../conf/msh.conf.full | sed -e '/^        $/d' > ../conf/msh.conf
rm -rf ../conf/msh.conf.full
}

function cpu_control {
typeset ltool="$1"
typeset jobq="$2"
if [[ "jobq" == "round_robin" ]] ; then
lic_control="local_ulimit"
else
if [[ -z $(grep "$ltool" "ads.define" | awk -F\: '{print $3}') ]] ; then
print "Please enter the mode of concurrent license control"
print "If you are running LSF, license access is normal control with lsf token."
print "LSF with token is a custom setup! Please consult your Athena representative."
print "We can also control license access via a check before running scheme under lsf."
print "This method is the easiest to configure. Just select local_ulimit to continue"
PS3='Continue? '
select resp in lsf local_ulimit; do
        if [[ -n $resp ]]; then
                RESP=$resp
                break
        else
                print "Invalid Selection"
		tput cup 19 0
		tput el
		tput cup 19 0
        fi
done
case $RESP in
        lsf )
                lic_control="lsf"
                ;;
        local_ulimit )
                lic_control="local_ulimit"
                ;;
esac
else
lic_control=$(grep "$ltool" ads.define | awk -F\: '{print $3}')
print "Using Preconfigured License Control = "$lic_control""
sleep 10
fi

fi
}


function autoconf {

typeset jobq="$2"

typeset callme1=$1
tput cup 11 0
print "Automatic Configuration of $callme1"
sleep 2
typeset LM_LIB=$(env | grep LM_LICENSE_FILE | awk -F\= '{print $2}'| sed -e 's/\:/\ /g')
APP=$(grep "$callme1" ads.define | awk -F\: '{print $2}')
feat_list=""

for application in $APP ; do

        for daemon1 in $LM_LIB ; do
	RESULT=$(../license/bin/lmstat -f $application -c $daemon1 | grep Users | awk -F\: '{print $1}')

        if [[ ! -z "$RESULT" ]] ; then
        feat_list=$(echo $feat_list $APP)
        DAEMON=$daemon1
        fi
        done
done
	if [[ -z "$DAEMON" || -z "$feat_list" ]] ; then
	doconfig "$callme1"
	else
	absolute_path=$(whence $tool)
	tserver=$(echo $DAEMON | awk -F\@ '{print $2}')
	tport=$(echo $DAEMON | awk -F\@ '{print $1}')
		if [[ -z "$tserver" || $tport = +([0-9]) ]] ; then
		print "Not able to configure $callme1 using automatich method!"
		print "Switching to Manual Configuration."
		sleep 20
		clear_lower	
		doconfig "$callme1" "$jobq"
		else
	cpu_control "$callme1" "$jobq"
	ed_tool_conf $callme1 $absolute_path $tserver $tport $lic_control "$feat_list"
		fi
	fi
completed_item=$(echo "$completed_item", "$callme1")
print "Finish $callme1 configuration"
sleep 5
}

function get_tools {
mptype="$1"
tptool="$2"
if [[ -e ads.define ]] ; then
typeset tools=$(cat ads.define | awk -F\: '{print $1}')
else
mptype="None"
tptool="None"
print ""
print "   ERROR: Custom installation definition file missing!"
print "      Configuration will only install the Athena operating platform!"
print "      No third party tools or multi-processing will be configure!"
print ""
PS3='Continue? '
select resp in Continue Exit; do
        if [[ -n $resp ]]; then
                RESP=$resp
                break
        else
                print "Invalid Selection"
		tput cup 17 0
		tput el
		tput cup 17 0
        fi
done
case $RESP in
        Continue )
                ;;
        Exit )
		tput reset
		tput clear
		print "Exit with no configuration update!"
                exit
                ;;
esac

fi

for item in $tools ; do
if [[ -z $(echo "$item" | grep "#") ]] ; then
#list1=$(print -n $list1 $item)
	if [[ "$item" == "lsf" || "$item" == "round_robin" ]] ; then
	mptype="$item"
	mptool="$mptype"
	else
	list1=$(print -n $list1 $item)
	tptool="$list1"
	fi
fi
done

if [[ "$tptool" == "Not Initialize Yet" ]] ; then
	tptool=""
fi
echo "mptype before check = "$mptype""
if [[ "$mptype" == "Not Initialize Yet" ]] ; then
	mptool=""
	mptype=""
fi
completed_item=$(echo Initialization)
}


function read_one1 {
typeset message=$1
tput cup 12 0
print "$message"
read subject1 ignore_others
print ""
print "You enter   $subject1"
print ""
print "Is this Correct?"
PS3='Selection? '
typeset resp=""
select resp in Yes No; do
	if [[ -n $resp ]]; then
		typeset RESP1=$resp
		break
	else
		print "Invalid Selection"
	fi
done
case $RESP1 in
	Yes) return ;;
	No) 
	subject1=""
	clear_lower 12
	tput cup 12 0
	print "                          "
	read_one1 "$message" ;;
esac

}

function round_robin {
typeset RJE="ssh"
tput cup 11 20
#tput smso
print "Round Robin Multi-Processing Configuration"
#tput rmso
tput cup 12 0
#print ""
print "Round Robin multi-processing is a simple replacement for the perferred Grid"
print "engine LSF."
tput cup 13 13
tput smso
print " This configuration exist solely for the purpose of software"
print "evaluation and dedicated resource demo!"
tput rmso
tput cup 14 40
print " Time benchmarking is not guaranteed!"
print "Setup require ssh access to all server with no password requirement. If server"
print "access can not be accomplished without password request, please exit and"
print "resolve passwordless access before processing."
print ""
PS3='Continue? '
typeset resp=""
select resp in Exit Continue ; do
        if [[ -n $resp ]]; then
                typeset RESP=$resp
                break
	else
		print "Invalid Selection"
		tput cup 21 0
		tput el
		tput cup 21 0
        fi
done
case $RESP in
        Exit )
		tput reset
		tput clear
		print "Exiting with no configuration update!"
                exit ;;
        Continue )
                 ;;
esac

# Beginning of server list generation
clear_lower
tput cup 11 0
typeset server_list=""
print "Round Robin Queue = $server_list"
typeset complete="no"
mess1="Please entry name of multi-processing server or type finish to quit"
while [[ "$complete" != "finish" ]] ; do
read_one1 "$mess1"
complete="$subject1"
if [[ "$complete" != "finish" ]] ; then
server_list=$(echo $server_list $subject1)
fi
#tput clear
clear_lower
tput cup 11 0
print "Round Robin Queue = $server_list"
done

# Beginning Server check
tput cup 12 0
print "Configuring Round Robin Queuing with the above list"
sleep 5

for server2 in $server_list ; do
	typeset processor=$(ssh "$server2" "grep processor /proc/cpuinfo | wc -l" 2> /dev/null) 
	typeset PROCESSOR=$( echo $processor | awk '{print $1}')

	while [[ $PROCESSOR -gt 0 ]];do
   		echo $server2 >> "mprocessor_"$PROCESSOR
   		(( PROCESSOR -= 1 ))
	done
done


file_list=$(ls | grep mprocessor_ )
typeset new_server_list=$( cat $file_list )
typeset num_processor=$(echo $new_server_list | wc -w | awk '{print $1}')
rm -rf mprocessor_*
sproduct=$(echo "$product" | tr "[:upper:]" "[:lower:]")
typeset lic_proc_num=$(grep "Multi_Processing" ../conf/ads_license.dat | grep "$sproduct" | awk '{print $6}')
if [[ "num_processor" > "$lic_proc_num" ]] ; then
	typeset NPROC_NUM="$num_processor"
else
	typeset NPROC_NUM="$lic_proc_num"
fi

#tput clear
#tput cup 11 0

print "Updating $product multi-processing configuration for Robin Robin job queuing"
tput cup 14 8
print "rje $RJE"
tput cup 15 8
print "nproc $NPROC_NUM"
tput cup 16 8
print "hosts "$new_server_list""
tput cup 17 0
print "Configuration can be edit later at ../conf/msh.conf"
echo $new_server_list > mprocessor_list
typeset NHOST=$(cat mprocessor_list)
$(rm -rf mprocessor_list)
perl -p -i -e "s/rje \"\"/rje $RJE/" ../conf/msh.conf
perl -p -i -e "s/queue \"\"/nproc $NPROC_NUM/" ../conf/msh.conf
perl -p -i -e "s/nproc \"\"/hosts $NHOST/" ../conf/msh.conf
perl -p -i -e "s/slave_timeout 0//" ../conf/msh.conf
completed_item=$(echo "$completed_item", Round Robin Queuing)

}

function adsconf {
print ""
print "Configuring Athena Top Level Logic"
typeset lpath=$(dirname $(pwd))
#typeset lpath=$(pwd | awk -F\/install '{print $1}')
typeset LPATH=$(echo $lpath | sed -e 's/\//\\\//g')
if [[ -e $lpath/conf/ads_license.dat ]] ; then
typeset integer servercount=$(grep "^SERVER" $lpath/conf/ads_license.dat | wc -l | awk '{print $1}')
case $servercount in
	1)
		typeset lserver=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $2}' )
		typeset port=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $NF}' )
		;;
	3)
		typeset lserver1=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $2}' | head -1)
		typeset lserver2=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $2}' | head -2 | tail -1)
		typeset lserver3=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $2}' | tail -1)
		typeset lserver="$lserver1:$lserver2:$lserver3"
		typeset port1=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $NF}'  | head -1)
		typeset port2=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $NF}' | head -2 | tail -1)
		typeset port3=$( grep "^SERVER" $lpath/conf/ads_license.dat | awk '{print $NF}' | tail -1 )
		typeset port="$port1:$port2:$port3"
		;;
	*)
		echo "Error Case: License server must be triad or standalone"
		exit
		;;
esac

else
print "Athena License file can not be found at ../conf/"
exit

fi

typeset LOGPATH=$(echo $lpath/log/ads_license.log | sed -e 's/\//\\\//g')
perl -p -i -e "s/ads_top \"\"/ads_top $LPATH/" ../conf/msh.conf
perl -p -i -e "s/ads_port \"\"/ads_port $port/" ../conf/msh.conf
perl -p -i -e "s/ads_lic \"\"/ads_lic $lserver/" ../conf/msh.conf
perl -p -i -e "s/ads_log \"\"/ads_log $LOGPATH/" ../conf/msh.conf
completed_item=$(echo "$completed_item", Athena Software)
}


function lsfwrite {
typeset ltool="$1"
typeset lpath="$2"
typeset lserver="$3"
typeset port="$4"
typeset lqueue="$5"
typeset product="$6"
if [[ ! -e ../conf/ads_license.dat ]] ; then
print "Athena License file ads_license.dat can not be found at ../conf/ads_license.dat"
print "msh configuration for nproc will be set to a default of 2"
NPROC="2"
sleep 15
else
NPROC=$(grep Multi_Processing ../conf/ads_license.dat | grep $product | awk '{print $6}')
fi

LPATH=$(echo $lpath | sed -e 's/\//\\\//g')

perl -p -i -e "s/"$ltool"_top \"\"/"$ltool"_top $LPATH/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_port \"\"/"$ltool"_port $port/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_lic \"\"/"$ltool"_lic $lserver/" ../conf/msh.conf

perl -p -i -e "s/rje \"\"/rje $ltool/" ../conf/msh.conf
perl -p -i -e "s/queue \"\"/queue $lqueue/" ../conf/msh.conf
perl -p -i -e "s/nproc \"\"/nproc $NPROC/" ../conf/msh.conf


}


function ed_tool_conf {
typeset ltool="$1"
typeset lpath="$2"
typeset lserver="$3"
typeset port="$4"
typeset lic_control="$5"
# 24 hour timeout with retry every 10 second
typeset timeout=8640
typeset new_list="$6"

typeset LPATH=$(echo $lpath | sed -e 's/\//\\\//g')

if [[ "$lic_control" = "local_ulimit" || "$lic_control" = "limit_fair" ]] ; then
perl -p -i -e "s/"$ltool"_top \"\"/"$ltool"_top $LPATH/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_port \"\"/"$ltool"_port $port/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_lic \"\"/"$ltool"_lic $lserver/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_feat \"\"/"$ltool"_feat \"$new_list\"/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_txout \"\"/"$ltool"_txout $timeout/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_control \"\"/"$ltool"_control $lic_control/" ../conf/msh.conf

else
perl -p -i -e "s/"$ltool"_top \"\"/"$ltool"_top $LPATH/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_port \"\"/"$ltool"_port $port/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_lic \"\"/"$ltool"_lic $lserver/" ../conf/msh.conf
#perl -p -i -e "s/"$ltool"_feat \"\"/"$ltool"_feat \"$new_list\"/" ../conf/msh.conf
perl -p -i -e "s/"$ltool"_control \"\"/"$ltool"_control $lic_control/" ../conf/msh.conf
fi

}


function man_tool_conf {
ltool=$1
lpath=$2
if [[ -z "$lpath" ]] ; then
read_one "Please enter the absolute path for $ltool" PATH
lpath=$(echo $SUBJECT | awk -F\bin '{print $1"bin"}')
#lpath="$SUBJECT"
fi
read_one "Please enter the License Server for $ltool" SERVER
lserver="$SUBJECT"

read_one "Please enter the Communication Port for the License Daemon of $ltool" PORT
port="$SUBJECT"

cpu_control "$ltool"

feature_setup $ltool $port $lserver

virtest=$(grep $ltool ../conf/msh.conf | grep '""' | wc -l | awk '{print $1}')
if [[ ! -z $new_list &&  $(grep $ltool ../conf/msh.conf | grep '""' | wc -l | awk '{print $1}') == "7" ]] ; then
ed_tool_conf "$ltool" "$lpath" "$lserver" "$port" "$lic_control" "$new_list"
else
echo "virgin test failure - Number of $ltool lines = $virtest"
echo "Feature List = $new_list"
echo "If Feature is blank, License daemon is not responding $port@$lserver"
#exit
sleep 5
man_tool_conf "$ltool" "$lpath"
fi

}


function doconfig {
tput cup 12 0
typeset item2="$1"
typeset jobq="$2"
print "$item2 is not enable to run in your current environment!"
PS3='Continue? '
select resp in Exit Continue Interactive ; do
        if [[ -n $resp ]]; then
                RESP=$resp
                break
        else
                print "Invalid Selection"
		tput cup 16 0
		tput el
		tput cup 16 0
        fi
done
case $RESP in
        Exit) print "Exiting with no further configuration update!"
                exit ;;
        Continue)
		print "Continue with no configure for $item2"
		perl -p -i -e "s/"$item2"_top \"\"/"$item2"_top /" ../conf/msh.conf
		perl -p -i -e "s/"$item2"_port \"\"/"$item2"_port /" ../conf/msh.conf
		perl -p -i -e "s/"$item2"_lic \"\"/"$item2"_lic /" ../conf/msh.conf
		perl -p -i -e "s/"$item2"_feat \"\"/"$item2"_feat /" ../conf/msh.conf
		perl -p -i -e "s/"$item2"_txout \"\"/"$item2"_txout /" ../conf/msh.conf
		perl -p -i -e "s/"$item2"_control \"\"/"$item2"_control /" ../conf/msh.conf

		return ;;
        Interactive)
		print "Configuring $item2 interactively"
		man_tool_conf $item2 ;;
esac

}

function header1 {
typeset lmessages="$1"
tput cup 11 0
print "Configuration is automatic when run in an environment enable to run all"
print "third-party tools! Tool must be in the search path and executable without"
print "fully qualified path. If require, environment variable LM_LICENSE_FILE must"
print "also contain the port and license server of the third party tools. The license"
print "file for $product (ads_license.dat) must exist in ../conf directory"
print "You may proceed with the configuration and manually edit the settings later."
#print "   Configuration settings may be change by editing ../conf/msh.conf"
print "   All configuration settings for $product reside in ../conf/msh.conf"
print ""
PS3='Continue? '
select resp in Exit Continue ; do
        if [[ -n $resp ]]; then
                RESP=$resp
                break
        else
                print "Invalid Selection"
		tput cup 21 0
		tput el
		tput cup 21 0
        fi
done
case $RESP in
        Exit ) 
		tput reset
		tput clear
		print "Exiting with no configuration update!"
                exit ;;
        Continue )
                if [[ -e ../conf/msh.conf ]]
                then
                print "A configuration file exist at ../conf"
                print "Renaming to ../msh.conf.bak"
                mv ../conf/msh.conf ../conf/msh.conf.bak
                gunzip -c ./msh.conf_tar.gz | tar xvf - > /dev/null
                mv ./msh.conf ../conf/msh.conf
                else
                gunzip -c ./msh.conf_tar.gz | tar xvf -
                mv ./msh.conf ../conf/msh.conf
                fi ;;
        esac

}

function lsf_conf {
print ""
touch ../log/lsf.log
echo "Function lsf_conf" >> ../log/lsf.log
print "Testing for operation of LSF command lsid"
echo "Testing for operation of LSF command lsid" >> ../log/lsf.log
LSID_TEST=`which lsid 2> /dev/null`
echo "LSID_TEST = $LSID_TEST" >> ../log/lsf.log

if [[ $LSID_TEST = '' ]]
then
print "LSF environment is not enable. You can exit and enable LSF before reruning the"
echo "LSF environment is not enable. You can exit and enable LSF before reruning the" >> ../log/lsf.log
print "configuration or skip LSF configuration for a manual setup later."
echo "configuration or skip LSF configuration for a manual setup later." >> ../log/lsf.log
print ""
PS3='Continue? '
typeset resp=""
select resp in skip exit ; do
        if [[ -n $resp ]]; then
                typeset RESP=$resp
                break
        else
                print "Invalid Selection"
		tput cup 17 0
		tput el
		tput cup 17 0
        fi
done
case $RESP in
        skip )
		print "Continue with Configuration!"
		print "Will configure LSF Manually!"
		perl -p -i -e "s/rje \"\"/rje /" ../conf/msh.conf
		perl -p -i -e "s/queue \"\"/nproc /" ../conf/msh.conf
		perl -p -i -e "s/lsf_top \"\"/lsf_top /" ../conf/msh.conf
		perl -p -i -e "s/lsf_port \"\"/lsf_port /" ../conf/msh.conf
		perl -p -i -e "s/lsf_lic \"\"/lsf_lic /" ../conf/msh.conf
		echo "Continue by skipping LSF configuration" >> ../log/lsf.log
		return
		clear_lower
                ;;
        exit )
                print "Exiting to resolve LSF environment."
		echo "Exit from configure.ksh without LSF configuration" >> ../log/lsf.log
		exit 1
                ;;
esac

else
echo "LSF environment is enabled!" >> ../log/lsf.log
LSIDPATH=$(which lsid)
echo "LSIDPATH = $LSIDPATH" >> ../log/lsf.log
VERSION=$(lsid | head -1  | cut -d"," -f1 | awk '{print $NF}')
echo "VERSION = $VERSION" >> ../log/lsf.log
BTOP=$(echo $LSIDPATH | awk -F/$VERSION '{print $1}')
echo "BTOP = $BTOP" >> ../log/lsf.log
path2_lsf_confd=$(env | grep LSF_ENVDIR | awk -F\= '{print $NF}')
echo "path2_lsf_confd = $path2_lsf_confd" >> ../log/lsf.log
	if [[ -e "$path2_lsf_confd/lsf.conf" ]] ; then
		LPATH=`grep LICENSE_FILE $path2_lsf_confd/lsf.conf | cut -d"=" -f2`
		echo "LPATH = $LPATH" >> ../log/lsf.log
		LSFLIC=`grep SERVER $LPATH | awk '{print $2}'`
		echo "LSFLIC = $LSFLIC" >> ../log/lsf.log
		LSFPORT=`grep SERVER $LPATH | awk '{print $4}'`
		echo "LSFPORT = $LSFPORT" >> ../log/lsf.log
		if [[ -z "$LSFLIC" && -z "$LSFPORT" ]] ; then
			print "Problem locating LSF configuration Component"
			print "Switching to Pass-Thru mode"
			print ""
			echo "Problem locating LSF configuration component" >> ../log/lsf.log
			echo "lsf_lic = $LSFLIC" >> ../log/lsf.log
			echo "lsf_port =$LSFPORT" >> ../log/lsf.log
			echo "Setting configuration for Pass-Thru mode" >> ../log/lsf.log
			LSFLIC="Pass-Thru"
			LSFPORT="Pass-Thru"
		fi
        else
		print "Problem locating LSF configuration directory"
		echo "Not able to locate LSF configuration directory" >> ../log/lsf.log
		exit 1
	fi

	if [[ ! -e $path2_lsf_confd/profile.lsf ]] ; then
	print "LSF environment wrapper profile.lsf do not exist! Profile.lsf is required for $product. Please contact your LSF administrator for further assistance."
	echo "Can not locate profile.lsf in directory $path2_lsf_confd" >> ../log/lsf.log
	exit 1
	fi
print "LSF environment is enabled: "
print " lsf_top $BTOP           # Top of LSF tree"
print " lsf_lic $LSFLIC                 # LSF license server"
print " lsf_port $LSFPORT                       # LSF license port"
print "The above lsf configuration will be written to ../conf/msh.conf at the"
print "end of the configuration"
sleep 5
clear_lower
print ""
AVAILQUEUE=`bqueues | grep Open:Active | awk '{print $1}'`
print "Please select a LSF queue for running $product."
print "Control-c to exit without writing"
PS3='LSF Queue? '

select queue in $AVAILQUEUE; do
        if [[ -n $queue ]]; then
                Q=$queue
                print Default LSF Queue for $product = $Q
                break
        else
                print 'invalid selection'
        fi
done
echo "BTOP - $BTOP" >> ../log/lsf.log
echo "LSFLIC - $LSFLIC" >> ../log/lsf.log
echo "LSFPORT - $LSFPORT" >> ../log/lsf.log
echo "Queue - $Q" >> ../log/lsf.log
echo "Product - $product" >> ../log/lsf.log
lsfwrite lsf $BTOP $LSFLIC $LSFPORT $Q $product

fi
completed_item=$(echo "$completed_item", LSF)

# setup NPROC
echo "Beginning of NPROC setup" >> ../log/lsf.log
sproduct=$(echo "$product" | tr "[:lower:]" "[:upper:]")
echo "Sproduct = $sproduct" >> ../log/lsf.log
if [[ $product == "AIM" ]] ; then
typeset i lic_proc_num=$(grep "Multi_Processing" ../conf/ads_license.dat | grep "$sproduct" | awk '{print $6}')
echo "lic_proc_num = $lic_proc_num" >> ../log/lsf.log
if [[ ! -z "$lic_proc_num" ]] ; then
	echo "Setting up NPROC in ../conf/msh.conf" >> ../log/lsf.log
	perl -p -i -e "s/nproc/nproc $lic_proc_num /" ../conf/msh.conf
#	perl -p -i -e "s/nproc \"\"/nproc $lic_proc_num /" ../conf/msh.conf
else
	echo "Problem setting NPROC" >> ../log/lsf.log
	echo "NPROC = $lic_proc_num" >> ../log/lsf.log
	print "Unable to setup nproc"
	sleep 5
fi
fi
echo "Completion of LSF configuration" >> ../log/lsf.log

}

function read_one {
clear_lower
tput cup 12 0
message=$1
op_class=$2
print "$message"
case $op_class in
	PATH)
	read subject1 ignore_others
	if [[ -e $subject1 && ! -z $(exec $subject1 -version | grep version) ]] ; then
	SUBJECT=$subject1
	return
	else
	print "Invalid Selection"
	sleep 5
	read_one "$message" "$op_class"
	fi
	;;
	PORT)
	read subject1 ignore_others
	if [[ -n $subject1 ]] ; then
	SUBJECT=$subject1
	return
	else
	print "Invalid Selection"
	sleep 5
	read_one "$message" "$op_class"
	fi
	;;
	SERVER)
	read subject1 ignore_others
	if [[ -z $(ping -c 2 "$subject1" | grep "unknown host") ]] ; then
	SUBJECT=$subject1
	return
	else
	print "Invalid Selection"
	sleep 5
	read_one "$message" "$op_class"
	fi
	;;
esac
	
}


# Start of Installation
# set third party tools install list
if [[ -e ../ads/productid ]] ; then
product=$(head -1 ../ads/productid | awk -F\- '{print $1}')
else
product="Athena Software"
fi
completed_item="None"
current_task="Introduction"
mptype="Not Initialize Yet"
tptool="Not Initialize Yet"
menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
header1 "$install_list"
current_task="Initialization"
menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
get_tools "$mptype" "$tptool"
install_list="$(echo $list1)"
current_task="Athena Software"
menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
# Configure Athena Software Tree
adsconf
print "Completed Athena Software Configuration"
sleep 5

if [[ -z "$mptype" ]] ; then
menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
tput cup 11 0
print "No multi-processing queuing architecture selected!"
print "You must configure multi-processing queuing manually"
sleep 15
else
for items in $mptype ; do
	if [[ "$items" ==  "round_robin" ]] ; then
	current_task="Round Robin Configuration"
	menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
	round_robin
	print "Completed Multi-processing Configuration"
	fi
	if [[ "$items" == "lsf" ]] ; then
	current_task="lsf"
	menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
	lsf_conf
	print "Completed Multi-processing Configuration"
	fi
done
fi
sleep 5
if [[ ! -z $(echo "$tptool" ) ]] ; then
for tool in $tptool ; do
	
	current_task="$tool"
	menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
	if [[ -z $(whence "$tool") ]] ; then
	doconfig $tool $mptype
	else
# Add pass-thru
#
	is_pass_thru=$(grep "^$tool" ads.define | awk -F\: '{print $3}')
		if [[ "$is_pass_thru" == "setup-pass" ]] ; then
		opt_setup_pass $tool $mptype
		current_task="$tool"
		menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
		else
		autoconf $tool $mptype
		current_task="$tool"
		menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
		fi
	fi
done
fi
sleep 5
current_task="Finalizing Current Configuration!"
menu_master "$mptype" "$tptool" "$completed_item" "$current_task"
tput cup 11 0
print "Finalizing Configuration"
#del_blank_line
if [[ "$product" == "Aim" ]] ; then
perl -p -i -e "s/slave_timeout 0//" ../conf/msh.conf
perl -p -i -e "s/site_conf user//" ../conf/msh.conf
perl -p -i -e "s/\# global\/user global\=resource queuing//" ../conf/msh.conf
fi
del_blank_line
sed '$!N; /^\(.*\)\n\1$/!P; D' ../conf/msh.conf > ../conf/msh.conf.tmp
integer diffout=$(diff ../conf/msh.conf ../conf/msh.conf.tmp | wc -l | awk '{print $1}')
if [[ "$diffout" < "6" ]] ; then
mv ../conf/msh.conf ../conf/msh.conf.orig
mv ../conf/msh.conf.tmp ../conf/msh.conf
rm -rf ../conf/msh.conf.orig
fi
sleep 10
tput reset
tput clear
print "Finished - Completed"
	cat ../conf/msh.conf
