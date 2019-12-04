#ifndef POINTCLOUDSTREAMER_H
#define POINTCLOUDSTREAMER_H
#include <iostream>
#include <fstream>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <pcl/point_cloud.h>
#include <pcl/io/ply_io.h>

#include <cwipc_util/api_pcl.h>
#include <cwipc_util/api.h>
#include <cwipc_codec/api.h>
#include <fstream>

class PointcloudStreamer
{
private :
        cwipc_pcl_pointcloud pc = new_cwipc_pcl_pointcloud();

        cwipc *obj;
        char *message = NULL;
        char *errorMessage = NULL;
        char *errorString;
        cwipc_encoder_params param;
        cwipc_encoder *encoder;
        std::ofstream output;


public:
    PointcloudStreamer();
    PointcloudStreamer(bool parameters);
    int udpstream(cwipc_pcl_pointcloud points);
    int tcpstream(cwipc_pcl_pointcloud points);
    int compression(cwipc_pcl_pointcloud points);
};



#endif // POINTCLOUDSTREAMER_H
