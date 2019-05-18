FROM ubuntu:xenial

RUN apt-get update
RUN apt-get -y dist-upgrade
RUN apt-get install -y openssh-server default-jre-headless git cmake make g++
RUN apt-get install -y zlib1g-dev libssl-dev libpcre3-dev libbz2-dev libmysqlclient-dev libmysql++-dev

RUN sed -i 's|session    required     pam_loginuid.so|session    optional     pam_loginuid.so|g' /etc/pam.d/sshd
RUN mkdir -p /var/run/sshd

RUN useradd -m -s /bin/bash jenkins
RUN echo "jenkins:jenkins" | chpasswd

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
