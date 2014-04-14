#!/bin/bash
#$2 VM_ip
#$1 venice_ip
scp -i ~/mykey.pem checkLink.sh ubuntu@$1:
output=`ssh -i ~/mykey.pem ubuntu@$1 "./checkLink.sh $2"`
echo $output
