#!/bin/sh
version=`git log -1 --pretty=format:"%h"`
timestamp=`git log -1 --pretty=format:"%ct"`
tag=`cat arcemu-tag`
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


echo "#ifndef GIT_VERSION_HPP" > src/arcemu-shared/git_version.h
echo "#define GIT_VERSION_HPP" >> src/arcemu-shared/git_version.h
echo "" >> src/arcemu-shared/git_version.h
echo "#define BUILD_TAG \"$tag\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_HASH 0x$version" >> src/arcemu-shared/git_version.h
echo "#define COMMIT_TIMESTAMP $timestamp" >> src/arcemu-shared/git_version.h
echo "#define BUILD_HASH_STR \"$version\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_USER_STR \"$username\"" >> src/arcemu-shared/git_version.h
echo "#define BUILD_HOST_STR \"$hostname\"" >> src/arcemu-shared/git_version.h
echo "" >> src/arcemu-shared/git_version.h
echo "#endif" >> src/arcemu-shared/git_version.h
