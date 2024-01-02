# ![logo](https://raw.githubusercontent.com/arcemu/arcemu/master/arcemulogo.png) Arcemu

## Project Status
This project is mostly abandoned since the end of 2014. It is only developed and maintained ocassionally.

## Supported platforms

CPU Architecture  | Linux (GCC) | Windows (Visual Studio)
:-- | :-- | :---
amd64 | :heavy_check_mark: | :heavy_check_mark:
riscv64 | :heavy_check_mark: | :x:
aarch64 | :heavy_check_mark: | :heavy_check_mark:

Note: The project also has code for other platforms like Apple, and BSD, but they may not be up to date, and are not "officially" supported.

Note: The Linux build is officially tested with various versions (16.04 and later) of Ubuntu Server LTS.

Note: The Windows build is officially tested with Windows 7 (VS2010) and Windows 10 (VS2017).

## Build Status

Platform | Build status
:--- | :---
Dockerized Linux build with Github Actions | [![master_actions](https://github.com/arcemu/arcemu/actions/workflows/master_actions.yml/badge.svg)](https://github.com/arcemu/arcemu/actions/workflows/master_actions.yml)
Windows build on AppVeyor | [![Build status](https://ci.appveyor.com/api/projects/status/avvvuy4vpn183ncm?svg=true)](https://ci.appveyor.com/project/dfighter1985/arcemu-vulhc)


## Code Quality

Tool | Status
:--- | :---
CodeFactor static analyzer | [![CodeFactor](https://www.codefactor.io/repository/github/arcemu/arcemu/badge/master)](https://www.codefactor.io/repository/github/arcemu/arcemu/overview/master)

## Introduction

Arcemu is a World of Warcraft server package. Based on the popular Ascent project (defunct as of 2007) it aims to provide players with a gameplay very close or identical to the real thing as of World of Warcraft version 3.3.5a.

It's a multithreaded C++ application with a relational database as a data storage backend ( MySQL or MariaDB ).

It supports easy extension and customization with plugins, and the following scripting languages:

* Lua
* Python (experimental)

## Installation

For installation instructions see the wiki.

## Development box

You can get yourself a quick and dirty installation by using Vagrant. It will set up a virtual machine for you, with Arcemu installed.

* Install Vagrant and VirtualBox
* Create a directory somewhere
* Check out Arcemu code to that directory
* Go to the created directory in the command line / shell
* Issue the command 'vagrant up', and wait until the process finishes.

Vagrant will download an OS for the virtual machine and then build and install Arcemu.
After it is done, you can log in to the box, using the following command:

```
vagrant ssh
```

## Links

* [Docker hub](https://hub.docker.com/u/arcemu)
* [Community wiki](https://arcemu.fandom.com/wiki/Arcemu_Wiki)

