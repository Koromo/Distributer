#!/bin/sh

fullpath()
{
    curdir=`pwd`
    cd $(dirname $1)
    fulldir=`pwd`
    base=`basename $1`
    cd $curdir
    echo "${fulldir}/${base}"
}

if [ $# -ne 3 ]; then
    echo "Usage: ${0} markers homeworks output"
    echo "markers: Markers path"
    echo "homeworks: Homeworks path."
    echo "output: Output path."
    exit 1
fi

cur_dir=`pwd`
tool_path=`dirname $0`
tool_path=`fullpath $tool_path`
markers_path=`fullpath $1`
homeworks_path=`fullpath $2`
out_path=`fullpath $3`
table_path="${out_path}/table"

if [ ! -d $out_path ]; then
    mkdir $out_path
    if [ $? -ne 0 ]; then
        exit -1
    fi
fi

"${tool_path}/tablegen" $markers_path $homeworks_path > $table_path
if [ $? -ne 0 ]; then
    exit -1
fi

cd $out_path
python "${tool_path}/collect.py" $homeworks_path $table_path
cd $cur_path

chmod -R a+rwx $out_path

exit 0
