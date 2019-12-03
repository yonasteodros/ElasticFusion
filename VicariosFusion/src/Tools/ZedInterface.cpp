#include "ZedInterface.h"
#include <functional>

#ifdef WITH_ZED
#include <stdio.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


cv::Mat slMat2cvMat(sl::Mat &input);


ZedInterface::ZedInterface(int inWidth,int inHeight)
  : width(inWidth),
  height(inHeight)
{
  initSuccessful = false;
  
  latestDepthIndex.assign(-1);

  init_params.camera_fps = 60; // Set fps at 30
 // init_params.camera_resolution = sl::RESOLUTION_HD1080;  
 init_params.camera_resolution = sl::RESOLUTION_HD720;
  //init_params.camera_resolution = sl::RESOLUTION_VGA;
  init_params.depth_mode = sl::DEPTH_MODE_PERFORMANCE;
  init_params.coordinate_system = sl::COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP; // OpenGL's coordinate system is right_handed
  init_params.coordinate_units = sl::UNIT_METER;    
  init_params.depth_minimum_distance = 0.2;
     
  // Open the camera
  sl::ERROR_CODE err = zed.open(init_params);
  if (err != sl::SUCCESS) {
      printf("ZedInterface: %s\n", toString(err).c_str());
      closeZED();
  }


  setAutoExposure(true);
  setAutoWhiteBalance(true);


  for(int i = 0; i < numBuffers; i++)
  {
    uint8_t * newDepth = (uint8_t *)calloc(width * height * 2,sizeof(uint8_t));
    uint8_t * newImage = (uint8_t *)calloc(width * height * 3,sizeof(uint8_t));   
    frameBuffers[i] = std::pair<std::pair<uint8_t *,uint8_t *>,int64_t>(std::pair<uint8_t *,uint8_t *>(newDepth,newImage),0);
  }
  
  initSuccessful = true;
  
  startZED();
  
}



ZedInterface::~ZedInterface()
{
  if(initSuccessful)
  {
    closeZED();
    
    for(int i = 0; i < numBuffers; i++)
    {
      free(frameBuffers[i].first.first);
      free(frameBuffers[i].first.second);
    }
   
  }
}



void ZedInterface::setAutoExposure(bool value)
{
  zed.setCameraSettings(sl::CAMERA_SETTINGS_EXPOSURE, -1, value);  
}

void ZedInterface::setAutoWhiteBalance(bool value)
{
  zed.setCameraSettings(sl::CAMERA_SETTINGS_WHITEBALANCE, -1, value);  
}

bool ZedInterface::getAutoExposure()
{
  return zed.getCameraSettings(sl::CAMERA_SETTINGS_EXPOSURE)>0;
}

bool ZedInterface::getAutoWhiteBalance()
{
  return zed.getCameraSettings(sl::CAMERA_SETTINGS_WHITEBALANCE)>0;
}



void ZedInterface::startZED() {
    quit = false;
    while (zed.grab() != sl::SUCCESS)
           sl::sleep_ms(1);            
    zed_callback = std::thread(&ZedInterface::run, this);
}


void ZedInterface::closeZED() {
    quit = true;  
    zed_callback.join();
    zed.close();
}

void ZedInterface::run() {

    // Set runtime parameters after opening the camera
    sl::RuntimeParameters  runtime_parameters;    
    runtime_parameters.sensing_mode = sl::SENSING_MODE_STANDARD; //SENSING_MODE_FILL with stabilization

    
    int new_width =  (int) width; 
    int new_height = (int) height; 


    sl::Mat image_zed(new_width, new_height, sl::MAT_TYPE_8U_C4); 
       
    sl::Mat depth_image_zed(new_width, new_height, sl::MAT_TYPE_32F_C1);


    while (!quit) {  
    //  /usr/local/zed/tools/'ZED Depth Viewer'

        if (zed.grab(runtime_parameters) == sl::SUCCESS) {     

           zed.retrieveImage(image_zed, sl::VIEW_LEFT, sl::MEM_CPU, new_width, new_height);
          // zed.retrieveImage(depth_image_zed, sl::VIEW_DEPTH, sl::MEM_CPU, new_width, new_height); 
           zed.retrieveMeasure(depth_image_zed, sl::MEASURE_DEPTH, sl::MEM_CPU, new_width,new_height);      

           lastDepthTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

           int bufferIndex = (latestDepthIndex.getValue() + 1) % numBuffers; 
                             
           frameBuffers[bufferIndex].second = lastDepthTime;          


//=========================================================================


           cv::Mat image_ocv = slMat2cvMat(image_zed);
           //std::cout << "size  = " <<image_ocv.size();
           cv::cvtColor(image_ocv, image_ocv, cv::COLOR_RGBA2BGR);
           memcpy(frameBuffers[bufferIndex].first.second, image_ocv.data,  new_width * new_height * 3);

//============================================================================= 

           cv::Mat depth_image_ocv = slMat2cvMat(depth_image_zed); 
           cv::Size cvSize(new_width, new_height);
           cv::Mat depth(cvSize, CV_16UC1);           
           depth_image_ocv *= 1000.0f;
           depth_image_ocv.convertTo(depth, CV_16UC1); // in mm, rounded             
           memcpy(frameBuffers[bufferIndex].first.first, depth.data , new_width * new_height * 2);


           
 //=======================================================================          
           
           latestDepthIndex++;

        } else sl::sleep_ms(1);
    }
}





        cv::Mat slMat2cvMat(sl::Mat &mat) {
            if (mat.getMemoryType() == sl::MEM_GPU)
                mat.updateCPUfromGPU();

            int cvType;
            switch (mat.getDataType()) {
                case sl::MAT_TYPE_32F_C1:
                    cvType = CV_32FC1;
                    break;
                case sl::MAT_TYPE_32F_C2:
                    cvType = CV_32FC2;
                    break;
                case sl::MAT_TYPE_32F_C3:
                    cvType = CV_32FC3;
                    break;
                case sl::MAT_TYPE_32F_C4:
                    cvType = CV_32FC4;
                    break;
                case sl::MAT_TYPE_8U_C1:
                    cvType = CV_8UC1;
                    break;
                case sl::MAT_TYPE_8U_C2:
                    cvType = CV_8UC2;
                    break;
                case sl::MAT_TYPE_8U_C3:
                    cvType = CV_8UC3;
                    break;
                case sl::MAT_TYPE_8U_C4:
                    cvType = CV_8UC4;
                    break;
            }
            return cv::Mat((int) mat.getHeight(), (int) mat.getWidth(), cvType, mat.getPtr<sl::uchar1>(sl::MEM_CPU), mat.getStepBytes(sl::MEM_CPU));
        }
#else

ZedInterface::ZedInterface(int inWidth,int inHeight)
  : width(inWidth),
  height(inHeight),
  initSuccessful(false)
{
  errorText = "Compiled without stereolabs zed library";
}

ZedInterface::~ZedInterface()
{
}

void ZedInterface::setAutoExposure(bool value)
{
}

void ZedInterface::setAutoWhiteBalance(bool value)
{
}

bool ZedInterface::getAutoExposure()
{
  return false;
}

bool ZedInterface::getAutoWhiteBalance()
{
  return false;
}
#endif




