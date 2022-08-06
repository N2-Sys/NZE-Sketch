#!/bin/bash

if [ ! -e build ]; then
    if [ `mkdir build &>/dev/null` ]; then
        echo \e[31mCan not make directory build\e[0m
        exit 1
    fi
fi

if [ ! -e bin ]; then
    if [ `mkdir bin &>/dev/null` ]; then
        echo \e[31mCan not make directory bin
        exit 1
    fi
fi

cd build

cmake .. -DEXECUTABLE_OUTPUT_PATH=../bin && make

# if [ $? == 0 ]; then
#     if [ `mv seq ../bin/ &>/dev/null` ]; then
#         echo \e[31mCan not move SeqSketch into the bin directory\e[0m
#         exit 2
#     fi
#     # if [ `mv build ../bin/ &>/dev/null` ]; then
#     #     echo \e[31mCan not move EmbedSketch into the bin directory\e[0m
#     #     exit 2
#     # fi
# else
#     echo \e[31mBuild Failed\e[0m
#     exit 3
# fi

exit 0

