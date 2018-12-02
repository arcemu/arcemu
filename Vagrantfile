# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/xenial64"

  # Don't check for Vagrant base box updates automatically
  config.vm.box_check_update = false

  # Forward ports
  config.vm.network "forwarded_port", guest: 22, host: 2222
  config.vm.network "forwarded_port", guest: 3724, host: 3724
  config.vm.network "forwarded_port", guest: 8129, host: 8129

  config.vm.provider "virtualbox" do |vb|
    vb.gui = true
	vb.cpus = 1
    vb.memory = "1024"
  end
  
  # Provision the virtual machine with shell commands
  config.vm.provision "shell", inline: <<-SHELL
	 # Enable password based authentication for SSH
	 sed -i 's/ChallengeResponseAuthentication no/ChallengeResponseAuthentication yes/g' /etc/ssh/sshd_config
	 service ssh restart
	 
	 # Create an arcemu user with password arcemu
	 useradd -m -s /bin/bash arcemu
	 echo "arcemu:arcemu" | chpasswd
	 
	 # Set up mysql server root password before installing
	 debconf-set-selections <<< 'mysql-server mysql-server/root_password password root'
	 debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password root'
	 
	 # Install dependencies
	 apt-get update
	 apt-get install -y git cmake make g++ mysql-server mysql-client
	 apt-get install -y zlib1g-dev libssl-dev libpcre3-dev libbz2-dev libmysqlclient-dev libmysql++-dev
	 
	 # Switch to the Arcemu user
	 su - arcemu
	 
	 # Set up database user and databases
	 echo "CREATE USER arcemu@localhost IDENTIFIED BY 'arcemu';" > db.sql
	 echo "CREATE DATABASE arcemu_world;" >> db.sql
	 echo "CREATE DATABASE arcemu_character;" >> db.sql
	 echo "CREATE DATABASE arcemu_logon;" >> db.sql
	 echo "GRANT ALL PRIVILEGES ON arcemu_world.* TO arcemu@localhost;" >> db.sql
	 echo "GRANT ALL PRIVILEGES ON arcemu_character.* TO arcemu@localhost;" >> db.sql
	 echo "GRANT ALL PRIVILEGES ON arcemu_logon.* TO arcemu@localhost;" >> db.sql
	 cat db.sql | mysql -u root --password=root
	 rm db.sql
	 
	 # Clone source from github. NOTE: This can take a long time.
	 mkdir arcemu
	 cd arcemu
	 git clone https://github.com/arcemu/arcemu.git src 2>&1
	 
	 # Load database schemas
	 cat src/sql/logon_structure.sql | mysql -u arcemu --password=arcemu arcemu_logon
	 cat src/sql/world_structure.sql | mysql -u arcemu --password=arcemu arcemu_world
	 cat src/sql/character_structure.sql | mysql -u arcemu --password=arcemu arcemu_character
	 
	 # Configure build
	 mkdir bin
	 mkdir build
	 cd build
	 cmake -DCMAKE_INSTALL_PREFIX=/home/arcemu/arcemu/bin ../src/cmake 2>&1
	
	 # Build! NOTE: This can take a very long time.
	 make
	 
	 # Install the binaries
	 make install
	 
	 # Copy config files
	 cp /home/arcemu/arcemu/src/configs/* /home/arcemu/arcemu/bin/etc

     
  SHELL
end
