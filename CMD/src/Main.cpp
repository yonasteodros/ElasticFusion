/*
 * This file is part of ElasticFusion.
 *
 * Copyright (C) 2015 Imperial College London
 * 
 * The use of the code within this file and all code within files that 
 * make up the software that is ElasticFusion is permitted for 
 * non-commercial purposes only.  The full terms and conditions that 
 * apply to the code within this file are detailed within the LICENSE.txt 
 * file and at <http://www.imperial.ac.uk/dyson-robotics-lab/downloads/elastic-fusion/elastic-fusion-license/> 
 * unless explicitly stated.  By downloading this file you agree to 
 * comply with these terms.
 *
 * If you wish to use any of this code for commercial purposes then 
 * please email researchcontracts.engineering@imperial.ac.uk.
 *
 */

#include "MainController.h"
#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>

cv::Mat slMat2cvMat(sl::Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case sl::MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
        case sl::MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
        case sl::MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
        case sl::MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
        case sl::MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
        case sl::MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
        case sl::MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
        case sl::MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
        default: break;
    }
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM_CPU));
}

int main(int argc, char * argv[])
{
   //MainController mainController(argc, argv);
   // mainController.launch();

    static const int w = 640;
    static const int h = 480;
    int64_t lastDepthTime;

    pangolin::CreateWindowAndBind("Main",w,h,pangolin::Params({{"scheme", "headless"}}));
    //Elastic fusion processs*/
    Resolution::getInstance(640, 480);
    Intrinsics::getInstance(528, 528, 320, 240);


    // Create a ZED camera object
    sl::Camera zed;

    // Set configuration parameters
    // Set configuration parameters
    sl::InitParameters init_params;
    init_params.camera_resolution = sl::RESOLUTION_HD1080;
    init_params.depth_mode = sl::DEPTH_MODE_PERFORMANCE;
    init_params.coordinate_units = sl::UNIT_METER;

    // Open the camera
    sl::ERROR_CODE err = zed.open(init_params);
    if (err != sl::SUCCESS)
        exit(-1);

    // Set runtime parameters after opening the camera
    sl::RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = sl::SENSING_MODE_STANDARD; // Use STANDARD sensing mode



    // Prepare new image size to retrieve half-resolution images
    sl::Resolution image_size = zed.getResolution();
    int new_width = image_size.width / 2;
    int new_height = image_size.height / 2;



    // To share data between sl::Mat and cv::Mat, use slMat2cvMat()
    // Only the headers and pointer to the sl::Mat are copied, not the data itself
    sl::Mat image_zed(new_width, new_height, sl::MAT_TYPE_8U_C4);
    cv::Mat image_ocv = slMat2cvMat(image_zed);
    sl::Mat depth_image_zed(new_width, new_height, sl::MAT_TYPE_8U_C4);
    cv::Mat depth_image_ocv = slMat2cvMat(depth_image_zed);
    sl::Mat point_cloud;



    // Capture 50 images and depth, then stop
    int i = 0;
    sl::Mat image, depth;
    std::cout << "Creating ElasticFusion object...\n";
    ElasticFusion eFusion;
    Eigen::Matrix4f * currentPose = 0;
    float weightMultiplier = 0;
    char key = ' ';

        while (key != 'q'){
            // A new image is available if grab() returns SUCCESS
            if (zed.grab(runtime_parameters) == sl::SUCCESS) {
                // Retrieve left image
                // Retrieve the left image, depth image in half-resolution
                zed.retrieveImage(image_zed, sl::VIEW_LEFT, sl::MEM_CPU, new_width, new_height);
                zed.retrieveImage(depth_image_zed, sl::VIEW_DEPTH, sl::MEM_CPU, new_width, new_height);
                lastDepthTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                //eFusion.processFrame(image_ocv,depth_image_ocv,lastDepthTime,currentPose,weightMultiplier);
                // Display image and depth using cv:Mat which share sl:Mat data
               cv::imshow("Image", image_ocv);
               cv::imshow("Depth", depth_image_ocv);
               // Handle key event
               key = cv::waitKey(10);
              // eFusion.processFrame();
            }
        }




   // eFusion.processFrame()
    std::cout << " ElasticFusion object created ...\n";
    // Swap frames and Process Events
    pangolin::FinishFrame();
    pangolin::QuitAll();
    // Close the camera
     zed.close();

    return 0;
}
