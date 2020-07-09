#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

int flag = 0;

void display(cv::Mat image){
    std::string windowName = "windowName";
    cv::namedWindow(windowName);
    cv::imshow(windowName, image);
    std::string str =  std::to_string(flag);
    cv::imwrite("progress_0709/"+str+".jpg", image);
    flag += 1;
    cv::waitKey(0);
    cv::destroyWindow(windowName);
}

void Keypoint(unsigned int id, double x, double y, double size) {
    // // sato
    // cout << "Keypoint2 " << flush;
    // //
}

int main(int argc, char**argv){

    // 450×450の画像　CV_8UC3(8bit 3チャンネル)
    cv::Mat image = cv::Mat::zeros(450, 450, CV_8UC3);

    int cols = image.cols;
    int rows = image.rows;

    for(int j = 0; j < rows; j++){
        for(int i = 0; i < cols; i++){
            image.at<cv::Vec3b>(j, i)[0] = 0;  //青
            image.at<cv::Vec3b>(j, i)[1] = 0;  //緑
            image.at<cv::Vec3b>(j, i)[2] = 0;  //赤
        }
    }

    display(image);

    // 適当に島を作成(@todo:指定、C++の文法だるすぎて断念)

    // 線分
    cv::line(image, cv::Point(20,20), cv::Point(200,20), cv::Scalar(255,255,255), 1, 4);  // 太さ1, 4近傍連結
    cv::line(image, cv::Point(20,50), cv::Point(200,50), cv::Scalar(255,255,255), 5, 8);  // 太さ5, 8近傍連結
    cv::line(image, cv::Point(20,80), cv::Point(200,80), cv::Scalar(255,255,255), 20, CV_AA);  // 太さ20, アンチエイリアス

    // 矩形
    cv::rectangle(image, cv::Point(300,20), cv::Point(400,50), cv::Scalar(255,255,255), -1, 4); //太さ-1(=塗りつぶし)

    // 円
    cv::circle(image, cv::Point(350,80), 20, cv::Scalar(255,255,255), -1, 4);   //半径50

    // 楕円・円弧
    /// 画像，中心座標，（長径・短径），回転角度，円弧開始角度，円弧終了角度，色，線太さ，連結
    double angle = 0;
    cv::ellipse(image, cv::Point(100, 150), cv::Size(50, 25), angle+50, angle, angle+360, cv::Scalar(255,255,255), -1, 8);
    angle = 100;
    cv::ellipse(image, cv::Point(100, 230), cv::Size(25, 50), angle, angle-200, angle+100, cv::Scalar(255,255,255), -1, CV_AA);
    
    // // ポリゴン
    // cv::RNG gen(0xffffffff);
  
    // int npt[] = {4, 4};
    // cv::Point pt1[2][4];
    // const cv::Point *ppt[2] = {pt1[0], pt1[1]};

    // // Red
    // for(int i=0; i<2; i++)
    //     for(int j=0; j<4; j++)
    //         pt1[i][j] = cv::Point(gen.uniform(0, rows), gen.uniform(0, cols));
    // // 画像，折れ線頂点の配列の配列，頂点数の配列，折れ線数，色，種類
    // cv::fillPoly(image, ppt, npt, 2, cv::Scalar(0,0,200), 4);
    // // Green
    // for(int i=0; i<2; i++)
    //     for(int j=0; j<4; j++)
    //         pt1[i][j] = cv::Point(gen.uniform(0, rows), gen.uniform(0, cols));
    // cv::fillPoly(image, ppt, npt, 2, cv::Scalar(0,200,0), 8);
    // // Blue，
    // for(int i=0; i<2; i++)
    //     for(int j=0; j<4; j++)
    //         pt1[i][j] = cv::Point(gen.uniform(0, rows), gen.uniform(0, cols));
    // cv::fillPoly(image, ppt, npt, 2, cv::Scalar(200,0,0), CV_AA);

    // ランダム
    cv::line(image, cv::Point(200,200), cv::Point(380,320), cv::Scalar(255,255,255), 15, 4);  // 太さ1, 4近傍連結
    cv::line(image, cv::Point(250,430), cv::Point(400,350), cv::Scalar(255,255,255), 15, 8);  // 太さ5, 8近傍連結
    cv::line(image, cv::Point(440,250), cv::Point(445,400), cv::Scalar(255,255,255), 20, CV_AA);  // 太さ20, アンチエイリアス
    cv::rectangle(image, cv::Point(200,180), cv::Point(250,280), cv::Scalar(255,255,255), -1, 4); //太さ-1(=塗りつぶし)
    cv::circle(image, cv::Point(425,425), 30, cv::Scalar(255,255,255), -1, 4);   //半径50
    angle = 0;
    cv::ellipse(image, cv::Point(300, 380), cv::Size(70, 30), angle+100, angle, angle+360, cv::Scalar(255,255,255), -1, 8);
    angle = 100;
    cv::ellipse(image, cv::Point(150, 400), cv::Size(40, 100), angle, angle-200, angle, cv::Scalar(255,255,255), -1, CV_AA);

    display(image);


    // ---------------------------------------------------------------------------------------------
    
    // 輪郭取得→重心取得
    
    cv::Mat get_center_image = image.clone();    //重心を青い点で表示させるために、カラー画像をcloneさせておく

    // Convert to grayscale
    if (image.channels() == 1) {
        image = image;
    } else {
        cv::cvtColor(image, image, CV_BGR2GRAY);
    }

    // Extract contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int contour_index = -1;         //index = -1、level = 2にするとすべての輪郭表示
    int max_level = 2;
    std::cout << std::endl;
    std::cout << "num of contours = " << contours.size() << std::endl;
    std::cout << "hierarchy = " << hierarchy.size() << std::endl;
    cv::Mat contoursImage = image.clone();
    cvtColor(contoursImage, contoursImage, CV_GRAY2BGR);
    drawContours(contoursImage, contours, contour_index, cv::Scalar(0,0,255), -1, CV_AA, hierarchy, max_level);
    
    display(contoursImage);

    // keypoints.reserve(contours.size());
    unsigned int pointID = 0;
    for (unsigned int i = 0; i < contours.size(); i++) {
        cv::Moments moments = cv::moments(contours[i], false);
        double x = moments.m10 / moments.m00;
        double y = moments.m01 / moments.m00;
        cv::circle(get_center_image, cv::Point(x, y), 10, cv::Scalar(150, 0, 0), -1);
        
        // if (!std::isnan(x) && !std::isnan(y)) { // Function isnan() is overloaded, so std:: is important
        //     keypoints.push_back(Keypoint(pointID++, x, y, cv::contourArea(contours[i])));
		// 	// taniguchi
		// 	// cout << "[" << pointID << "] " << x << ", " << y << endl;
		// 	cv::circle(image, cv::Point(x, y), 10, cv::Scalar(150, 0, 0), -1);
		// 	// end
        // }
    }
    
    display(get_center_image);




}