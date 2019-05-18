Contents of this directory:
---------------------------
jenkins_master - Jenkins Vagrant box
jenkins_slave_arcemu - Jenkins slave Docker image to build Arcemu

What does it all do?
--------------------
Jenkins is a CI/CD solution that can automatically build, test, and deploy software.
This directory contains a Vagrantfile for a Jenkins box (with Docker slave support), and a Docker slave for actually building, testing, deploying.

Setup:
------
* Set up the box using the Vagrantfile in the directory jenkins_master
* Manage Jenkins - Manage Plugins - Available - Docker module
* Manage Jenkins - Configure System
* Add Docker cloud
	Docker Host URI: tcp://127.0.0.1:8888
* Add Docker template:
	Label, name, image: jenkins_slave_arcemu
	Remote file system root: /home/jenkins 
	Connect method: Connect with SSH
	SSH key: Use configured SSH credentials.
	SSH credentials: ( user: jenkins, password: jenkins )
	Host key verification strategy: Non verifying strategy
	Pull strategy: Never pull
* Save

* Build the Docker image in jenkins_slave_arcemu directory

* Add Arcemu to Jenkins: (New item)
	name: Arcemu
	Multibranch Pipeline
	Add source - Git: 
* Enjoy!
