#!/bin/bash

if [ -z "$1" ]; then
    echo "Missing directory"
    exit 1
fi

pushd $1

for i in *.t3s; do 
    echo "#include \""`echo $i | sed 's/.t3s/_t3x.h/'`\"
done

echo ""
echo "struct Page"
echo "{"
echo -e "\tconst u8* t3x;"
echo -e "\tsize_t size;"
echo "};"
echo ""
echo "struct Page Pages[] ="
echo "{"

for i in *.t3s; do
    BASE=`echo $i | sed 's/.t3s//'`
    echo -e "\t{${BASE}_t3x, ${BASE}_t3x_size},"
done

echo "};"