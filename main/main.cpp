#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void calc_radius(vector<float> &radii, const vector<vector<Point>> &contours) {
    Point2f center;
    float radius;

    radii.clear();

    for (int i = 0; i < contours.size(); i++) {
        minEnclosingCircle(contours[i], center, radius);
        radii.push_back(radius);
    }
}

void avg_rad_disp(float &avg_rad, float &disp, const vector<float> &radii) {
    avg_rad = 0.f;
    for (int i = 0; i < radii.size(); i++) {
        avg_rad += radii[i];
    }
    avg_rad /= radii.size();
    
    disp = 0.f;
    for (int i = 0; i < radii.size(); i++) {
        disp += (avg_rad - radii[i]) * (avg_rad - radii[i]);
    }
    disp /= radii.size();
}

int main(int argc, char** argv) {
    String image_path = samples::findFile("resource/whiteballssample.jpg");
    Mat dst, src = imread(image_path, IMREAD_COLOR);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    namedWindow("Search for balls", WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
    resizeWindow("Search for balls", 1200, 1200 * src.rows / src.cols);

    // Бинаризация изображения
    cvtColor(src, dst, COLOR_BGR2GRAY);
    equalizeHist(dst, dst);
    threshold(dst, dst, 245, 255, THRESH_BINARY);
    // threshold(src, src, 140, 255, THRESH_BINARY);
    // threshold(src, src, 0, 255, THRESH_BINARY + THRESH_OTSU);

    // Удаление шума
    Mat kernel_nose = getStructuringElement(MORPH_CROSS, Size(3, 3));
    erode(dst, dst, kernel_nose, Point(-1, -1), 3);

    // Заливка "пустот" в шарах
    findContours(dst, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); i++) {
        if (hierarchy[i][3] >=0) {
            fillPoly(dst, contours[i], 255, 1);
        }
    }

    // "Разлипание" шаров
    Mat kernel_erode = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
    erode(dst, dst, kernel_erode, Point(-1, -1), 2);
    Mat kernel_dilate = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    dilate(dst, dst, kernel_dilate, Point(-1, -1), 2);

    // Поиск контуров шаров
    findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(src, contours, -1, (0, 0, 255), 3);

    // Вычисление среднего радиуса и дисперсии этой величины
    vector<float> radii;
    float avg_rad, disp;
    calc_radius(radii, contours);
    avg_rad_disp(avg_rad, disp, radii);
    
    // Вывод итоговых результатов
    string number_ball = "Number of balls : ";
    number_ball +=  to_string(contours.size());
    string rad = "Average radius: ";
    rad += to_string(avg_rad);
    string d = "Variance: ";
    d += to_string(disp);
    putText(src, number_ball, Point(100,100), 1, 6, (255,255,255), 5);
    putText(src, rad, Point(100,200), 1, 6, (255,255,255), 5);
    putText(src, d, Point(100,300), 1, 6, (255,255,255), 5);

    imshow("Search for balls", src);

    waitKey(0);
    return 0;
}