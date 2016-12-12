#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



using namespace std;
using namespace cv;


class Mydat
{
public:
	cv::Rect points2Rect(double points[8]);
	void tracker_dat_init(cv::Mat &image, cv::Rect rect, MyState &state,cv::Rect &location);
	void tracker_dat_update(MyState &state, cv::Mat &image, MyState &new_state, cv::Rect &new_location);
	

private:
	/*default params*/

	//image scaling
	int img_scale_target_diagonal = 75;//length of target diagonal,used to resize image
	//search regions and surrounding regions
	int search_window_padding = 2;
	float surr_window_factor = 1.9;
	//appearance model
	int color_space = 1;//rgb,hsv,or lab
	int num_bins = 16;//number of bins per channel
	int bin_mapping_value;//0-255
	double prob_lut_update_rate = 0.05;
	bool distractor_aware = true;//toggle distractor awareness
	double adapt_thresh_prob_bins; //bins for adaptive threshold
	//motion estimation
	int motion_estimation_history_size = 5;//if set to 0,means disable motion estimation
	//NMS-based localization
	int nms_scale = 1;
	double nms_overlap = 0.9;
	double nms_score_factor = 0.5;

	double myMax(double a, double b);
	double myMin(double a, double b);
	void pos2rect(cv::Point target_pos, cv::Size surr_size, cv::Size win_size, cv::Rect &rect);
	cv::Mat getSubwindow(cv::Mat &image, cv::Point pos, cv::Size sz);
	void getForeBackprobs(cv::Mat &img, cv::Rect target_rect_surr, int num_bins, int bin_mapping_value, vector<vector<vector <double>>> &prob_lut, cv::Mat &prob_map);


};

struct MyState
{
	//bool show_images;
	//bool report_points;
	double scale_factor;
	vector<vector<vector <double>>> prob_lut;
	vector<vector<vector <double>>> prob_lut_distractor;
	vector<vector<vector <double>>> prob_lut_masked;
	double adapt_thresh;
	vector<double> target_pos_history;
	vector<double> target_size_history;

};