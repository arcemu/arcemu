This is a docker image for building Arcemu

Instructions

1. Build the image 
docker build -t arcemu_build .

2. Run the dockerized build
docker run -t -i -v `pwd`/bin:/srv/build/arcemu/bin arcemu_build
