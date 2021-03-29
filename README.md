# standalone-error-program
A small, standalone (No CODAL) bit of code to display errors on the micro:bit, for use in DAPLink and as a template for BLE bootloader

## Build

## Install toolchain

Install git, ensure it is available on your platforms path.

Install the arm-none-eabi-\* command line utilities for ARM based devices and ensure they are available on your platforms path.

Install CMake(Cross platform make), this is the entirety of the build system. If on Windows, install ninja.

Install Python (if you are unfamiliar with CMake), python scripts are used to simplify the build process.

Clone this repository


### Compile

From root folder, run

python build.py

Built bin, hex and C header are stored in folder "built"

Documentation for the nrfx library can be found in [nrfx/doc](https://github.com/microbit-foundation/standalone-error-program/tree/master/nrfx/doc)

## License

This repository contains third party code provided by Nordic Semiconductor. This code can be found in the: `nrfx`, `nrfx-support`, and `toolchain` directories.
The license for nrfx can be found [here](nrfx/LICENSE).

Any other files were created by the Micro:bit Educational Foundation and are MIT licensed.

## Code of Conduct

Trust, partnership, simplicity and passion are our core values we live and breathe in our daily work life and within our projects. Our open-source projects are no exception. We have an active community which spans the globe and we welcome and encourage participation and contributions to our projects by everyone. We work to foster a positive, open, inclusive and supportive environment and trust that our community respects the micro:bit code of conduct. Please see our [code of conduct](https://microbit.org/safeguarding/) which outlines our expectations for all those that participate in our community and details on how to report any concerns and what would happen should breaches occur.
