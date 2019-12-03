#include <iostream>
#include <fstream>
#include "pointcloudstreamer.h"

PointcloudStreamer::PointcloudStreamer()
{

    std::cout<<" The stream class is created"<<std::endl;
}

int PointcloudStreamer::Stream(cwipc_pcl_pointcloud test)
{

         bool status =true;

        try
        {
                //obj= cwipc_read(test, 0, &message, CWIPC_API_VERSION);
                uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                cwipc *obj = cwipc_from_pcl(test, timestamp, NULL, CWIPC_API_VERSION);
                //obj = cwipc_from_pcl()
                if (obj == NULL) {
                    std::cout<<" Cannot read pointcloud: %s\n"<< message;
                    return 1;
                }
                else
                {
                   int sta = cwipc_write_debugdump("pointcloud1.cwipcdump", obj, &message);
                   if (sta < 0) {
                       std::cout<<" Cannot save pointcloud to cwipcdump: %s\n"<< message;
                        }
                   else{
                       status=false;
                   }
                }
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception caught : " << e.what() << std::endl;
        }

   return 0;
}
