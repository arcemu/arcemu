This is a docker-compose file for Arcemu.

Prerequisits:
It uses a database that runs on 127.0.0.1, so make sure you have a MySql 5.1+ database running there, with the Arcemu schema loaded.

Steps to make this work:

1. Create the neccessary volumes with the following commands:
sudo docker volume create arcemu_logon_etc
sudo docker volume create arcemu_logon_logs
sudo docker volume create arcemu_world_etc
sudo docker volume create arcemu_world_logs
sudo docker volume create arcemu_world_DBC
sudo docker volume create arcemu_world_maps
sudo docker volume create arcemu_world_scripts

2. Copy the neccessary files:
Copy logon.conf to /var/lib/docker/volumes/arcemu_logon_etc/_data
Copy optional.conf, realms.conf, world.conf to /var/lib/docker/volumes/arcemu_world_etc/_data
Copy the DBC files to /var/lib/docker/volumes/arcemu_world_DBC/_data
Copy the map files to /var/lib/docker/volumes/arcemu_world_maps/_data
(Optionally) Copy your Lua scripts to /var/lib/docker/volumes/arcemu_world_scripts/_data

3. Start the servers
sudo docker-compose up

docker-compose will pull the Arcemu docker images, start them up with the volumes mounted and you are ready to roll! :)
