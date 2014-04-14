#!/bin/sh
scp -i mykey.pem provisionLinks.sh ubuntu@$1
ssh -i mykey.pem ubuntu@$1 './provisionLinks.sh'
