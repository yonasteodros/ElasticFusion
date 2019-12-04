#include "pointcloudstreamer.h"


PointcloudStreamer::PointcloudStreamer()
{

}

PointcloudStreamer::PointcloudStreamer(bool parameters)
{
    param.do_inter_frame = false;
    param.gop_size = 1;
    param.exp_factor = 1.0;
    param.octree_bits = 9;
    param.jpeg_quality = 85;
    param.macroblock_size = 16;
    param.tilenumber = 0;
    encoder = cwipc_new_encoder(CWIPC_ENCODER_PARAM_VERSION, &param, &errorString, CWIPC_API_VERSION);
    if (encoder == NULL) {
            std::cerr << ": Could not create encoder: " << errorString << std::endl;
      }

}

int PointcloudStreamer::compression(cwipc_pcl_pointcloud test)
{

     bool status =true;
     try
        {

                uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                cwipc *obj = cwipc_from_pcl(test, timestamp, NULL, CWIPC_API_VERSION);

                std::cout<<" Size of uncompressed pt : "<<obj->get_uncompressed_size()<< " bytes." <<std::endl;
                encoder->feed(obj);
                obj->free();	// After feeding the pointcloud into the encoder we can free it.
                bool ok = encoder->available(true);
                if (!ok) {
                        std::cerr <<": Encoder did not create compressed data" << std::endl;
                 }
                size_t bufSize = encoder->get_encoded_size();
                char *buffer = (char *)malloc(bufSize);
                if (buffer == NULL) {
                    std::cerr <<": Could not allocate " << bufSize << " bytes." << std::endl;
                }

                ok = encoder->copy_data(buffer, bufSize);
                if (!ok) {
                    std::cerr <<": Encoder could not copy compressed data" << std::endl;
                    return 1;
                }
                encoder->free(); // We don't need the encoder anymore.
                std::cerr << "Encoded successfully, " << bufSize << " bytes." << std::endl;
                //
                // Save to output
                //
                output.open("compressedfile.cwicpc", std::ofstream::binary);
                output << buffer;
                boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
                output.close();

        }
        catch (std::exception& e)
        {
            std::cerr << "Exception caught : " << e.what() << std::endl;
        }

   return 0;
}

int PointcloudStreamer::udpstream(cwipc_pcl_pointcloud test)
{



}

int PointcloudStreamer::tcpstream(cwipc_pcl_pointcloud test)
{



}
