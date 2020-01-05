Steps to make this work:

0. Copy the entire directory somewhere else, so you don't pollute the source...

1. Copy logon.conf to srv/arcemu_logon/etc
2. Copy logon to srv

3. Create a volume for the config files and logs:
docker volume create arcemu_logon_logs
docker volume create arcemu_logon_etc

4. Copy the config file to /var/lib/docker/volumes/arcemu_logon_etc/_data

5. Create the Docker image:
docker build -t arcemu_logon .

6. Run the image:
docker run --network host -v arcemu_logon_etc:/srv/arcemu_logon/etc:ro  -v arcemu_logon_logs:/srv/arcemu_logon/log arcemu_logon
