#!/bin/bash

IPCS_M=`ipcs -m | egrep "0x[0-9a-f]+ [0-9]+" | cut -f2 -d" "`

for id in $IPCS_M; do
  ipcrm -m $id;
done
