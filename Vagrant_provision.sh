# Set up mysql server root password before installing
debconf-set-selections <<< 'mysql-server mysql-server/root_password password root'
debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password root'

# Install dependencies
apt-get update
apt-get install -y git cmake make g++ mysql-server mysql-client
apt-get install -y zlib1g-dev libssl-dev libpcre3-dev libbz2-dev libmysqlclient-dev libmysql++-dev libcurl4-openssl-dev

# Enable PTrace for debugging
echo 0 > /proc/sys/kernel/yama/ptrace_scope
echo "kernel.yama.ptrace_scope = 0" > /etc/sysctl.d/10-ptrace.conf

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
mkdir -p /home/vagrant/arcemu
mkdir -p /home/vagrant/arcemu/src
cp -r /vagrant/.git /home/vagrant/arcemu/src/
cd /home/vagrant/arcemu/src && git reset --hard HEAD

# Load database schemas
cat /home/vagrant/arcemu/src/sql/logon_structure.sql | mysql -u arcemu --password=arcemu arcemu_logon
cat /home/vagrant/arcemu/src/sql/world_structure.sql | mysql -u arcemu --password=arcemu arcemu_world
cat /home/vagrant/arcemu/src/sql/character_structure.sql | mysql -u arcemu --password=arcemu arcemu_character

# Create an admin account, admin:admin
echo "INSERT INTO accounts VALUES (1,'admin','','8301316d0d8448a34fa6d0c6bf1cbfa2b4a1a93a','az',0,NOW(),'','admin@admin',24,'enUS',0,'');" | mysql -u arcemu --password=arcemu arcemu_logon
	 
# Configure build
mkdir /home/vagrant/arcemu/bin
mkdir /home/vagrant/arcemu/build
cd /home/vagrant/arcemu/build
cmake -DCMAKE_INSTALL_PREFIX=/home/vagrant/arcemu/bin -DCMAKE_BUILD_TYPE=Debug ../src/cmake 2>&1

# Get the number of virtual CPUs
VCPUS=`cat /proc/cpuinfo | grep -P "^processor" | wc -l`

# Build! NOTE: This can take a very long time.
make -j$((VCPUS+1))

# Install the binaries
make install

# Create some directories
mkdir -p /home/vagrant/arcemu/bin/DBC
mkdir -p /home/vagrant/arcemu/bin/maps
mkdir -p /home/vagrant/arcemu/bin/vmaps
mkdir -p /home/vagrant/arcemu/bin/configs
mkdir -p /home/vagrant/arcemu/bin/log

# Copy config files
cp -r /home/vagrant/arcemu/src/configs/* /home/vagrant/arcemu/bin/configs

# Make everything owned by the vagrant user
chown -R vagrant:vagrant /home/vagrant
