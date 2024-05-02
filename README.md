# asabru-client
### This project is an executable used along with ChistaDATA Database Proxy ( chista-asabru )

This project contains code for the ChistaDATA database proxy client.

### Prerequisite

Make
`https://www.gnu.org/software/make`

CMake
`https://cmake.org/install/`

### Build

```
mkdir build
cd build
cmake ..
make
```

To use local directory files , use the command
```
cmake -DASABRU_COMMONS_BUILD=LOCAL_DIR -DASABRU_ENGINE_BUILD=LOCAL_DIR -DASABRU_PARSERS_BUILD=LOCAL_DIR ..
```