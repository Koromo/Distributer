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

if [ $# -ne 2 ]; then
    echo "Usage: ${0} homeworks output"
    echo "homeworks: Homeworks pass."
    echo "output: Output pass."
    exit 1
fi

cur_dir=`pwd`
tool_path=`dirname $0`
tool_path=`fullpath $tool_path`
homeworks_path=`fullpath $1`
out_path=`fullpath $2`
table_path="${out_path}/table"

mkdir $out_path
if [ $? -ne 0 ]; then
    exit 1
fi

"${tool_path}/distgen" $homeworks_path $table_path
if [ $? -ne 0 ]; then
    echo 10
    exit 1
fi

cd $out_path
python "${tool_path}/rezip.py" $homeworks_path $table_path
cd $cur_path

exit 0
