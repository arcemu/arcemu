#!/bin/sh
branch=`git rev-parse --abbrev-ref HEAD`
version=`git log -1 --pretty=format:"%h"`
timestamp=`git log -1 --pretty=format:"%ct"`
commit_time=`git log -1 --pretty=format:"%ci"`
hostname=`hostname`
username=`whoami | cut -d\\\ -f2`

if [ -z "$version" ]
then
	version="0"
fi

if [ -z "$timestamp" ]
then
	timestamp="0"
fi

if [ -z "$commit_time" ]
then
	commit_time="unknown commit time"
fi

if [ -z "$branch" ]
then
	branch="unknown branch"
fi


echo "#ifndef GIT_VERSION_HPP" > src/arcemu-shared/git_version.h
echo "#define GIT_VERSION_HPP" >> src/arcemu-shared/git_version.h
echo "" >> src/arcemu-shared/git_version.h
echo "#define BUILD_BRANCH \"$branch\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_HASH 0x$version" >> src/arcemu-shared/git_version.h
echo "#define COMMIT_TIMESTAMP $timestamp" >> src/arcemu-shared/git_version.h
echo "#define COMMIT_TIME \"$commit_time\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_HASH_STR \"$version\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_USER_STR \"$username\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_HOST_STR \"$hostname\"" >> src/arcemu-shared/git_version.h
echo "" >> src/arcemu-shared/git_version.h
echo "#endif" >> src/arcemu-shared/git_version.h
