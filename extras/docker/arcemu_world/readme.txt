Steps to make this work:

0. Copy the entire directory somewhere else, so you don't pollute the source...

1. Copy world.conf, optional.conf, realms.conf to ./srv/arcemu_world/etc
2. Copy world to ./srv/arcemu_world
3. Copy the modules to ./srv/arcemu_world/modules

4. Create a volume for the logs, DBC and maps:
docker volume create arcemu_world_logs
docker volume create arcemu_world_DBC
docker volume create arcemu_world_maps

5. Copy the DBC files to /var/lib/docker/volumes/arcemu_world_DBC/_data
6. Copy the map files to /var/lib/docker/volumes/arcemu_world_maps/_data

7. Create the Docker image:
docker build -t arcemu_world .

8. Run the image:
docker run --network host -td -v arcemu_world_logs:/srv/arcemu_world/log -v arcemu_world_DBC:/srv/arcemu_world/DBC -v arcemu_world_maps:/srv/arcemu_world/maps arcemu_world
