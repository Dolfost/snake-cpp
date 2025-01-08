##### Platform-agnostic CI/CD
This is a platform-agnostic set of scripts for continuous integration and
deployment. There is host-specific setup scripts in directory with this
README.md which handle setting up the host system before calling CMake script
in `cmake/<task>.cmake`. Each of setup scripts accepts one argument from shell,
namely the task name. Then it examines it and decides what dependencies to
install to satisfy `cmake/<task>.cmake` requirements and sets environment
variable `REPO` to the full path to repository root and
`TARTAN_CONFIGURATION_OPTIONS` to valid cmake configuration options.
Corresponding CMake scripts use it for running commands.

###### Synopsis
```
ubuntu.sh <task>
darwin.sh <task>
windows.bat <task>
```
where `<task>` is in `cmake/<task>.cmake`.
