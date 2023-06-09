/*author: RISHABH SINGH, HRITHIK KANOJE
Class: CS5330 Pattern Recog & Computer Vision
Prof: Bruce Maxwell
Project 4: Calibration and Augmented Reality
*/

#include <iostream>
#include "utils.h"

#if 0
int main() {

    std::string argv0 = "a.out";
    std::string argv1 = "chessboard";
    std::string argv2 = "E:\\PRCV Assignments\\CandAR\\objs\\teapot";
    int argc = 3;
    char* argv[3];
    argv[0] = _strdup(argv0.c_str());
    argv[1] = _strdup(argv1.c_str());
    argv[2] = _strdup(argv2.c_str());
    
    if (argc != 3) {
        std::cout << "Usage: ./ar.exe <target> <obj_name>" << std::endl;
        std::cout << "<target> should be chessboard, <obj_name> should be one in objs folder" << std::endl;
        exit(-1);
    }
    

    // circle grid configuration
    cv::SimpleBlobDetector::Params params;
    params.maxArea = 10e4;
    params.minArea = 10;
    params.minDistBetweenBlobs = 5;
    // Initialize the simple blob detector with the parameters above for the findCirclesGrid() function to find the circle grids
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

    int points_per_row, points_per_column;
    std::string target = std::string(argv[1]);
    if (target == "chessboard") {
        points_per_row = 9;
        points_per_column = 6;
    }
  
    else {
        std::cout << "Usage: ./ar.exe <camera_index> <target> <obj_name>" << std::endl;
        std::cout << "<camera_index> could be any integer, <target> should be chessboard " << std::endl;
        exit(-1);
    }

    cv::Size pattern_size = cv::Size(points_per_row, points_per_column);

    std::vector<cv::Point2f> corner_set;
    std::vector<cv::Vec3f> point_set;

    cv::TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03);
    cv::Mat camera_matrix;
    cv::Mat distortion_coefficients;
    cv::Mat rvec, tvec;
    bool show_obj = false;
    int count = 0;

    std::vector<cv::Point3f> vertices;
    std::vector<std::vector<int>> faces;
    if (target == "chessboard") {
        read_object(std::string(argv[2]) + ".obj", vertices, faces, 4.f, -2.5f);
    }
   

    cv::VideoCapture* capdev;
    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return (-1);
    }
    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);
    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;
    cv::Mat grey_scale;

    // read in the intrinsic parameters
    read_intrinsic_parameters("camera_intrinsic_paras_" + target + ".csv", camera_matrix, distortion_coefficients);

    for (;;) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        cv::cvtColor(frame, grey_scale, cv::COLOR_BGR2GRAY);

        corner_set.clear();
        point_set.clear();

        bool corner_find_flag;
        if (target == "chessboard") {
            corner_find_flag = cv::findChessboardCorners(frame, pattern_size, corner_set);
        }
    
        if (corner_find_flag) {
            cv::cornerSubPix(grey_scale, corner_set, cv::Size(5, 5), cv::Size(-1, -1), termcrit);
            cv::drawChessboardCorners(frame, pattern_size, corner_set, corner_find_flag);

            if (target == "chessboard") {
                for (int i = 0; i < points_per_column; i++) {
                    for (int j = 0; j < points_per_row; j++) {
                        point_set.push_back(cv::Point3f((float)j, (float)(-i), 0));
                    }
                }
            }
           
            cv::solvePnP(point_set, corner_set, camera_matrix, distortion_coefficients, rvec, tvec);
            print_matrix("rotation matrix", rvec);
            print_matrix("translation matrix", tvec);
            construct_axes(rvec, tvec, camera_matrix, distortion_coefficients, frame, corner_set[0]);
        }

        // see if there is a waiting keystroke
        int key = cv::waitKey(10);
        // if user types 'q', quit the program
        if (key == 'q') {
            break;
        }
        // if user types 'a', only shows the axes
        if (key == 'a') {
            show_obj = false;
        }
        // if user types 'w', keep showing the axes and 3d object
        else if (key == 'w' || show_obj) {
            if (!corner_find_flag) {
                std::cout << "No Corners found!" << std::endl;
            }
            else {
                show_obj = true;
                construct_object(rvec, tvec, camera_matrix, distortion_coefficients, vertices, faces, frame);
            }
        }
        if (key == 's') {
            cv::imwrite("../ar_captured_" + std::to_string(count++) + ".jpg", frame);
        }
        cv::imshow("Video", frame);
    }
    delete capdev;
    return (0);
}
#endif