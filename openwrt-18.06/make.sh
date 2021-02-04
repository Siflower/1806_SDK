#!/bin/bash
#$1 should be p10 or p20

board=ac28
imgtype=rel

git branch > /dev/null 2>&1
if [ ! $? -eq 0 ] ; then
	echo "dose not exist git base"
	tag="0.0"
	branch="default"
else
	# branch=`git branch  -r | grep "\->" |awk -F "/" 'END{print $NF}'`
	local_branch_name=`git branch -vv |grep "*" |awk '{print $2}'`
	# echo "local branch name $local_branch_name"
	if [ "$local_branch_name" = "(no" ]; then
		echo "branch set fail no local branch"
		exit 1
	fi

	branch=`git branch -vv |grep "*" | awk -F "[][]" '{print $2}'| awk -F "[/:]" '{print $2}'`
	echo "branch is $branch"

	tag=`git tag  |  grep "${branch}-" | sort -V | awk 'END{print}'`
	if [ ! -n "$tag" ] ;then
		#compatible with old version
		tag=`git tag  |  grep -v "v"  | sort -V | awk 'END{print}'`
		version=$tag
	else
		# version=`printf "$tag" | awk -F "[-]" '{print $2}'`
		 version=`echo ${tag##*-}`
	fi
	echo "version is $version"
	tag_commit=`git show $tag|grep ^commit | awk '{print substr($2,0,7)}'`
	echo "tag commit $tag_commit"
	last_commit=`git rev-parse --short HEAD`
	echo "last commit $last_commit"

	if [ $tag_commit != $last_commit ]; then
		commit_suffix=$last_commit
	fi
fi


#$1 p10 p10m p20 86v clean default p10m
if [ -n "$1" ]; then
	board=$1
fi

#$2 shoule be auto/flash/rel  default rel
if [ -n "$2" ]; then
	imgtype=$2
fi

case ${board} in
	evb_v5)
		target_board=target/linux/siflower/sf16a18_evb_v5_fullmask_def.config
		;;
	a28_evb)
		target_board=target/linux/siflower/sf19a28_evb_fullmask_def.config
		;;
	a28_ac28)
		target_board=target/linux/siflower/sf19a28_ac28_fullmask_def.config
		;;
	clean)
		echo "clean build enviroment"
		echo "delete build dir "
		rm -rf build_dir
		echo "delete tmp"
		rm -rf tmp
		echo "delete board"
		rm .board
		exit 1
		;;
	*)
		echo "error arg"
		exit 1 # Command to come out of the program with status 1
		;;
esac

config_profile=`grep "^CONFIG_TARGET_siflower_" ${target_board} | head -1 | awk -F "[_=]" '{print $4" "$5}'`
read chip chip_ver <<< "${config_profile}"

echo "build board is $board chip ${chip}_${chip_ver} imgtype $imgtype"

if [ -n "$commit_suffix" ]; then
	prefix=openwrt1806_${branch}_${board}_${chip}_${chip_ver}_${imgtype}_${version}_${commit_suffix}
else
	prefix=openwrt1806_${branch}_${board}_${chip}_${chip_ver}_${imgtype}_${version}
fi

target_bin="bin/targets/siflower/openwrt-siflower-${chip}-${chip_ver}-squashfs-sysupgrade.bin"

echo "prefix is ${prefix}"
#set up version.mk
rm include/siwifi_version.mk
echo "VERSION_DIST:=SiWiFi" >> include/siwifi_version.mk
echo 'VERSION_NICK:=$(PROFILE)' >> include/siwifi_version.mk
echo "VERSION_NUMBER:=${prefix}" >> include/siwifi_version.mk

sed -e '12cVERSION_NUMBER:='${prefix}'' include/siwifi_version.mk > tmp_version

sed -e '14cVERSION_NUMBER:='${prefix}'' tmp_version > include/siwifi_version.mk

rm tmp_version

echo "set openwrt version"


echo "set up board $target_config"

if [ "$imgtype" = "flash" ]; then
	target_board=target/linux/siflower/${chip}_p10_${chip_ver}_flash.config
fi

if [ -f .board ] && [ "$imagetype" != "auto" ]; then
	cmp_reselt=`cmp $target_board .config`
	if [ -n "$cmp_reselt" ]; then
		echo "board change, clean build enviroment"
		rm -rf build_dir
		rm -rf tmp
		cp $target_board .config
	fi
else
	cp $target_board .config
fi

rm $target_bin

case ${imgtype} in
	auto)
		echo "CONFIG_PACKAGE_autotest=y" >> .config
		echo "build auto"
		;;

	flash)
		echo "build flash"
		;;
	rel)
		echo "build release"
		;;
	*)
		echo "imgtype error arg"
		exit 1 # Command to come out of the program with status 1
		;;
esac

make package/base-files/clean
make -j32 V=s

if [ -f $target_bin ]; then
	cp $target_bin ${prefix}.bin
else
	echo "build fail, don't get target_bin"
fi

echo "build end"
