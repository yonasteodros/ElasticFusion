#ifndef POINTCLOUDSTREAMER_H
#define POINTCLOUDSTREAMER_H

#include <pcl/point_cloud.h>
#include <pcl/io/ply_io.h>

#include "cwipc_util/api_pcl.h"
#include "cwipc_util/api.h"


class PointcloudStreamer
{
private :
        cwipc *obj;
        char *message = NULL;
public:
    PointcloudStreamer();
    int Stream();
};



#endif // POINTCLOUDSTREAMER_H
