## Skeltn
skeltn is a build system for C / C++. The main goal of skeltn is to be very simple to use, specifically for desktop applications, meaning it is not a good fit for projects that need particular control over the build process (e.g. kernels).

A secondary goal is to be non-intrusive: because it is a huge pain to have to install somebody's custom build tool, skeltn generates makefiles that can be used to a reasonable extent to build skeltn-based projects without skeltn being present on a system.

## Building
If skeltn is already installed, simply run `skeltn build`. Otherwise, you can use skeltn-generated makefiles:
- Windows: `nmake /F make-windows`
- MacOS: `make -f make-macos` (I'm assuming; I don't actually know if Macs support make)
- Linux: `make -f make-linux`

## Usage
`skeltn help` displays some basic commands. The main idea is that your project includes a `.skeltn` file which tells skeltn how to build your project, at which point running `skeltn build` is used to build your project.

## .skeltn file
The format used in a `.skeltn` file is something similar to TOML, but with significantly fewer features.

It consists of a series of sections, such as `[Project]` or `[Source]`, which either specify certain properties or lists of information.

Under sections which specify properties, properties are specified as `Name: Value`.

A basic `.skeltn` file looks something like:
```
[Project]
Name: MyProject
SourceDirectory: src
Binary: project

[Source]
Main.cpp
Algorithm.cpp

[Library]
SDL2

[Library/Windows]
SDL2main
```

In the future, sections are going to be able to apply to particular operating systems by applying a `/OSName` to that section.