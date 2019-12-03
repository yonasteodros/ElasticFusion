#ifndef POINTCLOUDSTREAMER_H
#define POINTCLOUDSTREAMER_H

#include <pcl/point_cloud.h>
#include <pcl/io/ply_io.h>
#include <boost/asio.hpp>
#include <chrono>

#include <cwipc_util/api_pcl.h>
#include <cwipc_util/api.h>


class PointcloudStreamer
{
private :
        cwipc_pcl_pointcloud pc = new_cwipc_pcl_pointcloud();
        cwipc *obj;
        char *message = NULL;
       // boost::asio::ip::tcp::iostream socketStream_tcp;
       // boost::asio::ip::udp::iostream socketStream_udp;
public:
    PointcloudStreamer();
    int Stream(cwipc_pcl_pointcloud points);
};



#endif // POINTCLOUDSTREAMER_H
