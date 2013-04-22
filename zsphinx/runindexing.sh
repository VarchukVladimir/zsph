#!/bin/bash
source run.env

rm -f *.nexe
rm -f *.manifest

#rm indexer.nexe
#rm search.nexe

cp ../src/xmlpipecreator 	xmlpipecreator.nexe
cp ../src/indexer 		indexer.nexe
cp ../src/search 		search.nexe

#rm /data/indexer/stdout.data
#rm /data/xmlpipecreator/stdout.data
#rm /data/search/stdout.data

./genmanifest.sh

./ns_start.sh 2

${SETARCH} ${ZEROVM_ROOT}/zerovm -Mindexer.manifest &
${SETARCH} ${ZEROVM_ROOT}/zerovm -Mxmlpipecreator.manifest 

sleep 1

cat data/indexer/stdout.data

./ns_stop.sh


${SETARCH} ${ZEROVM_ROOT}/zerovm -Msearch.manifest 

cat data/search/stdout.data



