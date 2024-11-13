# CMake C++ example project for depthai-core library

CMake example project which serves as a template on how to quickly get started with C++ and depthai library.

This project is based on [this example](https://github.com/luxonis/depthai-core-example), but it follows along with the most current instructions from [the underlying library](https://github.com/luxonis/depthai-core/tree/main).

## Depthai library dependencies
- cmake >= 3.4
- C/C++14 compiler

## Submodules
Make sure submodules are initialized and updated and built:
```bash
git submodule update --init --recursive

cd depthai-core
mkdir -p build
cd build
cmake ..
cmake --build . --parallel
cd ../.. # cd to root directory once done
```

## Prerequisites 

### OpenCV

OpenCV is required to run the out-of-box example.
If OpenVINO is installed then no further action is required.

Otherwise install using one of the options below:

- Ubuntu 20.04 - `sudo apt install libopencv-dev`
- Fedora36+ - `sudo dnf install opencv-devel`
 - MacOS - `brew install opencv`
 - Or install from package / sources (Windows and other OSes)
https://docs.opencv.org/master/d0/d3d/tutorial_general_install.html

## Building

Configure and build
```bash
mkdir -p build
sh setup.sh
cmake --build build --parallel
```

## Running

To run the example application 'myapp', navigate to build directory and run 'myapp' executable
```bash
./myapp
```

## Problems

Program doesn't function as expected right away. This can be fixed by running DepthAI viewer ([installation instructions](https://docs.luxonis.com/software/tools/dai-viewer/)):

```bash
python3 -m depthai_viewer
```

After running and closing DepthAI viewer, running the `./myapp` executable will work as expected until the device is disconnected.