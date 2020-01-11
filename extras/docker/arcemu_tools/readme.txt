This directory contains the Docker source files for the Arcemu tools docker image.

How to make this work:
1. Copy the tool binaries to srv/arcemu_tools/ subdirectory.
2. Create the image: 
    sudo docker -t arcemu_tools .

3. Run the extractors from the Docker image, with the Lich King 3.3.5a client's directory mounted. Example:
    sudo docker run -ti -v /mnt/wow335a:/srv/arcemu_tools/data:rw arcemu_tools

The tools in the Docker container will extract the DBC, map and vmap files to the client's directory this way.
