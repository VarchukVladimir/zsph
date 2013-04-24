#!/bin/bash
source run.env

rm -f search.nexe
rm -f search.manifest

#rm indexer.nexe
#rm search.nexe

#cp ../src/xmlpipecreator 	xmlpipecreator.nexe
#cp ../src/indexer 		indexer.nexe
cp ../src/search 		search.nexe

#rm /data/indexer/stdout.data
#rm /data/xmlpipecreator/stdout.data
#rm /data/search/stdout.data

./genmanifest.sh

#./ns_start.sh 2

#${SETARCH} ${ZEROVM_ROOT}/zerovm -Mindexer.manifest &
${ZVM_PREFIX}/zerovm -Msearch.manifest 

#cat data/indexer/stdout.data

#${SETARCH} ${ZEROVM_ROOT}/zerovm -Msearch.manifest 

#cat stdch/search/stdout.data

#./ns_stop.sh

