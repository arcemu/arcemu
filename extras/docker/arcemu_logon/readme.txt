Steps to make this work:

0. Copy the entire directory somewhere else, so you don't pollute the source...

1. Copy logon.conf to srv/arcemu_logon/etc
2. Copy logon to srv

3. Create a volume for the logs:
docker volume create arcemu_logon_logs

4. Create the Docker image:
docker build -t arcemu_logon .

5. Run the image:
docker run --network host -d -v arcemu_logon_logs:/srv/arcemu_logon/log arcemu_logon
