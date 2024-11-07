#include <iostream>
#include <depthai/depthai.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>

int main()
{

    // Create output directory
    const std::string outputDir = "../output";

    // Create pipeline
    dai::Pipeline pipeline;

    // Create color camera node
    auto colorCam = pipeline.create<dai::node::ColorCamera>();
    // colorCam->setPreviewSize(300, 300);
    colorCam->setInterleaved(false);
    colorCam->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    colorCam->setColorOrder(dai::ColorCameraProperties::ColorOrder::BGR);

    // Create mono cameras for stereo depth
    auto monoLeft = pipeline.create<dai::node::MonoCamera>();
    auto monoRight = pipeline.create<dai::node::MonoCamera>();
    monoLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);
    monoRight->setResolution(dai::MonoCameraProperties::SensorResolution::THE_720_P);
    monoLeft->setBoardSocket(dai::CameraBoardSocket::LEFT);
    monoRight->setBoardSocket(dai::CameraBoardSocket::RIGHT);

    // Create stereo depth node
    auto stereo = pipeline.create<dai::node::StereoDepth>();
    stereo->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
    stereo->setDepthAlign(dai::CameraBoardSocket::RGB);
    monoLeft->out.link(stereo->left);
    monoRight->out.link(stereo->right);

    // Create XLinkOut nodes for preview and depth
    auto xoutPreview = pipeline.create<dai::node::XLinkOut>();
    auto xoutDepth = pipeline.create<dai::node::XLinkOut>();
    xoutPreview->setStreamName("preview");
    xoutDepth->setStreamName("depth");

    // Link nodes
    colorCam->video.link(xoutPreview->input);
    stereo->depth.link(xoutDepth->input);

    try
    {
        // Connect to device and start the pipeline
        dai::Device device(pipeline);

        // Get output queues
        std::cout << "Line 53\n";
        auto previewQueue = device.getOutputQueue("preview", 4, false);
        auto depthQueue = device.getOutputQueue("depth", 4, false);
        std::cout << "Line 56\n";

        int i = 0;
        while (true)
        {
            // Get preview frame
            auto previewFrame = previewQueue->get<dai::ImgFrame>();
            cv::Mat previewMat = previewFrame->getCvFrame();

            // Get depth frame
            auto depthFrame = depthQueue->get<dai::ImgFrame>();
            cv::Mat depthMat = depthFrame->getFrame();

            // Normalize depth for visualization
            cv::Mat depthVis;
            cv::normalize(depthMat, depthVis, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            cv::applyColorMap(depthVis, depthVis, cv::COLORMAP_JET);

            // Display frames
            cv::imshow("Preview", previewMat);
            cv::imshow("Depth", depthVis);

            if (i < 10)
            {
                // // For PNG files:
                // cv::imwrite("output/cam_" + i, previewMat);
                // cv::imwrite("output/depth_" + i, depthVis);
                // For raw binary:

                std::ofstream depthFile(outputDir + "/depth_" + std::to_string(i) + ".bin", std::ios::binary);
                depthFile.write(
                    reinterpret_cast<const char *>(depthMat.data),
                    depthMat.total() * depthMat.elemSize()
                );

                std::ofstream previewFile(outputDir + "/preview_" + std::to_string(i) + ".bin", std::ios::binary);
                previewFile.write(
                    reinterpret_cast<const char *>(previewMat.data),
                    previewMat.total() * previewMat.elemSize()
                );
                i++;
            }

                if (cv::waitKey(1) == 'q') break;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
