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
	 mkdir -p /home/arcemu/arcemu
	 git clone https://github.com/arcemu/arcemu.git /home/arcemu/arcemu/src 2>&1
	 
	 # Load database schemas
	 cat /home/arcemu/arcemu/src/sql/logon_structure.sql | mysql -u arcemu --password=arcemu arcemu_logon
	 cat /home/arcemu/arcemu/src/sql/world_structure.sql | mysql -u arcemu --password=arcemu arcemu_world
	 cat /home/arcemu/arcemu/src/sql/character_structure.sql | mysql -u arcemu --password=arcemu arcemu_character
	 
	 # Create an admin account, admin:admin
	 echo "INSERT INTO accounts VALUES (1,'admin','','8301316d0d8448a34fa6d0c6bf1cbfa2b4a1a93a','az',0,NOW(),'','a
dmin@admin',24,'enUS',0,'');" | mysql -u arcemu --password=arcemu arcemu_logon
	 
	 # Configure build
	 mkdir /home/arcemu/arcemu/bin
	 mkdir /home/arcemu/arcemu/build
	 cd /home/arcemu/arcemu/build
	 cmake -DCMAKE_INSTALL_PREFIX=/home/arcemu/arcemu/bin ../src/cmake 2>&1
	
	 # Build! NOTE: This can take a very long time.
	 make
	 
	 # Install the binaries
	 make install
	 
	 # Create some directories
	 mkdir -p /home/arcemu/arcemu/bin/DBC
	 mkdir -p /home/arcemu/arcemu/bin/maps
	 mkdir -p /home/arcemu/arcemu/bin/etc
	 mkdir -p /home/arcemu/arcemu/bin/log

	 # Copy config files
	 cp /home/arcemu/arcemu/src/configs/* /home/arcemu/arcemu/bin/etc
	 
	 # Make everything owned by the arcemu user
	 chown -R arcemu:arcemu /home/arcemu
     
  SHELL
end
