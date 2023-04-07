IFS=":"

function find_lib() {
grep -E "\#include <" | sed -En 's/.*#include <(.*)>/\1/p'
}

while read line
do
res="---"
for path in $MANPATH
do
path_to_man="$path/man3/$line.3"

if [ -f "$path_to_man" ]; then
res=`cat $path_to_man | find_lib`
break
fi;

if [ -f "$path_to_man.gz" ]; then
res=`gunzip -c -d "$path_to_man.gz" | find_lib`
break
fi;
done;

echo $res

done;