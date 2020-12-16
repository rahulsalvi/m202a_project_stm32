# STM32 Datalogger

This repo contains the code for the Nucleo board for my M202A project

# Building

You need arm-none-eabi-gdb

Building and deploying:
```bash
$ make
$ JLinkExe -device STM32F439ZI -if SWD -speed auto -autoconnect 1 -CommandFile
flash_bin.jlink
```

To view the printf output:
```bash
$ JLinkRTTClient
```
