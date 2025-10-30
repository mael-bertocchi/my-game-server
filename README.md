# My Game Server

This is the server part of the 3rd year project R-Type for Epitech. We were in group of 4 students, I have worked on the server part of the project and the rest of the team worked on the client part.

<div style="text-align:center"><img src="assets/title-screen.gif" alt="Server Title" width="600" /></div>

------

## Requirements

We are using [CMake](https://cmake.org/) as build system, and [vcpkg](https://vcpkg.io/) to manage dependencies.

------

## Getting Started

### Database

We use postgresql as our database system. You need to have a running instance of postgresql to use the server.

Additionally, you need to load the schema into your database. You can find the schema in the `schema.sql` file. You can load it using the following command:

```bash
psql --host <host> --username <user> --dbname <dbname> --port <port> --password --file schema.sql
```

### Configuration

We use a configuration file to set up the server. An example configuration file is provided in `config/example.cfg`. You can copy this file and modify it according to your needs.

### Compilation

To compile the project, you can run the following commands:

1. Clean previous build:

```bash
rm -rf build
```

2. Configure the build:

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

3. Build the project:

```bash
cmake --build build --parallel
```

### Usage

You can run the server with the following command, providing a path to your configuration file:

```bash
./r-type_server --file path/to/your/config.cfg [--debug]
```

The `--debug` option is optional and can be used to enable logging with different verbosity levels:

- `--debug default`: Logs all events except network requests
- `--debug network`: Logs all events including network requests

------

## Scripting

The server supports scripting using Lua. You can find the Lua scripts in the `examples` directory. You can modify these scripts to change the behavior of the server or create your own scripts.

------

## Documentation

We use Doxygen to generate documentation for the project. You can generate the documentation by running the following command:

```bash
doxygen Doxyfile
```

We also have documented the code for each action we can send or receive from the client. You can find this documentation in the `docs` directory.
