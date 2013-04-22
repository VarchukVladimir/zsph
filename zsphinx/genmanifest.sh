#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPT_PATH=`dirname "$SCRIPT"`

#Generate from template

sed s@{ABS_PATH}@$SCRIPT_PATH/@g manifest/indexer.manifest.template        	> indexer.manifest
sed s@{ABS_PATH}@$SCRIPT_PATH/@g manifest/xmlpipecreator.manifest.template  	> xmlpipecreator.manifest
sed s@{ABS_PATH}@$SCRIPT_PATH/@g manifest/search.manifest.template  		> search.manifest



