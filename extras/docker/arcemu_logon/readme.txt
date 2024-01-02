Steps to make this work:

0. Copy the entire directory somewhere else, so you don't pollute the source...
1. Copy logon to srv

2. Create a volume for the config files and logs:
docker volume create arcemu_logon_logs
docker volume create arcemu_logon_configs

3. Copy logon.conf to /var/lib/docker/volumes/arcemu_logon_configs/_data

4. Create the Docker image:
docker build -t arcemu_logon .

5. Run the image:
docker run --network host -ti -v arcemu_logon_configs:/srv/arcemu_logon/configs:ro -v arcemu_logon_logs:/srv/arcemu_logon/log arcemu_logon
