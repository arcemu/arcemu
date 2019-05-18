Install Docker then enter this directory and type:
docker build -t jenkins_slave_arcemu .

While it's not meant to be run just like this, you can do so:
* docker run -d -p 2222:22 jenkins_slave_arcemu
* ssh -p 2222 jenkins@localhost

Username / Password: jenkins / jenkins
