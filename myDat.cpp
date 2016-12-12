#include "myDat.h"



double Mydat::myMax(double a, double b){
	return ((a + b) + (a - b)) / 2;
}

double Mydat::myMin(double a, double b){
	return ((a + b) - (a - b)) / 2;
}
//convert points to rect
cv::Rect Mydat::points2Rect(double points[8]){
	double center_x = (points[0] + points[2]) / 2;
	double center_y = (points[1] + points[5]) / 2;
	double min_x = myMin(points[0], points[2]);
	double max_x = myMax(points[0], points[2]);
	double min_y = myMin(points[1], points[5]);
	double max_y = myMax(points[1], points[5]);
	double A1 = sqrt(pow((points[0] - points[2]), 2) + pow((points[1] - points[3]), 2)) * sqrt(pow((points[4] - points[2]), 2) + pow((points[5] - points[3]), 2));
	double A2 = (max_x - min_x) * (max_y - min_y);
	double s = sqrt(A1 / A2);
	double width  = s * (max_x - min_x) + 1;
	double height = s * (max_y - min_y) + 1;
	return (cv::Rect(round(center_x - width / 2), round(center_y - height / 2), round(width), round(height)));

}

//init for the first frame
void Mydat::tracker_dat_init(cv::Mat &image, cv::Rect rect, MyState &state, cv::Rect &location){
	double center_x = rect.x + (rect.width  - 1) / 2;
	double center_y = rect.y + (rect.height - 1) / 2;
	cv::Point target_pos = cv::Point(round(center_x), round(center_y));
	cv::Size target_size = cv::Size(rect.width, rect.height);

	//scale data
	state.scale_factor = myMin(1.0, (double)round(10 * img_scale_target_diagonal / sqrt(target_size.width * target_size.width + target_size.height *target_size.height)) / 10);
	target_pos  = state.scale_factor * target_pos;
	target_size.width  = state.scale_factor * target_size.width;
	target_size.height = state.scale_factor * target_size.height;

	//resize or perprocess the input image
	cv::Mat img_double;
	image.convertTo(img_double,CV_64F);
	cv::Mat img_scale;
	resize(img_double, img_scale, cv::Size(img_double.cols * state.scale_factor, img_double.rows * state.scale_factor));
	switch (color_space)
	{
	case 1://rgb
		img_double = img_double * 255.0;
		break;
	case 2://lab
		
		cvtColor(img_double, img_double,COLOR_RGB2Lab);
		break;
	case 3://hsv
		cvtColor(img_double, img_double, COLOR_RGB2HSV);
		img_double = img_double * 255.0;
		break;
	case 4://gray
		cvtColor(img_double, img_double, COLOR_RGB2GRAY);
		img_double = img_double * 255.0;
		break;
	default:
		break;
	}

	//target and surrouding
	cv::Size surr_size = cv::Size(floor(target_size.width * surr_window_factor), floor(target_size.height * surr_window_factor));
	//bacground rect
	cv::Rect surr_rect;
	pos2rect(target_pos, surr_size, cv::Size(img_double.cols, img_double.rows), surr_rect);
	//foreground rect
	cv::Rect target_rect;
	pos2rect(target_pos, target_size, cv::Size(img_double.cols, img_double.rows), target_rect);
	cv::Rect target_rect_surr = cv::Rect(target_rect.x - surr_rect.x + 1, target_rect.y - surr_rect.y + 1, target_rect.width, target_rect.height);
	cv::Mat surr_win_img = cv::Mat(img_double, surr_rect);
	imshow("surr_img", surr_win_img);//test right




}

void Mydat::pos2rect(cv::Point target_pos, cv::Size surr_size, cv::Size win_size, cv::Rect &rect){
	rect = cv::Rect(round(target_pos.x - surr_size.width / 2), round(target_pos.y - surr_size.height / 2), surr_size.width, surr_size.height);
	if (rect.x < 1)
	{
		int corr = abs(rect.x) + 1;
		rect.x = 1;
		rect.width = rect.width - corr;
	}
	else if (rect.y < 1)
	{
		int corr = abs(rect.y) + 1;
		rect.y = 1;
		rect.height = rect.height - corr;
	}
	else if ((rect.x + rect.width) > win_size.width )
	{
		rect.width = win_size.width - rect.x;
	}
	else if ((rect.y + rect.height) > win_size.height)
	{
		rect.height = win_size.height - rect.y;
	}



}

//
cv::Mat Mydat::getSubwindow(cv::Mat &image, cv::Point pos, cv::Size sz){

}

//
void Mydat::getForeBackprobs(cv::Mat &img, cv::Rect target_rect_surr, int num_bins, int bin_mapping_value, vector<vector<vector <double>>> &prob_lut, cv::Mat &prob_map){
	int row = img.rows;
	int col = img.cols;
	int channel = img.channels;
	//size of target
	int target_row = round(target_rect_surr.y);
	int target_col = round(target_rect_surr.x);
	int target_width = round(target_rect_surr.width);
	int target_height = round(target_rect_surr.height);

	//limit the range
	if ((target_row + target_height) > row)
		target_height = row - target_row;
	if ((target_col + target_width) > col)
		target_width = col - target_col;
	prob_map = cv::Mat::zeros(row, col, CV_64F);

	if (3 == channel)
	{
		vector<cv::Mat> target_hist(num_bins), surr_hist(num_bins);
		cv::Mat hist_mat = cv::Mat::zeros(num_bins, num_bins, CV_64F);

		for (int i = 0; i < num_bins; i++)
		{
			target_hist.push_back(hist_mat);
			surr_hist.push_back(hist_mat);
		}



	}

}