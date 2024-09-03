# bounce-world-client

A cc65 fujinet-lib application to connect to bouncy-world service.

You will need to run a bouncy world service from https://github.com/markjfisher/bounce-world

## building

Standard make for cc65 projects with fujinet-lib:

```shell
make clear
make release disk
```

Specify `TARGETS` value if you only wish to compile a set of clients. This is normal cc65 behaviour.

## endpoint URL

You can specify the endpoint URL when compiling the application which will embed it in the application and not aske for it when the application is run.
This is useful for shows etc or distrubuting a client for a known DNS endpoint.

```shell
make ENDPOINT_URL="http://192.168.0.200" release disk
```

## running

Deploy the released binary for the platform to a FujiNet (e.g. via SD or TNFS), and run it in your usual way.
It will ask you for the location of the bouncy world service. Enter as a simple http url, e.g. `http://localhost:8080`.
There is no need for "n:" part, that is done for you.

## testing

See the `makefiles/custom-<platform>.mk` files for requirements to run code under emulation.
Supported at the moment is Altirra on Atari, simply set the `ALTIRRA_HOME` value and run:

```shell
make test
```

## copying to SD

The fujinet supports webdav copying to the SD, and the cyberduck cli command `duck` can be used to do this on the command line.
This is particularly useful for testing c64 programs.

```shell
duck --upload dav://anonymous@fujinet.home/dav/bwc.prg dist/bwc.c64.prg -existing overwrite
```
