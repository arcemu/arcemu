pipeline {
	agent {
		label 'jenkins_slave_arcemu'
	}
	
	options {
		timestamps()
	}
	
	triggers {
		pollSCM('H/5 * * * *')
	}
	
	stages {
		stage('info') {
			steps {
				sh 'uname -a'
				sh 'cmake --version'
				sh 'make --version'
				sh 'g++ --version'
			}
		}
		
		stage('build') {
			steps {
				sh 'mkdir build'
				dir("build") {
					sh 'cmake -DCMAKE_INSTALL_PREFIX=/home/jenkins/arcemu/bin ../cmake 2>&1'
					sh 'make'
				}
			}
		}
		
		stage('install') {
			steps {
				dir("build") {
					sh 'make install'
				}
			}
		}
	}		
}
