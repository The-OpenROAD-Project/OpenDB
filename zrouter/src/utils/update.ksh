#!/bin/ksh -f

function die {
        echo "update.ksh: $*" 1>&2
        exit 1
}

function lic_daemon_update {
echo "lic_daemon_update: " >> $LOGGING_TREE/update.log
typeset in_tree="$1"
echo "install tree = $in_tree" >> $LOGGING_TREE/update.log

if [[ -e $in_tree/conf/ads_license.dat ]] ; then
	typeset lserver=$(grep "^SERVER" $in_tree/conf/ads_license.dat | awk '{print $2}')
	echo "license server = $lserver" >> $LOGGING_TREE/update.log
	typeset cur_server=$(hostname)
	echo "current server = $cur_server" >> $LOGGING_TREE/update.log
	if [[ "$lserver" == "$cur_server" ]] ; then
	$in_tree/license/bin/lmreread -c $in_tree/conf/ads_license.dat
	echo "Updating License Daemon " >> $LOGGING_TREE/update.log
	else
	print "$lserver is not the same as $cur_server"
	print "Please update license daemon manually"
	echo "$lserver is not the same as $cur_server" >> $LOGGING_TREE/update.log
	echo "Please update license daemon manually" >> $LOGGING_TREE/update.log
	fi
else
	print "License File is not at $in_tree/conf/ads_license.dat"
	print "Please update license daemon manually"
	echo "License File is not at $in_tree/conf/ads_license.dat" >> $LOGGING_TREE/update.log
	echo "Please update license daemon manually" >> $LOGGING_TREE/update.log
fi
}

function get_install_info {
echo "get_install_info: " >>  $LOGGING_TREE/update.log
typeset ntree="$1"
echo "get_install_info: newtree = "$ntree"" >>  $LOGGING_TREE/update.log

if [[ -e "$ntree" ]] ; then
        if [[ -e "$ntree/install" ]] ; then
                if [[ -e "$ntree/install/update.define" ]] ; then
		echo ""$ntree/install/update.define" exist" >>  $LOGGING_TREE/update.log
                install_list=$( grep "^ads:" "$ntree/install/update.define" | awk -F\ads: '{print $NF}' | sed -e 's/\:/\ /g' )
                else
		echo ""$ntree/install/update.define" do not exist" >> $LOGGING_TREE/update.log
                print ""$ntree/install/update.define" do not exist"
                print "Setting Install List to default"
                print "ADS software tree only"
		echo "ADS software tree only" >> $LOGGING_TREE/update.log
                typeset install_list="ads"
                fi
        else
        print "Error: "$ntree/install" do not exist"
        fi
else
$(clear_lower)
echo "get_install_info: call to clear_lower" >> $LOGGING_TREE/update.log
print "New installation tree "$ntree do not exist""
mess1="Please enter correct absolute path to new installation tree"
read_one "$mess1" PATH
echo "get_install_info: Acquire New path info" >> $LOGGING_TREE/update.log
correct_path="$SUBJEST"
get_install_info $correct_path
fi

# Initialize  Menu display variables
ads_version="none"
license1="none"
stool="none"
Install_list23=""
echo "INSTALL_LIST = $install_list" >> $LOGGING_TREE/update.log
for itemx in $install_list
do
	case "$itemx" in
		ads)
		ads_version=$(cat "$ntree/ads/productid")
		Install_list23=$(echo "$Install_list23" "$itemx")
			;;
		license)
			license1=$(grep "^license:" "$ntree/install/update.define" | cut -d":" -f2- | sed -e 's/\:/\ /g')
			if [[ -z "$license1" ]] ; then
			license1="None"
			else
			Install_list23=$(echo "$Install_list23" "$license1")
			fi
			;;
		*)
			if [[ "$stool" != "none" ]] ; then
			stool=$(echo "$stool" "$itemx")
			else
			stool=$(echo "$itemx")
			fi
			Install_list23=$(echo "$Install_list23" "$itemx")
			;;
	
	esac
done

echo "Menu License Variable = "$license1"" >> $LOGGING_TREE/update.log
echo "Menu Support Tools = "$stool"" >> $LOGGING_TREE/update.log
}

function update_now {
typeset ntree="$1"
typeset otree="$2"
lcompletion=""
typeset productid=$(cat "$otree/ads/productid")

if [[ -e "$ntree/install/update.define" ]] ; then
# Remove
echo "install_list = $install_list" >> $LOGGING_TREE/update.log
echo "install_list23 = $Install_list23" >> $LOGGING_TREE/update.log
echo "stool_list = $stool" >> $LOGGING_TREE/update.log
# Remove
ulist=$(echo $Install_list23)
##ulist=$(echo $stool)
else
ulist="ads"
fi
echo "Inside of update_now function - update list = "$ulist"" >> $LOGGING_TREE/update.log

for updat in $ulist
do
UPDAT=$(echo $updat)
case $UPDAT in
        "ads")
		echo "Inside of ads subroutine" >> $LOGGING_TREE/update.log
		menu_master verify_update_version
		if [[ $(check_build_time "$prod_timestamp" "$new_prod_timestamp") != "Not okay to upate" ]] ; then
		clear_lower
		menu_master "ADS tree Update"
                print "Updating ads software subtree"
                if [[ -w "$otree" ]] ; then
                        if [[ -e "$otree/archive/$productid" ]] ; then
                                if [[ ! -e "$otree/archive/$productid/ads" ]] ; then
                                $(mv "$otree/ads" "$otree/archive/$productid/")
                                TTOUT=$(rsync -av "$ntree/ads" "$otree")
                                else
                                $(mkdir -p $otree/archive/$productid)
                                TTOUT=$(rsync -av "$ntree/ads" "$otree")
                                fi
                        else
                        $(mkdir -p $otree/archive/$productid)
                        $(mv "$otree/ads" "$otree/archive/$productid/")
                        TTOUT=$(rsync -av "$ntree/ads" "$otree")
                        fi
                else
                print "No write permission for $otree"
                fi
		else
		print "New Version and Original Version are identical"
		print "No update performed"
		fi
		lcompletion=$(echo "$lcompletion" "Athena Software" )
		sleep 5
                ;;
        "license_file")
		echo "license_file update" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "license_file"
		if [[ -e "$(dirname $ntree)/ads_license.dat" && -w "$otree/conf/ads_license.dat" ]] ; then
		nhost=$( grep "^SERVER" "$(dirname $ntree)/ads_license.dat" | awk '{print $2}')
		ohost=$( grep "^SERVER" "$otree/conf/ads_license.dat" | awk '{print $2}')
		nid=$( grep "^SERVER" "$(dirname $ntree)/ads_license.dat" | awk '{print $3}')
		oid=$( grep "^SERVER" "$otree/conf/ads_license.dat" | awk '{print $3}')
		echo "New hostname: $nhost" >> $LOGGING_TREE/update.log
		echo "Old hostname: $ohost" >> $LOGGING_TREE/update.log
		echo "New hostid: $nid" >> $LOGGING_TREE/update.log
		echo "Old hostid: $oid" >> $LOGGING_TREE/update.log

			if [[ "$nid" == "$oid" ]] ; then
        			if [[ "$nhost" != "$ohost" ]] ; then
        			echo "Updating hostname" >> $LOGGING_TREE/update.log
        			perl -p -i -e "s/SERVER $nhost/SERVER $ohost/" "$(dirname $ntree)/ads_license.dat"
        			fi
			todate=`grep "$PRODUCT"_Master "$otree/conf/ads_license.dat" | awk '{print $5}'`
			echo "Old Master Feature Date = "$todate"" >> $LOGGING_TREE/update.log
			odate=$(echo "$todate" | awk -F\- '{print $1}')
			omonth=$(echo "$todate" | awk -F\- '{print $2}')
			oyear=$(echo "$todate" | awk -F\- '{print $3}')
			tndate=`grep "$PRODUCT"_Master "$(dirname $ntree)/ads_license.dat" | awk '{print $5}'`
			echo "New Master Feature Date = "$tndate"" >> $LOGGING_TREE/update.log
			ndate=$(echo "$tndate" | awk -F\- '{print $1}')
			nmonth=$(echo "$tndate" | awk -F\- '{print $2}')
			nyear=$(echo "$tndate" | awk -F\- '{print $3}')
			if [[ ! -z "$nyear" && ! -z "$oyear" && "$nyear" > "$oyear" ]] ; then
			print "Updating license file"
			echo "Updating license file 1" >> $LOGGING_TREE/update.log
			tdate1=$(date '+%m%d%H%M')
			$(mv "$otree/conf/ads_license.dat" "$otree/conf/ads_license.dat$tdate1")
			$(cp "$(dirname $ntree)/ads_license.dat" "$otree/conf/ads_license.dat")
			lic_daemon_update "$otree"
			elif [[ ! -z "$nyear" && ! -z "$oyear" && "$nmonth" > "$omonth" ]] ; then
			print "Updating license file"
			echo "Updating license file 2" >> $LOGGING_TREE/update.log
			tdate1=$(date '+%m%d%H%M')
			$(mv "$otree/conf/ads_license.dat" "$otree/conf/ads_license.dat$tdate1")
			$(cp "$(dirname $ntree)/ads_license.dat" "$otree/conf/ads_license.dat")
			lic_daemon_update "$otree"
			elif [[ ! -z "$nyear" && ! -z "$oyear" && "$ndate" > "$odate" ]] ; then 
			print "Updating license file"
			echo "Updating license file 3" >> $LOGGING_TREE/update.log
			tdate1=$(date '+%m%d%H%M')
			$(mv "$otree/conf/ads_license.dat" "$otree/conf/ads_license.dat$tdate1")
			$(cp "$(dirname $ntree)/ads_license.dat" "$otree/conf/ads_license.dat")
			lic_daemon_update "$otree"
			else
			print "Master Feature in new license is not newer!"
			print "No license Update"
			fi
			
			else
			echo "Hostid do match" >> $LOGGING_TREE/update.log
			print "Hostid do not Match, Skipping License update"
			fi
		else
		print "New ads_license.dat do not exist or old ads_license.dat is not writable!"
		echo "New ads_license.dat do not exist or old ads_license.dat is not writable!" >> $LOGGING_TREE/update.log
		fi

		lcompletion=$(echo "$lcompletion" ", license_file" )
                ;;
	"license_tree")
		echo "Inside of license_tree subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "license_tree"
		echo "Updating License Tree"
                if [[ -w "$otree" ]] ; then
		echo "otree = $otree" >> $LOGGING_TREE/update.log
		echo "$otree is writable" >> $LOGGING_TREE/update.log
		echo "productid = $productid" >> $LOGGING_TREE/update.log
                        if [[ -e "$otree/archive/$productid/license" ]] ; then
			echo "$$otree/archive/$productid/license already exist" >> $LOGGING_TREE/update.log
                       	$(mv "$otree/license"  "$otree/archive/$productid/")
                       	TTOUT=$(rsync -av "$ntree/license" "$otree")
			else
			echo "$$otree/archive/$productid/license do not exist" >> $LOGGING_TREE/update.log
			$(mkdir -p $otree/archive/$productid)
			$(mv "$otree/license" "$otree/archive/$productid/")
			TTOUT=$(rsync -av "$ntree/license" "$otree")
			fi
		else
                print "No write permission for $otree"
		echo "No write permission for $otree" >> $LOGGING_TREE/update.log
                fi
		lcompletion=$(echo "$lcompletion" "License_tree" )
		sleep 5

		;;
        pubtools)
		echo "Inside of pubtools subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "pubtools tree update"
                print "Updating pubtools tree by renaming old and updating with new"
		if [[ -w "$otree" ]] ; then
		echo "$otree is writable" >> $LOGGING_TREE/update.log
                	if [[ -e "$otree/archive/$productid/pubtools" ]] ; then
			echo "$otree/archive/$productid/pubtools already exist" >> $LOGGING_TREE/update.log
                        $(mv "$otree/pubtools"  "$otree/archive/$productid/pubtools$productid")
                        TTOUT=$(rsync -av "$ntree/pubtools" "$otree")
                        else
			echo "$otree/archive/$productid/pubtools do not exist" >> $LOGGING_TREE/update.log
			$(mkdir -p $otree/archive/$productid)
			$(mv "$otree/pubtools"  "$otree/archive/$productid/pubtools")
			TTOUT=$(rsync -av "$ntree/pubtools" "$otree")
                        fi
                fi
		lcompletion=$(echo "$lcompletion" "pubtools" )
		sleep 5
                ;;
        lib)
		echo "Inside of lib subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "lib_tree update"
                print "Updating lib_tree by renaming old and updating with new"
		if [[ -w "$otree" ]] ; then
		echo "$otree is writable" >> $LOGGING_TREE/update.log
                	if [[ -e "$otree/archive/$productid/lib" ]] ; then
			echo "$otree/archive/$productid/lib already exist" >> $LOGGING_TREE/update.log
                        $(mv "$otree/lib"  "$otree/archive/$productid/lib$productid")
                        TTOUT=$(rsync -av "$ntree/lib" "$otree")
                        else
			echo "$otree/archive/$productid/lib do not exist" >> $LOGGING_TREE/update.log
			$(mkdir -p $otree/archive/$productid)
			$(mv "$otree/lib"  "$otree/archive/$productid/lib")
			TTOUT=$(rsync -av "$ntree/lib" "$otree")
                        fi
                fi
		lcompletion=$(echo "$lcompletion" "lib" )
		sleep 5
                ;;
        doc)
		echo "Inside of doc subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "Documentation Update"
                print "Updating documentation tree"
                if [[ ! -z $( ls $ntree/doc ) && -w "$otree/doc" ]] ; then
                doclist=$(ls $ntree/doc )
                for IDOC in $doclist
                do
                idoc=$(echo $IDOC)
                if [[  ! -e "$otree/doc/$idoc" ]] ; then
                $(cp "$ntree/doc/$idoc" "$otree/doc")
                else
                        if [[ ! -z $(diff "$ntree/doc/$idoc" "$otree/doc/$idoc") ]] ; then
                                if [[ -e $otree/archive/$productid/doc ]] ; then
                                $(mv "$otree/doc/$idoc" "$otree/archive/$productid/doc")
                                $(cp "$ntree/doc/$idoc" "$otree/doc")
                                else
                                $(mkdir -p $otree/archive/$productid/doc)
                                $(mv "$otree/doc/$idoc" "$otree/archive/$productid/doc")
                                $(cp "$ntree/doc/$idoc" "$otree/doc")
                                fi
                        else
                        print "$ntree/doc/$idoc and $otree/doc/$idoc is identical"
                        print "Skipping Update"
                        fi
                fi
                done
                fi
		lcompletion=$(echo "$lcompletion" "Doc" )
		sleep 5
                ;;
        design)
		echo "Inside of design subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "Design_Update"
                print "Updating design tree"
                if [[ ! -z $( ls $ntree/design ) && -w "$otree/design" ]] ; then
		echo "$ntree/design is not empty and $otree/design is writable" >> $LOGGING_TREE/update.log
                designlist=$(ls $ntree/design )
                for IDESIGN in $designlist
                do
                idesign=$(echo $IDESIGN)
                if [[  ! -e "$otree/design/$idesign" ]] ; then
                $(cp "$ntree/design/$idesign" "$otree/design")
		echo "$idesign do not exist in the original tree, copy from newtree" >> $LOGGING_TREE/update.log
                else
                        if [[ ! -z $(diff "$ntree/design/$idesign" "$otree/design/$idesign") ]] ; then
                                if [[ -e $otree/archive/$productid/design ]] ; then
				echo "$otree/archive/$productid/design exist" >> $LOGGING_TREE/update.log
                                $(mv "$otree/design/$idesign" "$otree/archive/$productid/design")
                                $(cp "$ntree/design/$idesign" "$otree/design")
                                else
				echo "$otree/archive/$productid/design do not exist" >> $LOGGING_TREE/update.log
                                $(mkdir -p $otree/archive/$productid/design)
                                $(mv "$otree/design/$idesign" "$otree/archive/$productid/design")
                                $(cp "$ntree/design/$idesign" "$otree/design")
                                fi
                        else
                        print "$ntree/design/$idesign and $otree/design/$idesign is identical"
                        print "Skipping Update"
                        echo "$ntree/design/$idesign and $otree/design/$idesign is identical" >> $LOGGING_TREE/update.log
                        echo "Skipping Update" >> $LOGGING_TREE/update.log
                        fi
                fi
                done
                fi
		lcompletion=$(echo "$lcompletion" "Doc" )
		sleep 5
                ;;
        install)
		echo "Inside of install subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "Installation tools directory update"
                nlist=$(ls "$ntree/install")
                for mytool in $nlist
                do
                MYT=$(echo $mytool)
                if [[ -e "$otree/install/$MYT" ]] ; then
                        if [[ ! -z $(diff "$ntree/install/$MYT" "$otree/install/$MYT") ]] ; then
                                if [[ -e $otree/archive/$productid/install ]] ; then
                                $(mv "$otree/install/$MYT" "$otree/archive/$productid/install")
                                $(cp "$ntree/install/$MYT" "$otree/install")
                                else
                                $(mkdir -p $otree/archive/$productid/install)
                                $(mv "$otree/install/$MYT" "$otree/archive/$productid/install")
                                $(cp "$ntree/install/$MYT" "$otree/install")
                                fi
#                        else
#                                print ""$ntree/install/$MYT" "$otree/install/$MYT" are identical"
#                                print "Skipping Upgrade"
                        fi
                else
                $(cp "$ntree/install/$MYT" "$otree/install")
                fi
                done
		lcompletion=$(echo "$lcompletion" "install_tree" )
		sleep 5
                ;;
        bin)
		echo "Inside of bin subroutine" >> $LOGGING_TREE/update.log
		clear_lower
		menu_master "Top level bin directory update"
                blist=$(ls "$ntree/bin")
                for mytool in $blist
                do
                MYIT=$(echo $mytool)
                if [[ -e "$otree/bin/$MYIT" ]] ; then
                        if [[ ! -z $(diff "$ntree/bin/$MYIT" "$otree/bin/$MYIT") ]] ; then
                                if [[ -e $otree/archive/$productid/bin ]] ; then
                                $(mv "$otree/bin/$MYIT" "$otree/archive/$productid/bin")
                                $(cp "$ntree/bin/$MYIT" "$otree/bin")
                                else
                                $(mkdir -p $otree/archive/$productid/bin)
                                $(mv "$otree/bin/$MYIT" "$otree/archive/$productid/bin")
                                fi
                        else
                        print ""$ntree/bin/$MYIT" "$otree/bin/$MYIT" are identical"
                        print "Skipping Upgrade"
			echo ""$ntree/bin/$MYIT" "$otree/bin/$MYIT" are identical" >> $LOGGING_TREE/update.log
			echo "Skipping Upgrade" >> $LOGGING_TREE/update.log
                        fi
                else
                $(cp "$ntree/bin/$MYIT" "$otree/bin")
                fi
                done
		lcompletion=$(echo "$lcompletion" "toplevel_bin" )
		sleep 5
                ;;
        *)
		echo "Should never be here variable = $UPDAT" >> $LOGGING_TREE/update.log
		echo "Inside of default subroutine - should never be here" >> $LOGGING_TREE/update.log
                print "Should never go here"
                ;;

esac

done
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
typeset lproduct="$product"
typeset ltask="$1"
echo "" >> $LOGGING_TREE/update.log
echo "manu_master: Current Task = "$ltask"" >> $LOGGING_TREE/update.log
echo "manu_master: Athena Software Update Version = "$ads_version"" >> $LOGGING_TREE/update.log
echo "manu_master: License Upgrade = "$license1"" >> $LOGGING_TREE/update.log
echo "manu_master: Support Tools Upgrade = "$stool"" >> $LOGGING_TREE/update.log
echo "manu_master: Completed Upgrades = "$lcompletion"" >> $LOGGING_TREE/update.log
tput init
tput setb 3
tput clear
tput cup 0 30
print "$lproduct Software Update"
print "________________________________________________________________________________"
print "Upgrade Components: Athena Software, License, and support tools"
tput cup 3 4
print "Athena Software: $ads_version"
tput cup 4 4
print "License: "$license1""
tput cup 5 4
print "Support Tools: "$stool""
tput cup 7 0
print "Completion: "$lcompletion""
tput cup 8 0
print "Current Task: "$ltask""
print "________________________________________________________________________________"
tput setb 7
typeset lcount=10
while [[ "$lcount" -lt "24" ]] ; do
        tput cup "$lcount" 0
        tput el
        (( lcount += 1 ))
done
tput cup 11 0

}

function read_one {
echo "Inside read_one function" >> $LOGGING_TREE/update.log
clear_lower
tput cup 12 0
message=$1
echo "read_one: message = $message" >> $LOGGING_TREE/update.log
op_class=$2
print "$message"
case $op_class in
        PATH)
        read subject1 ignore_others
        if [[  -e "$subject1/ads/productid" && ! -z $(grep "$product" "$subject1/ads/productid") ]] ; then
        SUBJECT=$subject1
	echo "read_one: SUBJECT = $SUBJECT" >> $LOGGING_TREE/update.log
        return
        else
        print "Invalid Input"
	echo "read_one: Invalid Input" >> $LOGGING_TREE/update.log
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


function check_build_time {
typeset orig_stamp="$1"
typeset new_stamp="$2"

new_month=$(echo $new_stamp | cut -c1-2)
new_date=$(echo $new_stamp | cut -c3-4)
new_year=$(echo $new_stamp | cut -c5-6)

orig_month=$(echo $orig_stamp | cut -c1-2)
orig_date=$(echo $orig_stamp | cut -c3-4)
orig_year=$(echo $orig_stamp | cut -c9-10)

if [[ "$new_month" > "$orig_month" ]] ; then
echo "OKAY"
elif [[ "$new_month" == "$orig_month" && "$new_date" > "$orig_date" ]] ; then
echo "OKAY"
elif [[ "$new_month" < "$orig_month" && "$new_year" > "$orig_year" ]] ; then
echo "OKAY"
else
echo "Not okay to upate"
fi
}


#################### Start ##################################
is_okay=""
TOP=$(dirname $(dirname $0))
if [[ $TOP = . ]]
then
        TOP=$(dirname $(pwd))
fi
LOGGING_TREE="$TOP/log"
echo "Top of Directory Tree = $TOP" > $LOGGING_TREE/update.log
# Check for existence of current branch productid
# If exist set product and product release timestamp
if [[ -e "$TOP"/ads/productid && $( wc -l "$TOP"/ads/productid | awk '{print $1}') == "1" ]] ; then
product=$(cat "$TOP"/ads/productid | awk -F"-" '{print $1}')
PRODUCT=$(echo "$product" | tr "[:lower:]" "[:upper:]")
echo "product = "$product"" >> $LOGGING_TREE/update.log
prod_timestamp=$(cat "$TOP"/ads/productid | awk -F"-" '{print $2}')
echo "prod_timestamp = "$prod_timestamp"" >> $LOGGING_TREE/update.log
else
echo "Update.ksh: Can not location Product ID" $LOGGING_TREE/update.log
die "Update.ksh: Can not location Product ID"
fi

# Check if $TOP is the new branch or old branch
#tes11=$(grep ads "$TOP"/conf/msh.conf | grep "\"\"" | wc -l )
if [[ -e "$TOP"/conf/msh.conf && $( grep ads "$TOP"/conf/msh.conf | grep "\"\"" | wc -l ) > "3" ]] ; then
# Branch is Original
echo "orig_tree = $TOP" >> $LOGGING_TREE/update.log
orig_tree="$TOP"
# Acquire New Tree info
sleep 05
menu_master get_install_info
mess1="What is absolute path to the new upgrade tree?"
read_one "$mess1" PATH
new_tree="$SUBJECT"
new_prod_timestamp=$(cat "$new_tree"/ads/productid | awk -F"-" '{print $2}')
else
# Branch is new
new_tree="$TOP"
clear_lower
menu_master get_install_info
echo "get_install_info "$new_tree"" >>  $LOGGING_TREE/update.log
get_install_info "$new_tree"

sleep 5
menu_master get_original_tree
echo "What is absolute path to the orig "$product" tree?" >> $LOGGING_TREE/update.log
mess1="What is absolute path to the orig "$product" tree?"
read_one "$mess1" PATH
orig_tree="$SUBJECT"
prod_timestamp=$(cat "$orig_tree"/ads/productid | awk -F"-" '{print $2}')
new_prod_timestamp=$(cat "$new_tree"/ads/productid | awk -F"-" '{print $2}')
echo "Top: orig_tree = "$SUBJECT"" >> $LOGGING_TREE/update.log
echo "Top: prod_timestamp = "$prod_timestamp"" >> $LOGGING_TREE/update.log
echo "Top: new_prod_timestamp = "$new_prod_timestamp"" >> $LOGGING_TREE/update.log
fi
sleep 5
clear_lower
menu_master update_software
update_now "$new_tree" "$orig_tree"
sleep 15
menu_master "Completed"
