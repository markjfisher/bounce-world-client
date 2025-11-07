# Changelog

## [Unreleased]

## [2.0.3] - 2025-11-07

Fix indexing variable to be 16 bit to support > 85 shapes. Server version 2.1.0 caps
at 240 objects per client, which is ridiculously large number for an 8 bit client as
it slows the client down a lot. The server can still handle millions, but client
has to render them all per frame

Increase buffer size for client app data to 1024 to ensure 240 shapes doesn't cause
buffer overrun.

## [2.0.2] - 2025-11-02

- [cc65] Consolidated on single get_line.c for all cc65 targets [Mark Fisher]
- [coco] Fix cursor for coco [Rich Stephens]
- [fujinet-lib] Bump to version 4.8.1 with coco fix to undo dragon changes

## [2.0.1] - 2025-10-29

- Removed ENDPOINT_URL in favour of appkeys by Rich Stephens in last release
- Fixed some cursor showing/not-showing for atari (only tested on atari)
- Updated to newer version of the build system (pre moving to another build system)

The update to the build system means all downloaded files are under "_cache" instead of "_libs"
and also have sub-dirs for the type of resource.

People updating to this version can now remove the binary files under apple-tools (*.jar and PRODOS binary),
and their entire _libs dir.

The _cache dir can always be completely removed, and it will be reinstated when running build again.


## Pre-2.0.1 no changelogs captured
