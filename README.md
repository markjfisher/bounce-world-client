# bounce-world-client

A cc65 fujinet-lib application to connect to bouncy-world service.

You will need to run a bouncy world service from https://github.com/markjfisher/bounce-world

## building

Standard make for cc65 projects with fujinet-lib:

```shell
make clear release
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
