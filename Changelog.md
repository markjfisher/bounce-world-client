# Changelog

## [Unreleased]

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
