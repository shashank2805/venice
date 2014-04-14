#!/bin/bash
output=`ssh-keygen -R $1; ssh -o 'StrictHostKeyChecking no' -i ~/mykey.pem ubuntu@$1 "tc class show dev eth0" 2>&1`
accessible=`echo $output|grep "port 22: No route to host"`
link_installed=`echo $output| grep "class htb 1:1 root rate"`
#echo $accessible
#echo $link_installed
if [ -n "$accessible" ] 
then 
	echo "No Route"
elif [ -n "$link_installed" ]
then 
	echo "Provisioned"
else
	echo "Provisioning"
fi
