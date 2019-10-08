#!/bin/ksh -p

unset ENV

function die {
        echo "gen_support.ksh: $*" 1>&2
        exit 1
}

TOP=$(dirname $(dirname $0))
JOB_NAME=$(pwd|sed 's/\//_/g;s/^_//')
LOG_DIR="/tmp/$JOB_NAME"
WORK_DIR=$(pwd)
LOG_FILE=$WORK_DIR/logs
PROCINFO="version swaps meminfo cpuinfo"
LUSER=$(id | awk '{print $1}'| cut -d"(" -f2 | cut -d")" -f1)
USER=$(finger $LUSER | head -1)
SUMMARY="summary"
FDATE=$(date '+%d%m%y%H%M')

#if [[ -e $TOP/conf/msh.conf ]]
if [[ ! -e $TOP/conf/msh.conf ]]
then
die "echo $TOP do not appear to be the top of the Milos release tree"
fi

#if [[ -d $LOG_FILE/msh ]]
if [[ ! -d $LOG_FILE/msh ]]
then
die "$WORK_DIR: Do not appear to be the working directory!"
fi

if [[ ! -e $LOG_DIR ]]
then
mkdir $LOG_DIR
tar cvf - $LOG_FILE > $LOG_DIR/log_tar
else
die "$LOG_DIR exist! Please remove before running."
fi
DATE=$(date)
ENV=$(/usr/bin/printenv)

if [[ -e /etc/redhat-release ]]
then
cd /proc; tar cvf - $PROCINFO > $LOG_DIR/proc_tar
else
HOSTNAME=$(hostname)
die "echo $HOSTNAME is not running a supported operating system"
fi
echo "Verify QX Setup" > $LOG_DIR/Config.log
$TOP/install/verify_qx.ksh >> $LOG_DIR/Config.log
echo "Verify LSF Setup" >> $LOG_DIR/Config.log
$TOP/install/verify_lsf.ksh >> $LOG_DIR/Config.log
echo "Verify Milos License Setup" >> $LOG_DIR/Config.log
$TOP/install/check_milos_lic.ksh >> $LOG_DIR/Config.log
echo "Copy of Active msh.conf" >> $LOG_DIR/Config.log
cat $TOP/conf/msh.conf >> $LOG_DIR/Config.log
echo "User ENV Outside of ADS environment" >> $LOG_DIR/Config.log
/usr/bin/printenv >> $LOG_DIR/Config.log
echo "User ENV Inside ADS environment" >> $LOG_DIR/Config.log
$TOP/bin/ads env  >> $LOG_DIR/Config.log

printf "\n\n\n\n\n\n"
print "Please summarize your problem with Milos."
print "Enter/Return to finish!"
read problem
printf "\n\n\n\n\n\n"

print "Please enter your contact phone number including area code!"
print "Enter/Return to complete!"
read phone
printf "\n\n\n\n\n\n"

print "Please enter your contact Email address! Enter/Return to complete!"
read email
echo "DATE of Support Request		$DATE" > $LOG_DIR/$SUMMARY
echo "Support Case Requestor		$USER" >> $LOG_DIR/$SUMMARY
echo "Email Address of Requestor	$email" >> $LOG_DIR/$SUMMARY
echo "Phone Number of Requestor		$phone" >> $LOG_DIR/$SUMMARY
echo "Working Directory of Request	$WORK_DIR" >> $LOG_DIR/$SUMMARY
echo "Summary of Problem">> $LOG_DIR/$SUMMARY
echo $problem >> $LOG_DIR/$SUMMARY

cd /tmp
tar cvf - $JOB_NAME > "Milos_"$FDATE"_tar"
bzip2 "Milos_"$FDATE"_tar"
rm -rf $JOB_NAME
tarimage="/tmp/Milos_"$FDATE"_tar.bz2"
clear
print "Please ftp $tarimage to vip@athenads.com"
print "login account anonymous"
print "password = Your full qualify Email address"
print "cd write_only/support"
print "binary"
print "send $tarimage"
print "bye - to exit"

