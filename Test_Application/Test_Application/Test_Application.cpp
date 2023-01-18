// Test_Application.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <math.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

//CLASSIFIERS
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

//CONSTANTS
const int kEyePercentWidth = 25;
const int kEyePercentHeight = 30;
const int kEyePercentTop = 25;
const int kEyePercentSide = 13;

//GLOBAL VARIABLES
char input = 's';

void scaleToFastSize(const cv::Mat& src, cv::Mat& dst) {
	cv::resize(src, dst, cv::Size(50, (((float)50) / src.cols) * src.rows));
}

void findEyeHeightAndDisplay(Mat& image,Mat& grey_Image, vector<Rect> faces, double xProportionLeft, double yProportionLeft, double xProportionRight, double yProportionRight, double eyeHeightProportionLeft, double eyeHeightProportionRight, int& LeftEyeHeight, int& RightEyeHeight)
{
	cv::rectangle(image, faces[0], Scalar(0, 255, 255));
	Mat faceROI = grey_Image(faces[0]);

	//Calculate the center of the eye according to the proportion
	Point eyeCenterLeft(0, 0);
	Point eyeCenterRight(0, 0);

	eyeCenterLeft.x = xProportionLeft * (double)faces[0].width;
	eyeCenterLeft.y = yProportionLeft * (double)faces[0].height;

	eyeCenterRight.x = xProportionRight * (double)faces[0].width;
	eyeCenterRight.y = yProportionRight * (double)faces[0].height;

	//circle(faceROI,eyeCenterLeft,5,Scalar(255, 255, 255), 2);
	//circle(faceROI,eyeCenterRight, 5, Scalar(255, 255, 255), 2);

	int eye_region_width = faces[0].width * (kEyePercentWidth / 100.0);
	//int eye_region_height = faces[0].width * (kEyePercentHeight / 100.0);
	//int eye_region_top = faces[0].height * (kEyePercentTop / 100.0);
	//cv::Rect leftEyeRegion(faces[0].width* (kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);
	//cv::Rect rightEyeRegion(faces[0].width - eye_region_width - faces[0].width * (kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);

	//rectangle(faceROI, leftEyeRegion, Scalar(255, 255, 255));
	//rectangle(faceROI, rightEyeRegion, Scalar(255, 255, 255));

	//Draw the rectangle over the eyes
	int boxHeightLeft = eyeHeightProportionLeft * (double)faces[0].height;
	int boxHeightRight = eyeHeightProportionRight * (double)faces[0].height;
	Rect leftEyeRegion(eyeCenterLeft.x - (eye_region_width / 2), eyeCenterLeft.y - (boxHeightLeft / 2), eye_region_width, boxHeightLeft);
	Rect rightEyeRegion(eyeCenterRight.x - (eye_region_width / 2), eyeCenterRight.y - (boxHeightRight / 2), eye_region_width, boxHeightRight);
	cv::rectangle(faceROI, leftEyeRegion, Scalar(255, 255, 255));
	cv::rectangle(faceROI, rightEyeRegion, Scalar(255, 255, 255));
	cv::imshow("Working", faceROI);
	waitKey(50);
	//Extract the eyes from the image and than use the red channel for tracking the eyes
	Mat eyeExtractedLeft = faceROI(leftEyeRegion);
	Mat eyeExtractedRight = faceROI(rightEyeRegion);

	//Invert the image to be able to get a better picture of the eye
	cv::GaussianBlur(eyeExtractedLeft, eyeExtractedLeft, cv::Size(5, 5), 0, 0);
	cv::GaussianBlur(eyeExtractedRight, eyeExtractedRight, cv::Size(5, 5), 0, 0);

	for (int y = 0; y < eyeExtractedLeft.rows; ++y) {
		unsigned char* row = eyeExtractedLeft.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedLeft.cols; ++x) {
			row[x] = (255 - row[x]);
		}
	}
	for (int y = 0; y < eyeExtractedRight.rows; ++y) {
		unsigned char* row = eyeExtractedRight.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedRight.cols; ++x) {
			row[x] = (255 - row[x]);
		}
	}

	//Take the exponential transform of the image
	for (int y = 0; y < eyeExtractedLeft.rows; ++y) {
		unsigned char* row = eyeExtractedLeft.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedLeft.cols; ++x) {
			uchar placeHolder = row[x] * ((double)log(255) / 255.0);
			row[x] = exp(placeHolder);
		}
	}

	for (int y = 0; y < eyeExtractedRight.rows; ++y) {
		unsigned char* row = eyeExtractedRight.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedRight.cols; ++x) {
			uchar placeHolder = row[x] * ((double)log(255) / 255.0);
			row[x] = exp(placeHolder);
		}
	}

	//Binarize the image
	//First calculate the average of the intensity
	//Second calculate the standard deviation of the pixels
	int amountOfPixelsinImageLeft;
	int amountOfPixelsinImageRight;
	amountOfPixelsinImageLeft = eyeExtractedLeft.cols * eyeExtractedLeft.rows;
	amountOfPixelsinImageRight = eyeExtractedRight.cols * eyeExtractedRight.rows;
	int sumLeft = 0;
	int sumRight = 0;
	double averageLeft;
	double averageRight;

	for (int y = 0; y < eyeExtractedLeft.rows; ++y) {
		unsigned char* row = eyeExtractedLeft.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedLeft.cols; ++x) {
			sumLeft += row[x];
		}
	}

	for (int y = 0; y < eyeExtractedRight.rows; ++y) {
		unsigned char* row = eyeExtractedRight.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedRight.cols; ++x) {
			sumRight += row[x];
		}
	}

	averageLeft = (double)sumLeft / (double)amountOfPixelsinImageLeft;
	averageRight = (double)sumRight / (double)amountOfPixelsinImageRight;

	//Find the standard deviation
	double sumStdLeft = 0.0;
	double sumStdRight = 0.0;

	for (int y = 0; y < eyeExtractedLeft.rows; ++y) {
		unsigned char* row = eyeExtractedLeft.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedLeft.cols; ++x) {
			sumStdLeft += pow((row[x] - averageLeft), 2);
		}
	}

	for (int y = 0; y < eyeExtractedRight.rows; ++y) {
		unsigned char* row = eyeExtractedRight.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedRight.cols; ++x) {
			sumStdRight += pow((row[x] - averageRight), 2);
		}
	}

	sumStdLeft = sumStdLeft / amountOfPixelsinImageLeft;
	sumStdRight = sumStdRight / amountOfPixelsinImageRight;

	sumStdLeft = sqrt(sumStdLeft);
	sumStdRight = sqrt(sumStdRight);

	for (int y = 0; y < eyeExtractedLeft.rows; ++y) {
		unsigned char* row = eyeExtractedLeft.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedLeft.cols; ++x) {
			if (row[x] > (averageLeft + (0.9 * sumStdLeft)))
			{
				row[x] = 255;
			}
			else
			{
				row[x] = 0;
			}

		}
	}

	for (int y = 0; y < eyeExtractedRight.rows; ++y) {
		unsigned char* row = eyeExtractedRight.ptr<unsigned char>(y);
		for (int x = 0; x < eyeExtractedRight.cols; ++x) {
			if (row[x] > (averageRight + (0.9 * sumStdRight)))
			{
				row[x] = 255;
			}
			else
			{
				row[x] = 0;
			}
		}
	}

	//Implement the blob detection
	//Make sure to choose the blob that is closer to the center of the screen
	SimpleBlobDetector::Params params;

	//Change the thresholds (make sure to review how this works)
	params.minThreshold = 0;
	params.maxThreshold = 255;

	//Extract Light Blobs
	params.blobColor = 255;

	//Filter by Area
	params.filterByArea = false;
	//Calculate the area of the incoming image
	int areaIncomingImageLeft;
	int areaIncomingImageRight;
	areaIncomingImageLeft = eyeExtractedLeft.cols * eyeExtractedLeft.rows;
	areaIncomingImageRight = eyeExtractedRight.cols * eyeExtractedRight.rows;
	//Choose the one that has the smallest area
	int areaChosen;
	if (areaIncomingImageLeft < areaIncomingImageRight)
	{
		areaChosen = areaIncomingImageLeft;
	}
	else
	{
		areaChosen = areaIncomingImageRight;
	}

	params.minArea = 0.01 * (double)areaChosen;

	//MIGHT HAVE TO PLAY AROUND WITH THESE TO GET THE ALGORITHM TO WORK PROPERLY
	//Filter by Circularity
	params.filterByCircularity = false;
	params.minCircularity = 0.1;

	//Filter by Convexity
	params.filterByConvexity = false;
	params.minConvexity = 0.87;

	//Filter by Intertia
	params.filterByInertia = false;
	params.minInertiaRatio = 0.01;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	//Utilize the blob detector on the image and choose the blob that is closest to the center

	//Detect blobs
	//Left Eye
	vector<KeyPoint> keypointsLeft;
	vector<KeyPoint> keypointsRight;
	detector->detect(eyeExtractedLeft, keypointsLeft);

	//Right Eye
	detector->detect(eyeExtractedRight, keypointsRight);

	//Extract the correct blob (the one closest to the center)
	Point centerofImageLeft;
	Point centerofImageRight;

	centerofImageLeft.x = eyeExtractedLeft.cols / 2;
	centerofImageLeft.y = eyeExtractedLeft.rows / 2;

	centerofImageRight.x = eyeExtractedRight.cols / 2;
	centerofImageRight.y = eyeExtractedRight.rows / 2;

	//Find the blob closest to the center
	double minimumDisplacementlLeft;
	int indexLeftEye;
	double minimumDisplacementlRight;
	int indexRightEye;
	if (keypointsLeft.size() > 0)
	{
		for (int i = 0; i < keypointsLeft.size(); i++)
		{
			int differencetInX;
			int differenceInY;

			differencetInX = keypointsLeft[i].pt.x - centerofImageLeft.x;
			differenceInY = keypointsLeft[i].pt.y - centerofImageLeft.y;

			double minDisplacementLeftPlaceHolder;

			if (i == 0)
			{
				minimumDisplacementlLeft = sqrt((pow(differencetInX, 2) + pow(differenceInY, 2)));
				indexLeftEye = 0;
			}
			else
			{
				minDisplacementLeftPlaceHolder = sqrt((pow(differencetInX, 2) + pow(differenceInY, 2)));
				if (minDisplacementLeftPlaceHolder < minimumDisplacementlLeft)
				{
					minimumDisplacementlLeft = minDisplacementLeftPlaceHolder;
					indexLeftEye = i;
				}
			}

		}


		//Left eye height
		int flag1Left = 1;
		int flag2Left = 1;
		int indexTopPartforLeftEye = 0;
		int yPointFromTopforLeftEye = 0;
		int indexBottomPartforLeftEye = 0;
		int yPointBottomforLeftEye = 0;
		while (flag1Left)
		{
			if (eyeExtractedLeft.at<uchar>(keypointsLeft[indexLeftEye].pt.y + indexTopPartforLeftEye, keypointsLeft[indexLeftEye].pt.x) == 255)
			{
				indexTopPartforLeftEye--;
			}
			else
			{
				flag1Left = 0;
				yPointFromTopforLeftEye = keypointsLeft[indexLeftEye].pt.y + indexTopPartforLeftEye;
			}
			//Error checking to make sure that it does not go out of bounds
			if ((int)keypointsLeft[indexLeftEye].pt.y + indexTopPartforLeftEye == 1)
			{
				flag1Left = 0;
			}
		}
		while (flag2Left)
		{
			if (eyeExtractedLeft.at<uchar>(keypointsLeft[indexLeftEye].pt.y + indexBottomPartforLeftEye, keypointsLeft[indexLeftEye].pt.x) == 255)
			{
				indexBottomPartforLeftEye++;
			}
			else
			{
				flag2Left = 0;
				yPointBottomforLeftEye = keypointsLeft[indexLeftEye].pt.y + indexBottomPartforLeftEye;
			}
			//Error checking to make sure that it does not go out of bounds
			if ((int)keypointsLeft[indexLeftEye].pt.y + indexBottomPartforLeftEye == eyeExtractedLeft.rows)
			{
				flag2Left = 0;
			}
		}
		//Find the difference between the two to get the height
		int eyeOpeningHeightLeft = yPointBottomforLeftEye - yPointFromTopforLeftEye;
		//std::cout << "Eye_Height_Left: " << eyeOpeningHeightLeft << endl;
		Point pointLeft(keypointsLeft[indexLeftEye].pt.x, keypointsLeft[indexLeftEye].pt.y);
		cv::circle(eyeExtractedLeft, pointLeft, 1, Scalar(0, 0, 0), 2);
		imshow("Left Eye",eyeExtractedLeft);
		LeftEyeHeight = eyeOpeningHeightLeft;
		
		
	}
	else
	{
		LeftEyeHeight = -1;
	}

	if (keypointsRight.size() > 0)
	{
		for (int i = 0; i < keypointsRight.size(); i++)
		{
			int differencetInX;
			int differenceInY;

			differencetInX = keypointsRight[i].pt.x - centerofImageRight.x;
			differenceInY = keypointsRight[i].pt.y - centerofImageRight.y;

			double minDisplacementRightPlaceHolder;

			if (i == 0)
			{
				minimumDisplacementlRight = sqrt((pow(differencetInX, 2) + pow(differenceInY, 2)));
				indexRightEye = 0;
			}
			else
			{
				minDisplacementRightPlaceHolder = sqrt((pow(differencetInX, 2) + pow(differenceInY, 2)));
				if (minDisplacementRightPlaceHolder < minimumDisplacementlRight)
				{
					minimumDisplacementlRight = minDisplacementRightPlaceHolder;
					indexRightEye = i;
				}
			}
		}



		//Right eye height
		int flag1Right = 1;
		int flag2Right = 1;
		int indexTopPartforRightEye = 0;
		int yPointFromTopforRightEye = 0;
		int indexBottomPartforRightEye = 0;
		int yPointBottomforRightEye = 0;
		while (flag1Right)
		{
			if (eyeExtractedRight.at<uchar>(keypointsRight[indexRightEye].pt.y + indexTopPartforRightEye, keypointsRight[indexRightEye].pt.x) == 255)
			{
				indexTopPartforRightEye--;
			}
			else
			{
				flag1Right = 0;
				yPointFromTopforRightEye = keypointsRight[indexRightEye].pt.y + indexTopPartforRightEye;
			}
			//Error checking to make sure that it does not go out of bounds
			if ((int)keypointsRight[indexRightEye].pt.y + indexTopPartforRightEye == 1)
			{
				flag1Right = 0;
			}
		}
		while (flag2Right)
		{
			if (eyeExtractedRight.at<uchar>(keypointsRight[indexRightEye].pt.y + indexBottomPartforRightEye, keypointsRight[indexRightEye].pt.x) == 255)
			{
				indexBottomPartforRightEye++;
			}
			else
			{
				flag2Right = 0;
				yPointBottomforRightEye = keypointsRight[indexRightEye].pt.y + indexBottomPartforRightEye;
			}
			//Error checking to make sure that it does not go out of bounds
			if ((int)keypointsRight[indexRightEye].pt.y + indexBottomPartforRightEye == eyeExtractedRight.rows)
			{
				flag2Right = 0;
			}
		}
		//Find the difference between the two to get the height
		int eyeOpeningHeightRight = yPointBottomforRightEye - yPointFromTopforRightEye;
		//std::cout << "Eye_Height_Right: " << eyeOpeningHeightRight << endl;
		Point pointRight(keypointsRight[indexRightEye].pt.x, keypointsRight[indexRightEye].pt.y);
		cv::circle(eyeExtractedRight, pointRight, 1, Scalar(0, 0, 0), 2);
		imshow("Right Eye", eyeExtractedRight);
		RightEyeHeight = eyeOpeningHeightRight;
		return;
		

	}
	else
	{
		RightEyeHeight = -1;
		return;
	}
	
}


int find_Radius(Point center, Mat& eye)
{
	int  N = 100;
	double h = (2 * 3.14159265359) / double(N);
	int max_y = 0;
	int max_x = 0;
	int radius_max = 0;
	max_y = eye.rows - center.y;
	max_x = eye.cols - center.x;

	if (center.x <= center.y && center.x <= max_y && center.x <= max_x)
	{
		radius_max = center.x;
	}
	else if (center.y <= center.x && center.y <= max_y && center.y <= max_x)
	{
		radius_max = center.y;
	}
	else if (max_x <= center.y && max_x <= max_y && max_x <= center.x)
	{
		radius_max = max_x;
	}
	else if (max_y <= center.y && max_y <= max_x && max_y <= center.x)
	{
		radius_max = max_y;
	}

	//Store all of the radii in a vector class
	vector<int> f_radii;
	for (int r_candidate = 1; r_candidate < radius_max; r_candidate++)
	{
		//Find the integral using the trapezoidal rule
		//Make sure to implement the algorithm that checks the previous value
		double sum = 0;

		for (int i = 0; i <= N; i++)
		{
			int x_value = center.x + r_candidate * cos(0 + h * i);
			int y_value = center.y + r_candidate * sin(0 + h * i);
			int x_value_previous = 0;
			int y_value_previous = 0;
			//Calculate the intensity value of the matrix
			uchar* Ir = eye.ptr<uchar>(y_value);
			uchar I = Ir[x_value]; //MAKE SURE TO CHECK THIS OVER TO SEE IF IT IS THE RIGHT WAY TO INDEX VALUES AND THAT IT IS RETURNING THE PROPER VALUES
			if (i != 0 && (x_value != x_value_previous && y_value != y_value_previous))
			{
				if (i == 0 || i == N)
				{
					sum += I;
				}
				else
				{
					sum += 2 * I;
				}
			}
			else if (i == 0)
			{
				sum += I;
			}
			x_value_previous = x_value;
			y_value_previous = y_value;
		}
		double integ = 0.5 * sum * h;
		//Remember the function should go from r_candidate to radius_max - 1
		f_radii.push_back(integ);
	}

	int number = 0;
	//Take the derivative numerically and store the values
	if (f_radii.size() == 1)
	{

		return 1;
	}

	if (f_radii.size() == 0)
	{

		return 0;
	}

	vector<double> f_derivative;
	for (int j = 0; j < f_radii.size(); ++j)
	{
		double derivative = 0;
		if (j == 0)
		{
			//This is where the issue is
			derivative = (f_radii[j + 1] - f_radii[0]);
			derivative = derivative / (2 * h);
		}
		else if (j == f_radii.size() - 1) //MADE CHANGES FROM 2 TO 1
		{
			derivative = (f_radii[j] - f_radii[j - 1]);
			derivative = derivative / (2 * h);
		}
		else
		{
			derivative = (f_radii[j + 1] - f_radii[j - 1]);
			derivative = derivative / (2 * h);
		}
		f_derivative.push_back(derivative);
	}

	double max = f_derivative[0];
	int final_radius = 0;
	//Find the maximum
	for (int z = 0; z < f_derivative.size(); ++z)
	{
		if (f_derivative[z] > max)
		{
			max = f_derivative[z];
			final_radius = z;
		}
	}
	return final_radius;
}

int eye_opening_height(Mat image, int radius, Point center, Point& start, Point& end)
{
	//WE ARE PERFORMING THE ALGORITHM ON THE RED CHANNEL
	int diameter = 2 * radius;
	vector<double> h_y;

	for (int y = center.y - radius; y < center.y + radius; y++)
	{
		Vec3b* Pts = image.ptr<Vec3b>(y);
		double theta1 = 0;
		double theta2 = 0;
		int x1 = 0;
		int x2 = 0;
		if (y < center.y)
		{
			if (y == center.y - radius)
			{
				theta1 = -3.14159265359 / 2;
				//Make sure that this is computed properly
				x1 = center.x + radius * cos(theta1);
				h_y.push_back(Pts[x1][2]);
			}
			else
			{
				//Make sure to check to see if this is calculated properly
				theta1 = (y - center.y) / radius;
				theta1 = asin(theta1);
				theta2 = -1 * (3.14159265359 + theta1);
				//This is the left value
				x1 = center.x + radius * cos(theta2);
				//This is the right value
				x2 = center.x + radius * cos(theta1);
				int counter = x1;
				double sum = 0;
				while (counter <= x2)
				{
					sum = sum + Pts[counter][2];
					counter++;
				}
				sum = sum / (x2 - x1);
				h_y.push_back(sum);
			}
		}
		else if (y > center.y)
		{
			if (center.y + radius)
			{
				theta1 = 3.14159265359 / 2;
				x1 = center.x + radius * cos(theta1);
				h_y.push_back(Pts[x1][2]);
			}
			else
			{
				//Make sure to check to see if this is calculated properly
				theta1 = (y - center.y) / radius;
				theta1 = asin(theta1);
				//Make sure to check to see if this is correct
				theta2 = 3.14159265359 - theta1;
				x1 = center.x + radius * cos(theta2);
				x2 = center.x + radius * cos(theta1);
				int counter = x1;
				double sum = 0;
				while (counter <= x2)
				{
					sum = sum + Pts[counter][2];
					counter++;
				}
				sum = sum / (x2 - x1);
				h_y.push_back(sum);
			}
		}
		else
		{
			x1 = center.x - radius;
			x2 = center.x + radius;
			int counter = x1;
			double sum = 0;
			while (counter <= x2)
			{
				sum = sum + Pts[counter][2];
				counter++;
			}
			sum = sum / (x2 - x1);
			h_y.push_back(sum);
		}
	}




	//Find the lowest values
	//Algorithm:
	//First find the max value
	//Then take 80 percent of that
	//Set that to the upper threshold
	//Take the values that are less than that and store them
	int test = 0;
	double max = h_y[0];
	for (int i = 0; i < h_y.size(); i++)
	{
		if (h_y[i] > max)
		{
			max = h_y[i];
		}
	}
	double percentage = 0.8;
	double max_percentage = percentage * max;
	vector<double> pile;

	for (int j = 0; j < h_y.size(); j++)
	{
		if (h_y[j] < max_percentage)
		{
			pile.push_back(j);
		}
	}
	int iris_height;
	if (pile.size() == 0)
	{
		iris_height = 0;
	}
	else
	{
		iris_height = pile[pile.size() - 1] - pile[0];
	}
	start.x = center.x;
	if (pile.size() == 0)
	{
		start.y = center.y;
	}
	else
	{
		start.y = pile[0] + (center.y - radius);
	}

	end.x = center.x;
	if (pile.size() == 0)
	{
		end.y = center.y;
	}
	else
	{
		end.y = pile[pile.size() - 1] + (center.y - radius);
	}

	return iris_height;
}




Mat Compute_Gradient_X(const Mat& mat) //Done
{
	cv::Mat out(mat.rows, mat.cols, CV_64F);

	for (int y = 0; y < mat.rows; ++y) {
		const uchar* Mr = mat.ptr<uchar>(y);
		double* Or = out.ptr<double>(y);

		Or[0] = Mr[1] - Mr[0];
		for (int x = 1; x < mat.cols - 1; ++x) {
			Or[x] = (Mr[x + 1] - Mr[x - 1]) / 2.0;
		}
		Or[mat.cols - 1] = Mr[mat.cols - 1] - Mr[mat.cols - 2];
	}

	return out;
}

//Make sure to implement the weighting for the image based on the research article
Point objective_function(const cv::Mat& image)
{
	Mat gradientMatrixX = Compute_Gradient_X(image);
	Mat gradientMatrixY = Compute_Gradient_X(image.t()).t();

	//create a matrix that will store all of the sum values
	Mat outSum = Mat::zeros(image.rows, image.cols, CV_64F);
	double max = 0;
	Point max_Point(0, 0);

	//THIS IS THE SLOWEST POINT IN THE PROGRAM
	for (int y = 0; y < image.rows; ++y)
	{
		const double* Xr = gradientMatrixX.ptr<double>(y);
		const double* Yr = gradientMatrixY.ptr<double>(y);
		for (int x = 0; x < image.cols; ++x)
		{
			double gX = Xr[x];
			double gY = Yr[x];
			if (gX == 0.0 && gY == 0.0)
			{
				continue;
			}


			for (int cy = 0; cy < image.rows; ++cy)
			{
				double* Or = outSum.ptr<double>(cy);

				for (int cx = 0; cx < image.cols; ++cx)
				{
					if (x == cx && y == cy)
					{
						continue;
					}
					double dx = x - cx;
					double dy = y - cy;

					double magnitude = sqrt((dx * dx) + (dy * dy));
					dx = dx / magnitude;
					dy = dy / magnitude;
					double dotProduct = dx * gX + dy * gY;

					dotProduct = std::max(0.0, dotProduct);
					Or[cx] += dotProduct * dotProduct;
				}
			}

		}

	}
	//THIS IS THE SLOWEST POINT IN THE PROGRAM
	//Search in the sum matrix for the maximum value of C
	for (int y = 0; y < image.rows; y++)
	{
		double* oS = outSum.ptr<double>(y);
		for (int x = 0; x < image.cols; x++)
		{
			if (oS[x] > max)
			{
				max = oS[x];
				max_Point.x = x;
				max_Point.y = y;
			}

		}
	}

	return max_Point;
}

void mouseEventFunction(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		input = 'g';
		std::cout << "Left click has been made" << input << endl;
	}
}

void mouseEventFunctionCalibrationLeftWink(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		input = 'r';
		std::cout << "Left click has been made" << input << endl;
	}
}

void mouseEventFunctionCalibrationRightWink(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		input = 'l';
		std::cout << "Left click has been made" << input << endl;
	}
}


int main()
{
    //MAKE SURE TO ORGANIZE THESE VARIABLES BETTER
	int eyeHeightWinkRight = 1000;
	int eyeHeightWinkLeft = 1000;
    Mat image;
    vector<Rect> faces;
    VideoCapture cap(0);
	double xProportionLeft;
	double yProportionLeft;
	double xProportionRight;
	double yProportionRight;
	double eyeHeightProportionLeft;
	double eyeHeightProportionRight;
	Mat grey_Image;

	//Set the window
	namedWindow("Calibration");
	setMouseCallback("Calibration", mouseEventFunction, NULL);
	namedWindow("CalibrationLeftWink");
	setMouseCallback("CalibrationLeftWink", mouseEventFunctionCalibrationLeftWink, NULL);
	namedWindow("CalibrationRightWink");
	setMouseCallback("CalibrationRightWink", mouseEventFunctionCalibrationRightWink, NULL);

	if (!face_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"))
	{
		std::cout << "Could not load the cascade" << endl;
		return 0;
	}
	if (!eyes_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"))
	{
		std::cout << "Could not load the cascade" << endl;
		return 0;
	}


	if (!cap.isOpened())
	{
		std::cout << "cannot open camera";
		return 0;

	}

	Recalibrate:
	std::cout << "Adjust your face so that it is on the center of the screen then click on the window." << endl;
	while (input == 's')
	{
		cap >> image;
		waitKey(10);
		cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
		face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			cv::rectangle(image, faces[0], Scalar(0, 255, 255));
		}
		cv::imshow("Calibration", image);
	}

	if (faces.empty() == false && input == 'g')
	{
		//Find the portion of the face that contains the eyes
		int eye_region_width = faces[0].width * (kEyePercentWidth / 100.0);
		int eye_region_height = faces[0].width * (kEyePercentHeight / 100.0);
		int eye_region_top = faces[0].height * (kEyePercentTop / 100.0);
		cv::Rect leftEyeRegion(faces[0].width * (kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);
		cv::Rect rightEyeRegion(faces[0].width - eye_region_width - faces[0].width * (kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);


		Point topLeftCornerLeftEye(leftEyeRegion.x + faces[0].x, leftEyeRegion.y + faces[0].y);
		Point bottomRightCornerLeftEye(leftEyeRegion.x + leftEyeRegion.width + faces[0].x, leftEyeRegion.y + faces[0].y + leftEyeRegion.height);

		Point topLeftCornerRightEye(rightEyeRegion.x + faces[0].x, rightEyeRegion.y + faces[0].y);
		Point bottomRightCornerRightEye(rightEyeRegion.x + rightEyeRegion.width + faces[0].x, rightEyeRegion.y + faces[0].y + rightEyeRegion.height);


		cv::rectangle(image, topLeftCornerLeftEye, bottomRightCornerLeftEye, Scalar(255, 255, 255));
		cv::rectangle(image, topLeftCornerRightEye, bottomRightCornerRightEye, Scalar(255, 255, 255));


		//Find the center of the eyes and then use that to be able to get a better approximation of where the eyes are located
		Mat faceROI = grey_Image(faces[0]);
		Mat faceROI_Colour = image(faces[0]);


		Mat eye1 = faceROI(leftEyeRegion);
		Mat eye2 = faceROI(rightEyeRegion);
		Mat eye_colour1 = faceROI_Colour(leftEyeRegion);
		Mat eye_colour2 = faceROI_Colour(rightEyeRegion);


		//Scale the image to a faster size
		Mat eye_ROI1;
		Mat eye_ROI2;
		//scaleToFastSize(eye1, eye_ROI1);
		//scaleToFastSize(eye2, eye_ROI2);

		Point eye_c1 = objective_function(eye1);
		Point eye_c2 = objective_function(eye2);

		//imshow("faces", faceROI);

		//Calculate the eye height and radius
		//Use that to calibrate where the eyes are


		int radius1 = find_Radius(eye_c1, eye1);
		int radius2 = find_Radius(eye_c2, eye2);
		Point start1(0, 0);
		Point end1(0, 0);
		Point start2(0, 0);
		Point end2(0, 0);

		int iris_height1 = eye_opening_height(eye_colour1, radius1, eye_c1, start1, end1);
		int iris_height2 = eye_opening_height(eye_colour2, radius2, eye_c2, start2, end2);

		//Calculate the proportion of the eye center with respect to the greater image


		//Multiply this by the actual height to find the approxoimate center of the eye
		//This has to be adjusted for the size of the faceROI
		xProportionLeft = ((double)eye_c1.x + (double)leftEyeRegion.x) / (double)faceROI.cols;
		yProportionLeft = ((double)eye_c1.y + (double)leftEyeRegion.y) / (double)faceROI.rows;

		xProportionRight = ((double)eye_c2.x + (double)rightEyeRegion.x) / (double)faceROI.cols;
		yProportionRight = ((double)eye_c2.y + (double)rightEyeRegion.y) / (double)faceROI.rows;


		//Find the porportion of the eye height with respect to the rest of the image
		int eyeHeightLeft = (radius1 * 2) + 20;
		int eyeHeightRight = (radius2 * 2) + 20;

		//Find the proportion of the eye height with respect to the rest of the face
		eyeHeightProportionLeft = (double)eyeHeightLeft / (double)faceROI.rows;
		eyeHeightProportionRight = (double)eyeHeightRight / (double)faceROI.cols;
			

		//Draw all of the features on the face
		cv::circle(eye_colour1, eye_c1, radius1, Scalar(0, 0, 255), 1);
		cv::circle(eye_colour2, eye_c2, radius2, Scalar(0, 0, 255), 1);
		//Draw the eye_height
		line(eye_colour1, start1, end1, Scalar(0, 255, 0), 2, 8);
		line(eye_colour2, start2, end2, Scalar(0, 255, 0), 2, 8);

		cv::imshow("debugWindow1", eye_colour1);
		cv::imshow("debugWindow2", eye_colour2);
		waitKey(10);
		std::cout << "Does this look like an accurate picture of the eyes. g for yes and s for no" << endl;
		cin >> input;
		if (input == 's')
		{
			input = 's';
			goto Recalibrate;
		}
	}

	//Initiate a calibration process
Recalibrate2:
	input = 'r';
	while (input == 'r')
	{
		cap >> image;
		waitKey(10);
		cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
		face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			cv::rectangle(image, faces[0], Scalar(0, 255, 255));
			int dumby = 0;
			findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight,dumby, eyeHeightWinkRight);
			cout << "Eye Height For Winking Right: " << eyeHeightWinkRight << endl;
		}
		cv::imshow("CalibrationRightWink", image);
	}
	while (input == 'l')
	{
		cap >> image;
		waitKey(10);
		cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
		face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			cv::rectangle(image, faces[0], Scalar(0, 255, 255));
			int dumby = 0;
			findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, eyeHeightWinkLeft, dumby);
			cout << "Eye Height For Winking Left: " << eyeHeightWinkLeft << endl;
		}
		cv::imshow("CalibrationLeftWink", image);
	}
	
	int flagWinkDetection = false;
	while (true)
	{
		//Draw the region of the eyes on the image and then extract them from the image to do the colour channel selection and exponential transform
		cap >> image;
		int eyeHeightLeft;
		int eyeHeightRight;

		waitKey(10);

		//Split the image into the channels and take only the red channel for processing
		std::vector<cv::Mat> rgbChannels(3);
		cv::split(image, rgbChannels);
		cv::Mat grey_Image = rgbChannels[2];


		face_cascade.detectMultiScale(grey_Image, faces, 1.2, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
		
			findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, eyeHeightLeft, eyeHeightRight);
			if (flagWinkDetection == false)
			{
				if (eyeHeightLeft == -1 || eyeHeightRight == -1)
				{
					//cout << "Error" << endl;
				}
				else if (eyeHeightLeft <= eyeHeightWinkLeft && eyeHeightRight > eyeHeightWinkRight)
				{
					cout << "wink" << endl;
					cout << "Left Eye Height: " << eyeHeightLeft << endl;
					cout << "Right Eye Height: " << eyeHeightRight << endl;
					flagWinkDetection = true;
					
				
				}
				else if (eyeHeightLeft > eyeHeightWinkLeft && eyeHeightRight <= eyeHeightWinkRight)
				{
					cout << "wink" << endl;
					cout << "Left Eye Height: " << eyeHeightLeft << endl;
					cout << "Right Eye Height: " << eyeHeightRight << endl;
					flagWinkDetection = true;
				}
				else
				{
					
				}
			}
			else
			{
				if (eyeHeightLeft > eyeHeightWinkLeft && eyeHeightRight > eyeHeightWinkRight)
				{
					flagWinkDetection = false;
				}
			}
			
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


/*std::cout << "Wink one eye. Press r for right and l for left." << endl;
	cin >> input;
	//Capture the correct eye that the person wants to wink with and use that in the calibration to determine if the eye is closed
	if (input == 'r')
	{
		//Draw the region of the eyes on the image and then extract them from the image to do the colour channel selection and exponential transform
		cap >> image;
		waitKey(10);

		//Split the image into the channels and take only the red channel for processing
		std::vector<cv::Mat> rgbChannels(3);
		cv::split(image, rgbChannels);
		cv::Mat grey_Image = rgbChannels[2];

		face_cascade.detectMultiScale(grey_Image, faces, 1.2, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			eyeHeightWinkRight = findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, input);
			if(eyeHeightWinkRight == -1)
			{
				goto Recalibrate2;
			}
		}
		else
		{
			goto Recalibrate2;
		}
		std::cout << "Wink the other eye. Hit any button to continue..." << endl;
		waitKey(0);

		//Draw the region of the eyes on the image and then extract them from the image to do the colour channel selection and exponential transform
		cap >> image;
		waitKey(10);

		//Split the image into the channels and take only the red channel for processing
		cv::split(image, rgbChannels);
		grey_Image = rgbChannels[2];

		face_cascade.detectMultiScale(grey_Image, faces, 1.2, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			eyeHeightWinkLeft = findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, 'l');
			if (eyeHeightWinkLeft == -1)
			{
				goto Recalibrate2;
			}
		}
		else
		{
			goto Recalibrate2;
		}
	}
	else if (input == 'l')
	{
		cap >> image;
		waitKey(10);

		//Split the image into the channels and take only the red channel for processing
		std::vector<cv::Mat> rgbChannels(3);
		cv::split(image, rgbChannels);
		cv::Mat grey_Image = rgbChannels[2];

		face_cascade.detectMultiScale(grey_Image, faces, 1.2, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			eyeHeightWinkLeft = findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, input);
			if (eyeHeightWinkLeft == -1)
			{
				goto Recalibrate2;
			}
		}
		std::cout << "Wink the other eye. Hit any button to continue..." << endl;
		waitKey(0);

		//Draw the region of the eyes on the image and then extract them from the image to do the colour channel selection and exponential transform
		cap >> image;
		waitKey(10);

		//Split the image into the channels and take only the red channel for processing
		cv::split(image, rgbChannels);
		grey_Image = rgbChannels[2];

		face_cascade.detectMultiScale(grey_Image, faces, 1.2, 3, 0, Size(250, 250));
		if (faces.empty() == false)
		{
			eyeHeightWinkRight = findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, 'r');
			if (eyeHeightWinkRight == -1)
			{
				goto Recalibrate2;
			}
		}
		else
		{
			goto Recalibrate2;
		}
	}
	else
	{
		goto Recalibrate2;
	}*/
