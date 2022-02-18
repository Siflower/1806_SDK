#!/bin/sh
#$1 should be p10 or p20

config=sfa28_evb
ver=fullmask
flash="0"
pcba="0"

#$1 should be p10 or p20 p10m 86v
if [ -n "$1" ]; then
	config=$1
fi

if [ -n "$2" ]; then
	ver=$2
fi

if [ -n "$3" ]; then
	flash=$3
fi

if [ -n "$4" ]; then
	pcba=$4
fi

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

	# handle release branch case release branch name is not release!!!
	is_86v_release=$(echo $branch | grep "release.86v")
	is_rep_release=$(echo $branch | grep "release.rep")
	isrelease=$(echo $branch | grep "release")
	if [ "$is_86v_release" != "" ];then
		branch="release.86v"
	elif [ "$is_rep_release" != "" ];then
		branch="release.rep"
	elif [ "$isrelease" != "" ];then
		branch="release"
	fi
# need to use tag for rep
	is_rep_board=$(echo $config | grep "rep")

	if [ "$is_rep_board" != "" ];then
		tag=`git tag  |  grep "${branch}.rep-" | sort -V | awk 'END{print}'`
	else
		tag=`git tag  |  grep "${branch}-" | sort -V | awk 'END{print}'`
	fi


	if [ ! -n "$tag" ] ;then
		#compatible with old version
		tag=`git tag  |  grep -v "v"  | sort -V | awk 'END{print}'`
		version=$tag
	else
		version=`printf "$tag" | awk -F "[-]" '{print $2}'`
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

echo "build config is $config"

if [ "$pcba" = "1" ]; then
	target_bin="sfax8/u-boot.img"
else
	target_bin="sfax8/uboot_full.img"
fi

if [ "$pcba" = "1" ]; then
	uboot_or_pcba=pcba
else
	uboot_or_pcba=uboot
fi

if [ -n "$commit_suffix" ]; then
	prefix=${uboot_or_pcba}_${branch}_${config}_${ver}_${version}_${commit_suffix}
else
	prefix=${uboot_or_pcba}_${branch}_${config}_${ver}_${version}
fi

echo "branch=$branch  config=$branch  ver=$ver  version=$version"

rm $target_bin -rf

./sf_make.sh cmd=clean prj=$config ver=$ver

./sf_make.sh prj=$config ver=$ver flash=$flash pcba=$pcba

if [ -f $target_bin ]; then
	cp $target_bin ${prefix}.bin
else
	echo "build fail, don't get target_bin"
fi

echo "build end"
