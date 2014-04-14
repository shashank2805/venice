#!/bin/sh
chmod +x limit_from_172.16.100.9.sh 
ssh-keygen -R 172.16.100.9
scp -o 'StrictHostKeyChecking no' -i ~/mykey.pem limit_from_172.16.100.9.sh ubuntu@172.16.100.9:~ 
ssh -o 'StrictHostKeyChecking no' -i ~/mykey.pem ubuntu@172.16.100.9 "./limit_from_172.16.100.9.sh" 
