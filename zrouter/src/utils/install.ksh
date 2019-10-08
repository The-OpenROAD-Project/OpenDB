#!/bin/ksh -p

function die {
        echo "install.ksh: $*" 1>&2
        exit 1
}

# Initial install log
$(date >> install_log.txt ; id >> install_log.txt)

# Check for blank ../ directory
#
bdir_check=$(ls ../)
if [[ "$bdir_check" != "milos_install" ]] ; then
echo "Install directory ../ is not clean" >> install_log.txt
echo "ls ../ return - $bdir_check" >> install_log.txt
bdir_path=$(dirname $(pwd))
print "Installation directory $bdir_path contain information not included "
print "in the release tree! Do you still wish to proceed."
PS3='Continue? '
select resp in exit okay_proceed; do
        if [[ -n $resp ]]; then
                RESP=$resp
                break
        fi
done

case $RESP in
        exit )
                exit;
                ;;
        okay_to_proceed )
                ;;
esac
else
echo "Install directory is clean" >> install_log.txt
echo "ls ../ return - $bdir_check" >> install_log.txt
fi

echo "Completed Install Directory Check"

# untar release image
#
rel_image=$(ls *.bz2)
if [[ -z "$rel_image" ]] ; then
echo "No Release Image in current directory" >> install_log.txt
echo "Release Image check return - $rel_image " >> install_log.txt
print "No release image found in current directory"
print "Exiting"
else
echo "Release Image is found in current directory" >> install_log.txt
echo "Release Image check return - $rel_image " >> install_log.txt
fi
timage=$(echo $rel_image | awk -F\.bz2 '{print $1}')
setup_image=$( cd .. ; bunzip2 -c milos_install/$rel_image | tar xvf - )
echo $setup_image >> install_log.txt

echo "Completed Image setup"

# Check for included license file
#
if [[ -r ./ads_license.dat && -s ./ads_license.dat ]] ; then
$(cp ./ads_license.dat ../Milos-v*/conf)
echo "Installation license file ads_license.dat" >> install_log.txt
$(cat ./ads_license.dat >> install_log.txt )
echo "Completed ads_license.dat installation." >> install_log.txt
else
echo "./ads_license.dat do not exist - no license installation" >> install_log.txt
fi

# Check Third Party Tool
#
rtree=$(ls ../ | grep Milos-v)
if [[ -e ./ads.define ]] ; then
$(cp -f ./ads.define ../$rtree/install/ads.define)
echo "copy local ads.define to distribution" >> install_log.txt
$(cat ./ads.define >> install_log.txt)
else
echo "no local ads.define file" >> install_log.txt
fi

if [[ -r ../$rtree/install/ads.define ]] ; then
ttool_list=$(cat ../Milos-v*/install/ads.define | cut -d":" -f1)
echo $ttool_list >> install_log.txt
for etool in $ttool_list
do
	if [[ "$etool" == "lsf" ]] ; then
	etool="bsub"
	fi
	tsearch=$(whence $etool)
	if [[ -z "$tsearch" ]] ; then
	badtool_list=$(echo $badtool_list $etool)
	print "3Party tools $etool is not in your environment search Path"
	echo "3Party tools $etool is not in your current Path" >> install_log.txt
	
	fi

done
echo "Third Party tools $badtool_list is not in your environment path" >> install_log.txt
echo "Completed Third Party tool check" >> install_log.txt
print "Tool will not be install during installation"

else
echo "Completed Third Party tool check - No ads.define found" >> install_log.txt

fi

# Installing Milos
#
echo "Starting Milos Installation" >> install_log.txt
print "Starting Milos Installation"
print "Please select Continue for all available Continue selections"
print "When ask for a LSF queue, please select queue designated for Milos operation"
$( cd ../$rtree/install ; ./configure.ksh )
echo "Completed Milos Installation" >> install_log.txt

# License deamon startup if the following items are in order
# Correctly setup license file in ../Milos-v*/conf/ads_license.dat
# Current Server = License server - define in ads_license.dat
echo "Starting License Server" >> install_log.txt
sname=$(hostname | cut -d"." -f1 )
shostid=$(../$rtree/license/bin/lmhostid | tail -1 | awk -F\" '{print $2}')
echo "Installation Server name = $sname, Installation Server hostid = $shostid" >> install_log.txt
if [[ -r ../$rtree/conf/ads_license.dat ]] ; then
	lname=$( grep SERVER ../$rtree/conf/ads_license.dat | awk '{print $2}')
	lhostid=$( grep SERVER ../$rtree/conf/ads_license.dat | awk '{print $3}')
	echo "License file host = $lname, License file hostid = $lhostid" >> install_log.txt
	if [[ "$sname" == "$lname" && "$shostid" == "$lhostid" ]] ; then
	license_start=$( ../$rtree/license/bin/lmgrd -c ../$rtree/conf/ads_license.dat -l ../$rtree/log & )
	echo "$license_start" >> install_log.txt
	else
	echo "license server / file information do not match" >> install_log.txt
	echo "License Daemon not started" >> install_log.txt
	echo "Athena License Daemon must be manually start"
	fi
else
	echo " ../$rtree/conf/ads_license.dat do not exist" >> install_log.txt
	echo "No license startup" >> install_log.txt
	print "Athena License Daemon must be manually started"
fi

$(cat ../$rtree/conf/msh.conf >> install_log.txt )
print "Completed Milos Installation"
print ""
print ""

Ctree=$(dirname $(pwd))
ADSPATH="$Ctree/$rtree/bin"

print ""
print "Manual Athena License daemon startup: "
print "   From License Server execute - "
print "    $ADSPATH/license/bin/lmgrd -c $ADSPATH/conf/ads_license.dat -l $ADSPATH/log/athena.log"
print ""

# Generate cshrc.ads, profile.ads
#

echo "Customer's ADS Path = $ADSPATH" >> install_log.txt

echo "#**********************************************************" > ../$rtree/bin/cshrc.ads
echo "# cshrc.ads:" >> ../$rtree/bin/cshrc.ads
echo "#    Sets the C shell user environment for Athena commands" >> ../$rtree/bin/cshrc.ads
echo "#**********************************************************" >> ../$rtree/bin/cshrc.ads
echo "#!/bin/csh" >> ../$rtree/bin/cshrc.ads
echo "setenv _ORIG_USER_PATH "'$'PATH"" >> ../$rtree/bin/cshrc.ads
echo "setenv PATH "$ADSPATH:'$'PATH"" >> ../$rtree/bin/cshrc.ads

echo "Completed cshrc.ads" >> install_log.txt
$(cat ../$rtree/bin/cshrc.ads >> install_log.txt)

echo "#**********************************************************" > ../$rtree/bin/profile.ads
echo "# profile.ads:" >> ../$rtree/bin/profile.ads
echo "#    Sets the C shell user environment for Athena commands" >> ../$rtree/bin/profile.ads
echo "#**********************************************************" >> ../$rtree/bin/profile.ads
echo "#!/bin/sh" >> ../$rtree/bin/profile.ads
echo "_ORIG_USER_PATH="'$'PATH"" >> ../$rtree/bin/profile.ads
echo "PATH="$ADSPATH:'$'PATH"" >> ../$rtree/bin/profile.ads

echo "Completed profile.ads" >> install_log.txt
$(cat ../$rtree/bin/profile.ads >> install_log.txt)

print "For Installation Trouble-shooting"
print "   1) tar and compress install_log.txt "
print "   2) mail tarimage to isupport@athenadesign.com "
print "     or  "
print "   3) mail to your designated Athena Representative "
print ""
print ""
print "To start Athena software" 
print "   1) setup Environment"
print "      csh, tcsh shell: source $ADSPATH/bin/cshrc.ads"
print "      sh, bash, ksh shell: source $ADSPATH/bin/profile.ads"
print "   2) cd "$"working directory"
print "   3) execute - ads milos or ads milos "$"working_tcl_file "
