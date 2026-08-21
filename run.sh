if [ -z "$1" ] || [ "$1" -ne 0 ]; then
    [ -e ./a.out ] && rm ./a.out
    [ -e ./main.txt ] && rm ./main.txt
    if [ "$2" = "1" ]; then
        g++ -O0 -std=c++20 -g ./main.cpp
    else
        g++ -O3 -std=c++20 -fpermissive -fassociative-math -ffast-math -ffloat-store ./main.cpp
    fi
fi
if [ -e ./a.out ]; then
    [ -e ./output.txt ] && rm ./output.txt
    cp ./main.cpp ./main.txt
    cat ./input.txt | ./a.out | tee ./output
    [ -e ./output ] && mv ./output ./output.txt
    if [ "$2" = "1" ]; then
        gdb -q -batch -ex "set debuginfod enabled off" -ex "set confirm off" -ex "run < ./input.txt" -ex "bt full" ./a.out
    fi
fi