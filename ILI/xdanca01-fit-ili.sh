#!/bin/bash

#Task 1 - 4 loop devices
echo "Task 1 - Creating 4 loop devices"

echo "Creating dir for 4 files in tmp dir"
tmpdir=$(mktemp -d)

echo "Checking package coreutils - dd"

rpm -q coreutils > /dev/null
ret=$(echo $?)
if [ $ret -eq 1 ] ; then
    echo "Installing coreutils"
    yum install coreutils -y > /dev/null
fi

echo "Checking package util-linux - losetup"

rpm -q util-linux > /dev/null
ret=$(echo $?)
if [ $ret -eq 1 ] ; then
    echo "Installing util-linux"
    yum install util-linux -y > /dev/null
fi

for i in `seq 1 4`; do
    dd if=/dev/zero of=$tmpdir/file$i bs=200M count=1 > /dev/null 2>&1
    echo "Created file $tmpdir/file$i"
    losetup loop$i $tmpdir/file$i > /dev/null 2>&1
    echo "Created loop device: loop$i"
done



#TASK 2 - 2 RAIDS FROM LOOP DEVICES
echo "Task 2 - Creating RAID1 and RAID0"

echo "Checking package mdadm - mdadm"

rpm -q mdadm > /dev/null
ret=$(echo $?)
if [ $ret -eq 1 ] ; then
    echo "Installing mdadm"
    yum install mdadm -y > /dev/null
fi

echo "Creating RAID1"
echo "y" | mdadm --create /dev/md0 --level=mirror --raid-devices=2 /dev/loop1 /dev/loop2 > /dev/null

echo "Creating RAID0"
echo "y" | mdadm --create /dev/md1 --level=mirror --raid-devices=2 /dev/loop3 /dev/loop4 > /dev/null


#TASK 3 - volume group on top of 2 RAID devices
echo "Task 3 - Creating group on top of 2 RAID devices"

echo "Checking package lvm2 - vgcreate"
rpm -q lvm2 > /dev/null
ret=$(echo $?)
if [ $ret -eq 1 ] ; then
    echo "Installing lvm2"
    yum install lvm2 -y > /dev/null
fi

echo "Creating volume group FIT_vg"
vgcreate FIT_vg /dev/md0 /dev/md1 > /dev/null


#TASK 4 - 2 logical volumes from FIT_vg
echo "Task 4 - Creating 2 logical volumes based on FIT_vg"

echo "Creating logical volume FIT_lv1"
lvcreate FIT_vg -n FIT_lv1 -L100M

echo "Creating logical volume FIT_lv2"
lvcreate FIT_vg -n FIT_lv2 -L100M

#TASK 5 - set fs on FIT_lv1 to ext4
echo "Task 5 - Set fs on FIT_lv1 to ext4"

echo "Setting ext4 fs on /dev/FIT_vg/FIT_lv1"
mkfs -t ext4 /dev/FIT_vg/FIT_lv1

#TASK 6 - set fs on FIT_lv2 to xfs
echo "Task 6 - Set fs on FIT_lv2 to xfs"

echo "Setting xfs fs on /dev/FIT_vg/FIT_lv2"
mkfs -t xfs /dev/FIT_vg/FIT_lv2

#TASK 7 - mount FIT_lv1
echo "Task 7 - mount FIT_lv1 and FIT_lv2"

echo "Creating directory for mount: /mnt/test1"
mkdir /mnt/test1

echo "Creating directory for mount: /mnt/test2"
mkdir /mnt/test2

echo "Mounting FIT_lv1 to /mnt/test1"
mount /dev/FIT_vg/FIT_lv1 /mnt/test1

echo "Mounting FIT_lv2 to /mnt/test2"
mount /dev/FIT_vg/FIT_lv2 /mnt/test2

#TASK 8 - Resize FIT_lv1 to max space from volume group
echo "Task 8 - Resize FIT_lv1 to max capacity from volume group"

echo "Extending logical volume size to max capacity"
lvextend -l +100%FREE /dev/FIT_vg/FIT_lv1

echo "Checking package e2fsprogs - resize2fs"
rpm -q e2fsprogs > /dev/null
ret=$(echo $?)
if [ $ret -eq 1 ] ; then
    echo "Installing e2fsprogs"
    yum install e2fsprogs -y > /dev/null
fi

echo "Resizing fs to new size"
resize2fs /dev/FIT_vg/FIT_lv1

#TASK 9 - Create 300MB file by command dd
echo "Task 9 - Create 3OOMB file by command dd"

echo "Creating file /mnt/test1/big_file"
dd if=/dev/urandom of=/mnt/test1/big_file bs=300M count=1

echo "Creating checksum of file /mnt/test1/big_file"
sha512sum /mnt/test1/big_file

#TASK 10 - Emulate faulty disk
echo "Task 10 - Emulate faulty disk"

echo "Creating file for new loop device"
dd if=/dev/zero of=$tmpdir/file5 bs=200M count=1 > /dev/null 2>&1
losetup loop5 $tmpdir/file5 > /dev/null 2>&1
echo "Created loop device: loop5"

echo "Adding loop5 to /dev/md0 - RAID1"
mdadm --manage /dev/md0 --add /dev/loop5

echo "Replacing loop1 with loop5"
mdadm --manage /dev/md0 --replace /dev/loop1 --with /dev/loop5

echo "Verifing recovery from faulty disk"
mdadm --detail /dev/md0 | grep "loop5" | grep "active"
ret=$(echo $?)
if [ $ret -eq 0 ] ; then
    echo "Recovery wasnt successfull"
else
    echo "Recovery was successfull"
fi
