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

mkdir $out_path
if [ $? -ne 0 ]; then
    exit 1
fi

"${tool_path}/distgen" $markers_path $homeworks_path $table_path
if [ $? -ne 0 ]; then
    echo 10
    exit 1
fi

cd $out_path
python "${tool_path}/rezip.py" $homeworks_path $table_path
cd $cur_path

exit 0
