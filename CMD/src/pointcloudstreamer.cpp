#include <iostream>
#include <fstream>
#include "pointcloudstreamer.h"

PointcloudStreamer::PointcloudStreamer()
{
    std::cout<<" The stream class is created"<<std::endl;
}

int PointcloudStreamer::Stream()
{
   // int status = cwipc_write("Egyptian_fused.ply", obj, &message);

    //if (status < 0) {
    //    std::cerr << "Egyptian_fused.ply" << ": Cannot save pointcloud to ply: " << message << std::endl;
     //   return 1;
  //  }
}
