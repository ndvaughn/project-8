#!/bin/bash
export Your_Name="Nolan Vaughn"
export ASU_ID="1229650437"

message="[CSE330][Fall2023][Project-3][ASUID:$ASU_ID][$Your_Name]"

make clean
make

echo ""
echo "---------------------- Compilation Completed -----------------------------"
echo "---------------------- Start Testing -----------------------------"
echo ""

echo "Test script will insert the test kernel module"
insmod proc_filesys.ko


if [[ $1 -eq 1 ]]; then
    echo "write $message to /proc/my_proc_file"
    echo $message > /proc/my_proc_file

    echo ""
    echo "dmseg output:"
    dmesg | tail -n 5


    echo ""
    echo "---------------------- Test Case: 1 -----------------------------"
    echo "read entire content from /proc/my_proc_file:"
    head -c ${#message} /proc/my_proc_file

    echo ""
    echo "dmseg output:"
    dmesg | tail -n 5


    echo ""
    echo "---------------------- Test Case: 2 -----------------------------"
    echo "read from head:"
    head -c 29 /proc/my_proc_file

    echo ""
    echo "dmseg output:"
    dmesg | tail -n 5

    echo ""
    echo "---------------------- Test Case: 3 -----------------------------"
    echo "read from middle:"
    # head -c 29 /proc/my_proc_file
    dd if=/proc/my_proc_file bs=1 skip=29 count=18 2> /dev/null

    echo ""
    echo "dmseg output:"
    dmesg | tail -n 5


elif [[ $1 -eq 2 ]]; then
    echo "---------------------- Test Case: 4 -----------------------------"
    additional_content="This is additional content.xxxxxxxxxxxxxxxxxxxxx"
    message="$message $additional_content"
    echo "write beyond the limit to /proc/my_proc_file, which should return -EINVAL (Invalid argument)"
    echo -n $message > /proc/my_proc_file


elif [[ $1 -eq 3 ]]; then

    echo ""
    echo "---------------------- Test Case: 5 -----------------------------"
    
    echo $message > /proc/my_proc_file
    echo "read from tail:"
    tail -c $(( ${#Your_Name}+3 )) /proc/my_proc_file

    echo ""
    echo "dmseg output:"
    dmesg | tail -n 5

fi

echo ""
echo ""
echo "Test script will remove the test kernel module"
rmmod proc_filesys.ko
echo "---------------------- make clean -----------------------------"
make clean
