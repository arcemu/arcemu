Steps to make this work:

0. Copy the entire directory somewhere else, so you don't pollute the source...

1. Copy world to ./srv/arcemu_world
2. Copy the modules to ./srv/arcemu_world/modules

3. Create a volume for the logs, DBC and maps, and config files:
docker volume create arcemu_world_logs
docker volume create arcemu_world_DBC
docker volume create arcemu_world_maps
docker volume create arcemu_world_etc
docker volume create arcemu_world_scripts

4. Copy the DBC files to /var/lib/docker/volumes/arcemu_world_DBC/_data
5. Copy the map files to /var/lib/docker/volumes/arcemu_world_maps/_data
6. Copy the config files to /var/lib/docker/volumes/arcemu_world_etc/_data
7. (OPTIONAL) If you have any, copy the script files (.lua) to /var/lib/docker/volumes/arcemu_world_scripts/_data

8. Create the Docker image:
docker build -t arcemu_world .

9. Run the image:
docker run --network host -ti -v arcemu_world_etc:/srv/arcemu_world/etc:ro -v arcemu_world_logs:/srv/arcemu_world/log -v arcemu_world_DBC:/srv/arcemu_world/DBC -v arcemu_world_maps:/srv/arcemu_world/maps -v arcemu_world_scripts:/srv/arcemu_world/scripts arcemu_world
