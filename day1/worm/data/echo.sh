#!/bin/bash
# Move the seed from the back to position 5 in the list
echo $1 $2 $3 $4 "${@: -1}" "${@: 5 : $#-5}"
