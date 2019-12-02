#include <iostream>
#include <fstream>
#include "pointcloudstreamer.h"

PointcloudStreamer::PointcloudStreamer()
{
    std::cout<<" The stream class is created"<<std::endl;
}

int PointcloudStreamer::Stream()
{

         int status;
        try
        {
            status = cwipc_write("/home/user/Development/3DDataSet/Egyptian_fused.ply", obj, &message);
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception caught : " << e.what() << std::endl;
        }


    if (status < 0) {
        std::cerr << "Egyptian_fused.ply" << ": Cannot save pointcloud to ply: " << message << std::endl;
        return 1;
     }
   return 0;
}
