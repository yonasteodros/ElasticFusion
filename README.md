# VicariosFusion Repository  #

This repository is a Dynamic 3D reconstruction pipline. It contains pointcloud streaming and 3d object segmentation methods


## This project is optimised For Jetson AGX Xavier (ARM Processors) #

This repository is based on ElasticFusion dense visual system.I have added some codes to support ARM Processors and ZED and Realsense cameras.

In order to compile for Zed camera don't forget to add the following parameters in bash.sh 

```bash
cd ../../GUI
mkdir build
cd build
cmake -DWITH_ZED=1 ../src/
make -j8
```
For RealSense camera 

```bash
cd ../../GUI
mkdir build
cd build
cmake -DWITH_REALSENSE=1 ../src/
make -j8
```

# dependencies #

- Elastic Fusion 
    [Install it from this link]( Real-time dense visual SLAM system capable of capturing comprehensive dense globally consistent surfel-based maps of room scale environments explored using an RGB-D camera.)
- Deeplearning segemntation
    [ Install it from this link] (https://github.com/dbolya/yolact#you-only-look-at-coefficients)

