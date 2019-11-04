# Hello fuse
A small FUSE driver proving a filesystem with the only file "hello". 

### Build it.
A Dockerfile provided to build a container with `libfuse` installed and `hello-filesystem` built.
Run `run_docker.sh` to launch the container with interactive bash session.

### Try it.
To mount the `hello-filesystem`, inside of the docker container, run
```
./hello <mountpoint>
```
Now you can `ls` the mounted directory, find the `hello` file and even `cat` it.
