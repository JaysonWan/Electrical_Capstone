#pragma once
//#include <string>
//#include "stdafx.h"
#include <windows.h>
//#include <conio.h>
//#include <math.h>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <list>

//CONSTANTS
const int kEyePercentWidth = 25;
const int kEyePercentHeight = 30;
const int kEyePercentTop = 25;
const int kEyePercentSide = 13;

//GLOBAL VARIABLES
char input = 's';
double xProportionLeft;
double yProportionLeft;
double xProportionRight;
double yProportionRight;
double eyeHeightProportionLeft;
double eyeHeightProportionRight;
int eyeHeightWinkRight = 1000;
int eyeHeightWinkLeft = 1000;


//Flags
int calibration_flag = 0;


namespace CppCLRWinFormsProject {

	

	//using namespace System;
	//using namespace System::ComponentModel;
	//using namespace System::Collections;
	//using namespace System::Windows::Forms;
	//using namespace System::Data;
	//using namespace System::Drawing;
	//using namespace cv;
	using namespace std;

	//List of Buttons
	std::list<int> list_of_Keyboard_Buttons_X;
	std::list<int> list_of_Keyboard_Buttons_Y;




	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;
	cv::VideoCapture cap(0);

	//Functions Required for Eye Tracking

	void scaleToFastSize(const cv::Mat& src, cv::Mat& dst) {
		cv::resize(src, dst, cv::Size(50, (((float)50) / src.cols) * src.rows));
	}

	void findEyeHeightAndDisplay(cv::Mat& image, cv::Mat& grey_Image, vector<cv::Rect> faces, double xProportionLeft, double yProportionLeft, double xProportionRight, double yProportionRight, double eyeHeightProportionLeft, double eyeHeightProportionRight, int& LeftEyeHeight, int& RightEyeHeight)
	{
		cv::rectangle(image, faces[0], cv::Scalar(0, 255, 255));
		cv::Mat faceROI = grey_Image(faces[0]);

		//Calculate the center of the eye according to the proportion
		cv::Point eyeCenterLeft(0, 0);
		cv::Point eyeCenterRight(0, 0);

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
		cv::Rect leftEyeRegion(eyeCenterLeft.x - (eye_region_width / 2), eyeCenterLeft.y - (boxHeightLeft / 2), eye_region_width, boxHeightLeft);
		cv::Rect rightEyeRegion(eyeCenterRight.x - (eye_region_width / 2), eyeCenterRight.y - (boxHeightRight / 2), eye_region_width, boxHeightRight);
		cv::rectangle(faceROI, leftEyeRegion, cv::Scalar(255, 255, 255));
		cv::rectangle(faceROI, rightEyeRegion, cv::Scalar(255, 255, 255));
		
		//cv::imshow("Working", faceROI);
		
		cv::waitKey(50);
		//Extract the eyes from the image and than use the red channel for tracking the eyes
		cv::Mat eyeExtractedLeft = faceROI(leftEyeRegion);
		cv::Mat eyeExtractedRight = faceROI(rightEyeRegion);

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
		cv::SimpleBlobDetector::Params params;

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

		cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

		//Utilize the blob detector on the image and choose the blob that is closest to the center

		//Detect blobs
		//Left Eye
		vector<cv::KeyPoint> keypointsLeft;
		vector<cv::KeyPoint> keypointsRight;
		detector->detect(eyeExtractedLeft, keypointsLeft);

		//Right Eye
		detector->detect(eyeExtractedRight, keypointsRight);

		//Extract the correct blob (the one closest to the center)
		cv::Point centerofImageLeft;
		cv::Point centerofImageRight;

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
			cv::Point pointLeft(keypointsLeft[indexLeftEye].pt.x, keypointsLeft[indexLeftEye].pt.y);
			cv::circle(eyeExtractedLeft, pointLeft, 1, cv::Scalar(0, 0, 0), 2);
			imshow("Left Eye", eyeExtractedLeft);
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
			cv::Point pointRight(keypointsRight[indexRightEye].pt.x, keypointsRight[indexRightEye].pt.y);
			cv::circle(eyeExtractedRight, pointRight, 1, cv::Scalar(0, 0, 0), 2);
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


	int find_Radius(cv::Point center, cv::Mat& eye)
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


	int eye_opening_height(cv::Mat image, int radius, cv::Point center, cv::Point& start, cv::Point& end)
	{
		//WE ARE PERFORMING THE ALGORITHM ON THE RED CHANNEL
		int diameter = 2 * radius;
		vector<double> h_y;

		for (int y = center.y - radius; y < center.y + radius; y++)
		{
			cv::Vec3b* Pts = image.ptr<cv::Vec3b>(y);
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

	cv::Mat Compute_Gradient_X(const cv::Mat& mat) //Done
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

	cv::Point objective_function(const cv::Mat& image)
	{
		cv::Mat gradientMatrixX = Compute_Gradient_X(image);
		cv::Mat gradientMatrixY = Compute_Gradient_X(image.t()).t();

		//create a matrix that will store all of the sum values
		cv::Mat outSum = cv::Mat::zeros(image.rows, image.cols, CV_64F);
		double max = 0;
		cv::Point max_Point(0, 0);

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
		if (event == cv::EVENT_LBUTTONDOWN)
		{
			input = 'g';
			//std::cout << "Left click has been made" << input << endl;
		}
	}

	void mouseEventFunctionCalibrationLeftWink(int event, int x, int y, int flags, void* userdata)
	{
		if (event == cv::EVENT_LBUTTONDOWN)
		{
			input = 'r';
			//std::cout << "Left click has been made" << input << endl;
		}
	}

	void mouseEventFunctionCalibrationRightWink(int event, int x, int y, int flags, void* userdata)
	{
		if (event == cv::EVENT_LBUTTONDOWN)
		{
			input = 'l';
			//std::cout << "Left click has been made" << input << endl;
		}
	}

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			timer1->Start();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ Q;
	private: System::Windows::Forms::Button^ W;
	private: System::Windows::Forms::Button^ E;

	protected:


	private: System::Windows::Forms::CheckBox^ Shift;
	private: System::Windows::Forms::Button^ Backspace;
	private: System::Windows::Forms::Button^ Space;
	private: System::Windows::Forms::Button^ Enter;
	private: System::Windows::Forms::TextBox^ textBox1;

	private: System::Windows::Forms::Label^ label1;

	private: System::Windows::Forms::CheckBox^ CAPS;

	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Timer^ timer2;

	private: System::Windows::Forms::Button^ R;
	private: System::Windows::Forms::Button^ T;
	private: System::Windows::Forms::Button^ Y;
	private: System::Windows::Forms::Button^ U;
	private: System::Windows::Forms::Button^ I;
	private: System::Windows::Forms::Button^ O;
	private: System::Windows::Forms::Button^ P;
	private: System::Windows::Forms::Button^ A;
	private: System::Windows::Forms::Button^ Z;
	private: System::Windows::Forms::Button^ X;


	private: System::Windows::Forms::Button^ S;
	private: System::Windows::Forms::Button^ D;
	private: System::Windows::Forms::Button^ C;

	private: System::Windows::Forms::Button^ F;
	private: System::Windows::Forms::Button^ V;


	private: System::Windows::Forms::Button^ G;
	private: System::Windows::Forms::Button^ B;


	private: System::Windows::Forms::Button^ H;
	private: System::Windows::Forms::Button^ N;


	private: System::Windows::Forms::Button^ J;
	private: System::Windows::Forms::Button^ M;


	private: System::Windows::Forms::Button^ K;
	private: System::Windows::Forms::Button^ L;
	private: System::Windows::Forms::Button^ comma;
	private: System::Windows::Forms::Button^ Period;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Q = (gcnew System::Windows::Forms::Button());
			this->W = (gcnew System::Windows::Forms::Button());
			this->E = (gcnew System::Windows::Forms::Button());
			this->Shift = (gcnew System::Windows::Forms::CheckBox());
			this->Backspace = (gcnew System::Windows::Forms::Button());
			this->Space = (gcnew System::Windows::Forms::Button());
			this->Enter = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->CAPS = (gcnew System::Windows::Forms::CheckBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->R = (gcnew System::Windows::Forms::Button());
			this->T = (gcnew System::Windows::Forms::Button());
			this->Y = (gcnew System::Windows::Forms::Button());
			this->U = (gcnew System::Windows::Forms::Button());
			this->I = (gcnew System::Windows::Forms::Button());
			this->O = (gcnew System::Windows::Forms::Button());
			this->P = (gcnew System::Windows::Forms::Button());
			this->A = (gcnew System::Windows::Forms::Button());
			this->Z = (gcnew System::Windows::Forms::Button());
			this->X = (gcnew System::Windows::Forms::Button());
			this->S = (gcnew System::Windows::Forms::Button());
			this->D = (gcnew System::Windows::Forms::Button());
			this->C = (gcnew System::Windows::Forms::Button());
			this->F = (gcnew System::Windows::Forms::Button());
			this->V = (gcnew System::Windows::Forms::Button());
			this->G = (gcnew System::Windows::Forms::Button());
			this->B = (gcnew System::Windows::Forms::Button());
			this->H = (gcnew System::Windows::Forms::Button());
			this->N = (gcnew System::Windows::Forms::Button());
			this->J = (gcnew System::Windows::Forms::Button());
			this->M = (gcnew System::Windows::Forms::Button());
			this->K = (gcnew System::Windows::Forms::Button());
			this->L = (gcnew System::Windows::Forms::Button());
			this->comma = (gcnew System::Windows::Forms::Button());
			this->Period = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// Q
			// 
			this->Q->Location = System::Drawing::Point(214, 258);
			this->Q->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Q->Name = L"Q";
			this->Q->Size = System::Drawing::Size(57, 63);
			this->Q->TabIndex = 0;
			this->Q->Text = L"Q";
			this->Q->UseVisualStyleBackColor = true;
			this->Q->Click += gcnew System::EventHandler(this, &Form1::Q_Click);
			// 
			// W
			// 
			this->W->Location = System::Drawing::Point(280, 258);
			this->W->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->W->Name = L"W";
			this->W->Size = System::Drawing::Size(57, 63);
			this->W->TabIndex = 1;
			this->W->Text = L"W";
			this->W->UseVisualStyleBackColor = true;
			this->W->Click += gcnew System::EventHandler(this, &Form1::W_Click);
			// 
			// E
			// 
			this->E->Location = System::Drawing::Point(345, 258);
			this->E->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->E->Name = L"E";
			this->E->Size = System::Drawing::Size(57, 63);
			this->E->TabIndex = 2;
			this->E->Text = L"E";
			this->E->UseVisualStyleBackColor = true;
			this->E->Click += gcnew System::EventHandler(this, &Form1::E_Click);
			// 
			// Shift
			// 
			this->Shift->Appearance = System::Windows::Forms::Appearance::Button;
			this->Shift->Location = System::Drawing::Point(15, 374);
			this->Shift->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Shift->Name = L"Shift";
			this->Shift->Size = System::Drawing::Size(189, 92);
			this->Shift->TabIndex = 31;
			this->Shift->TabStop = false;
			this->Shift->Text = L"Shift";
			this->Shift->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->Shift->UseVisualStyleBackColor = true;
			this->Shift->CheckedChanged += gcnew System::EventHandler(this, &Form1::Shift_CheckedChanged);
			// 
			// Backspace
			// 
			this->Backspace->Location = System::Drawing::Point(867, 258);
			this->Backspace->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Backspace->Name = L"Backspace";
			this->Backspace->Size = System::Drawing::Size(189, 63);
			this->Backspace->TabIndex = 10;
			this->Backspace->Text = L"Backsapce";
			this->Backspace->UseVisualStyleBackColor = true;
			this->Backspace->Click += gcnew System::EventHandler(this, &Form1::Backspace_Click);
			// 
			// Space
			// 
			this->Space->Location = System::Drawing::Point(214, 475);
			this->Space->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Space->Name = L"Space";
			this->Space->Size = System::Drawing::Size(842, 60);
			this->Space->TabIndex = 30;
			this->Space->Text = L"Space";
			this->Space->UseVisualStyleBackColor = true;
			this->Space->Click += gcnew System::EventHandler(this, &Form1::Space_Click);
			// 
			// Enter
			// 
			this->Enter->Location = System::Drawing::Point(802, 331);
			this->Enter->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Enter->Name = L"Enter";
			this->Enter->Size = System::Drawing::Size(255, 134);
			this->Enter->TabIndex = 29;
			this->Enter->Text = L"Enter";
			this->Enter->UseVisualStyleBackColor = true;
			this->Enter->Click += gcnew System::EventHandler(this, &Form1::Enter_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(210, 202);
			this->textBox1->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(847, 47);
			this->textBox1->TabIndex = 7;
			this->textBox1->TabStop = false;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(872, 209);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 20);
			this->label1->TabIndex = 9;
			// 
			// CAPS
			// 
			this->CAPS->Appearance = System::Windows::Forms::Appearance::Button;
			this->CAPS->Location = System::Drawing::Point(70, 302);
			this->CAPS->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->CAPS->Name = L"CAPS";
			this->CAPS->Size = System::Drawing::Size(135, 63);
			this->CAPS->TabIndex = 11;
			this->CAPS->TabStop = false;
			this->CAPS->Text = L"Cap Lock";
			this->CAPS->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->CAPS->UseVisualStyleBackColor = true;
			this->CAPS->CheckedChanged += gcnew System::EventHandler(this, &Form1::CAPS_CheckedChanged);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 50;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// timer2
			// 
			this->timer2->Interval = 50;
			this->timer2->Tick += gcnew System::EventHandler(this, &Form1::timer2_Tick);
			// 
			// R
			// 
			this->R->Location = System::Drawing::Point(411, 258);
			this->R->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->R->Name = L"R";
			this->R->Size = System::Drawing::Size(57, 63);
			this->R->TabIndex = 3;
			this->R->Text = L"R";
			this->R->UseVisualStyleBackColor = true;
			this->R->Click += gcnew System::EventHandler(this, &Form1::R_Click);
			// 
			// T
			// 
			this->T->Location = System::Drawing::Point(476, 258);
			this->T->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->T->Name = L"T";
			this->T->Size = System::Drawing::Size(57, 63);
			this->T->TabIndex = 4;
			this->T->Text = L"T";
			this->T->UseVisualStyleBackColor = true;
			this->T->Click += gcnew System::EventHandler(this, &Form1::T_Click);
			// 
			// Y
			// 
			this->Y->Location = System::Drawing::Point(542, 258);
			this->Y->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Y->Name = L"Y";
			this->Y->Size = System::Drawing::Size(57, 63);
			this->Y->TabIndex = 5;
			this->Y->Text = L"Y";
			this->Y->UseVisualStyleBackColor = true;
			this->Y->Click += gcnew System::EventHandler(this, &Form1::Y_Click);
			// 
			// U
			// 
			this->U->Location = System::Drawing::Point(606, 258);
			this->U->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->U->Name = L"U";
			this->U->Size = System::Drawing::Size(57, 63);
			this->U->TabIndex = 6;
			this->U->Text = L"U";
			this->U->UseVisualStyleBackColor = true;
			this->U->Click += gcnew System::EventHandler(this, &Form1::U_Click);
			// 
			// I
			// 
			this->I->Location = System::Drawing::Point(672, 258);
			this->I->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->I->Name = L"I";
			this->I->Size = System::Drawing::Size(57, 63);
			this->I->TabIndex = 7;
			this->I->Text = L"I";
			this->I->UseVisualStyleBackColor = true;
			this->I->Click += gcnew System::EventHandler(this, &Form1::I_Click);
			// 
			// O
			// 
			this->O->Location = System::Drawing::Point(736, 258);
			this->O->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->O->Name = L"O";
			this->O->Size = System::Drawing::Size(57, 63);
			this->O->TabIndex = 8;
			this->O->Text = L"O";
			this->O->UseVisualStyleBackColor = true;
			this->O->Click += gcnew System::EventHandler(this, &Form1::O_Click);
			// 
			// P
			// 
			this->P->Location = System::Drawing::Point(802, 258);
			this->P->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->P->Name = L"P";
			this->P->Size = System::Drawing::Size(57, 63);
			this->P->TabIndex = 9;
			this->P->Text = L"P";
			this->P->UseVisualStyleBackColor = true;
			this->P->Click += gcnew System::EventHandler(this, &Form1::P_Click);
			// 
			// A
			// 
			this->A->Location = System::Drawing::Point(214, 331);
			this->A->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->A->Name = L"A";
			this->A->Size = System::Drawing::Size(57, 63);
			this->A->TabIndex = 11;
			this->A->Text = L"A";
			this->A->UseVisualStyleBackColor = true;
			this->A->Click += gcnew System::EventHandler(this, &Form1::A_Click);
			// 
			// Z
			// 
			this->Z->Location = System::Drawing::Point(214, 403);
			this->Z->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Z->Name = L"Z";
			this->Z->Size = System::Drawing::Size(57, 63);
			this->Z->TabIndex = 20;
			this->Z->Text = L"Z";
			this->Z->UseVisualStyleBackColor = true;
			this->Z->Click += gcnew System::EventHandler(this, &Form1::Z_Click);
			// 
			// X
			// 
			this->X->Location = System::Drawing::Point(280, 403);
			this->X->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->X->Name = L"X";
			this->X->Size = System::Drawing::Size(57, 63);
			this->X->TabIndex = 21;
			this->X->Text = L"X";
			this->X->UseVisualStyleBackColor = true;
			this->X->Click += gcnew System::EventHandler(this, &Form1::X_Click);
			// 
			// S
			// 
			this->S->Location = System::Drawing::Point(280, 331);
			this->S->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->S->Name = L"S";
			this->S->Size = System::Drawing::Size(57, 63);
			this->S->TabIndex = 12;
			this->S->Text = L"S";
			this->S->UseVisualStyleBackColor = true;
			this->S->Click += gcnew System::EventHandler(this, &Form1::S_Click);
			// 
			// D
			// 
			this->D->Location = System::Drawing::Point(345, 331);
			this->D->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->D->Name = L"D";
			this->D->Size = System::Drawing::Size(57, 63);
			this->D->TabIndex = 13;
			this->D->Text = L"D";
			this->D->UseVisualStyleBackColor = true;
			this->D->Click += gcnew System::EventHandler(this, &Form1::D_Click);
			// 
			// C
			// 
			this->C->Location = System::Drawing::Point(345, 403);
			this->C->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->C->Name = L"C";
			this->C->Size = System::Drawing::Size(57, 63);
			this->C->TabIndex = 22;
			this->C->Text = L"C";
			this->C->UseVisualStyleBackColor = true;
			this->C->Click += gcnew System::EventHandler(this, &Form1::C_Click);
			// 
			// F
			// 
			this->F->Location = System::Drawing::Point(411, 331);
			this->F->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->F->Name = L"F";
			this->F->Size = System::Drawing::Size(57, 63);
			this->F->TabIndex = 14;
			this->F->Text = L"F";
			this->F->UseVisualStyleBackColor = true;
			this->F->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// V
			// 
			this->V->Location = System::Drawing::Point(411, 403);
			this->V->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->V->Name = L"V";
			this->V->Size = System::Drawing::Size(57, 63);
			this->V->TabIndex = 23;
			this->V->Text = L"V";
			this->V->UseVisualStyleBackColor = true;
			this->V->Click += gcnew System::EventHandler(this, &Form1::V_Click);
			// 
			// G
			// 
			this->G->Location = System::Drawing::Point(476, 331);
			this->G->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->G->Name = L"G";
			this->G->Size = System::Drawing::Size(57, 63);
			this->G->TabIndex = 15;
			this->G->Text = L"G";
			this->G->UseVisualStyleBackColor = true;
			this->G->Click += gcnew System::EventHandler(this, &Form1::G_Click);
			// 
			// B
			// 
			this->B->Location = System::Drawing::Point(476, 403);
			this->B->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->B->Name = L"B";
			this->B->Size = System::Drawing::Size(57, 63);
			this->B->TabIndex = 24;
			this->B->Text = L"B";
			this->B->UseVisualStyleBackColor = true;
			this->B->Click += gcnew System::EventHandler(this, &Form1::B_Click);
			// 
			// H
			// 
			this->H->Location = System::Drawing::Point(542, 331);
			this->H->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->H->Name = L"H";
			this->H->Size = System::Drawing::Size(57, 63);
			this->H->TabIndex = 16;
			this->H->Text = L"H";
			this->H->UseVisualStyleBackColor = true;
			this->H->Click += gcnew System::EventHandler(this, &Form1::H_Click);
			// 
			// N
			// 
			this->N->Location = System::Drawing::Point(542, 403);
			this->N->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->N->Name = L"N";
			this->N->Size = System::Drawing::Size(57, 63);
			this->N->TabIndex = 25;
			this->N->Text = L"N";
			this->N->UseVisualStyleBackColor = true;
			this->N->Click += gcnew System::EventHandler(this, &Form1::button12_Click);
			// 
			// J
			// 
			this->J->Location = System::Drawing::Point(606, 331);
			this->J->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->J->Name = L"J";
			this->J->Size = System::Drawing::Size(57, 63);
			this->J->TabIndex = 17;
			this->J->Text = L"J";
			this->J->UseVisualStyleBackColor = true;
			this->J->Click += gcnew System::EventHandler(this, &Form1::J_Click);
			// 
			// M
			// 
			this->M->Location = System::Drawing::Point(606, 403);
			this->M->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->M->Name = L"M";
			this->M->Size = System::Drawing::Size(57, 63);
			this->M->TabIndex = 26;
			this->M->Text = L"M";
			this->M->UseVisualStyleBackColor = true;
			this->M->Click += gcnew System::EventHandler(this, &Form1::M_Click);
			// 
			// K
			// 
			this->K->Location = System::Drawing::Point(672, 331);
			this->K->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->K->Name = L"K";
			this->K->Size = System::Drawing::Size(57, 63);
			this->K->TabIndex = 18;
			this->K->Text = L"K";
			this->K->UseVisualStyleBackColor = true;
			this->K->Click += gcnew System::EventHandler(this, &Form1::K_Click);
			// 
			// L
			// 
			this->L->Location = System::Drawing::Point(736, 331);
			this->L->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->L->Name = L"L";
			this->L->Size = System::Drawing::Size(57, 63);
			this->L->TabIndex = 19;
			this->L->Text = L"L";
			this->L->UseVisualStyleBackColor = true;
			this->L->Click += gcnew System::EventHandler(this, &Form1::L_Click);
			// 
			// comma
			// 
			this->comma->Location = System::Drawing::Point(672, 403);
			this->comma->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->comma->Name = L"comma";
			this->comma->Size = System::Drawing::Size(57, 63);
			this->comma->TabIndex = 27;
			this->comma->Text = L",";
			this->comma->UseVisualStyleBackColor = true;
			this->comma->Click += gcnew System::EventHandler(this, &Form1::comma_Click);
			// 
			// Period
			// 
			this->Period->Location = System::Drawing::Point(736, 403);
			this->Period->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Period->Name = L"Period";
			this->Period->Size = System::Drawing::Size(57, 63);
			this->Period->TabIndex = 28;
			this->Period->Text = L".";
			this->Period->UseVisualStyleBackColor = true;
			this->Period->Click += gcnew System::EventHandler(this, &Form1::Period_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(1101, 548);
			this->textBox2->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(10, 12);
			this->textBox2->TabIndex = 31;
			this->textBox2->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1214, 809);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->Period);
			this->Controls->Add(this->comma);
			this->Controls->Add(this->L);
			this->Controls->Add(this->K);
			this->Controls->Add(this->M);
			this->Controls->Add(this->J);
			this->Controls->Add(this->N);
			this->Controls->Add(this->H);
			this->Controls->Add(this->B);
			this->Controls->Add(this->G);
			this->Controls->Add(this->V);
			this->Controls->Add(this->F);
			this->Controls->Add(this->C);
			this->Controls->Add(this->D);
			this->Controls->Add(this->S);
			this->Controls->Add(this->X);
			this->Controls->Add(this->Z);
			this->Controls->Add(this->A);
			this->Controls->Add(this->P);
			this->Controls->Add(this->O);
			this->Controls->Add(this->I);
			this->Controls->Add(this->U);
			this->Controls->Add(this->Y);
			this->Controls->Add(this->T);
			this->Controls->Add(this->R);
			this->Controls->Add(this->CAPS);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->Enter);
			this->Controls->Add(this->Space);
			this->Controls->Add(this->Backspace);
			this->Controls->Add(this->Shift);
			this->Controls->Add(this->E);
			this->Controls->Add(this->W);
			this->Controls->Add(this->Q);
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	   System::String^ word;
		char* pchar;
		int tab = 0;
		int tick = 0;
		int t2 = 0;
		//char* pchar = &word[];

		int cursor = 1; // cusor index

		///////////////////////// MOUSE MOVMENT ///////////////////////////////
		/*
		class click
		{
			[D11Import('useer32.d11')]
			static extern void mouseEvent(int dwFlags, int dx, int dy, int dw Data, int dwExtra);

			[Flags]

			public enum MouseEventFlags
			{
				LEFTDOWN = 0X00000002;
				LEFTUP = 0X00000004;
				RIGHTDOWN = 0X00000008;
				RIGTHUP = 0X0000010;
				MIDDLEDOWN = 0X00000020;
				MIDDLEUP = 0X00000040;
				MOVE = 0X00000001;
				ABSOLUTE = 0X00008000;
			};
			public void leftclick(Point p)
			{
				cursor.Position = p;
			}
			public void rightclick(Point p)
			{
				cursor.Position = p;
				mouse_event((int)(mouseEventFlags.RIGHTDOWN), 0, 0, 0, 0);
				mouse_event((int)(mouseEventFlags.RIGHTUP), 0, 0, 0, 0);

			}
			public void doubleclick(Point p)
			{
				cursor.Position = p;
				mouse_event((int)(mouseEventFlags.LEFTDOWN), 0, 0, 0, 0);
				mouse_event((int)(mouseEventFlags.LEFTDUP), 0, 0, 0, 0);

			}
		};
		*/

	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {
		//Populate the list of buttons when we are done
		//This is going to have to be done one by one
		list_of_Keyboard_Buttons_X.push_back(Q->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Q->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(W->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(W->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(E->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(E->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(R->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(R->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(T->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(T->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Y->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Y->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(U->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(U->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(I->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(I->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(O->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(O->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(P->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(P->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Backspace->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Backspace->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(CAPS->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(CAPS->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(A->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(A->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(S->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(S->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(D->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(D->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(F->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(F->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(G->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(G->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(H->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(H->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(J->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(J->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(K->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(K->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(L->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(L->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Enter->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Enter->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Shift->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Shift->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Z->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Z->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(X->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(X->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(C->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(C->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(V->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(V->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(B->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(B->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(N->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(N->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(M->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(M->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(comma->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(comma->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Period->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Period->Location.Y);
		list_of_Keyboard_Buttons_X.push_back(Space->Location.X);
		list_of_Keyboard_Buttons_Y.push_back(Space->Location.Y);
	}

		   ///////////////////// LETTERS //////////////////////
	private: System::Void Q_Click(System::Object^ sender, System::EventArgs^ e) {

		if (Shift->Checked == true || CAPS->Checked == true)
		{
			word = word + "Q";
			Shift->Checked = false;
		}
		else
		{
			word = word + "q";
		}
		this->textBox1->Text = word;
		TabIndex = 2;
		//this->textBox2->;
		tab = 0;
	}
	private: System::Void W_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "W";

		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void E_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "E";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;

	}
	private: System::Void R_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "R";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}


		   ///////////////////// Key Board Functions //////////////////
	private: System::Void Backspace_Click(System::Object^ sender, System::EventArgs^ e) {


		if (textBox1->Text->Length > 0)
		{
			textBox1->Text = textBox1->Text->Remove(textBox1->Text->Length - 1, 1);
		}
		this->word = textBox1->Text;
		tab = 0;
		tick = 1;
		
		if (tick > 1){																												
			word = "";
			this->textBox1->Text = word;
			
		}
		this->Text = tick.ToString();
		tick = 0;

	}
	private: System::Void Shift_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		tab = 0;
	}
	private: System::Void Space_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + " ";

		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void Enter_Click(System::Object^ sender, System::EventArgs^ e) {

		word = "";

		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}

		   //////////////// Output //////////////////
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		tab = 0;

	}
	private: System::Void CursorIndex_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {



	}

	private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {

		//The code for eye calibration
		cv::Mat image;
		vector<cv::Rect> faces;
		cv::Mat grey_Image;
		timer1->Stop();
		

		while (calibration_flag == 0)
		{
			
			//Set the window
			cv::namedWindow("Calibration");
			cv::setMouseCallback("Calibration", mouseEventFunction, NULL);
			cv::namedWindow("CalibrationLeftWink");
			cv::setMouseCallback("CalibrationLeftWink", mouseEventFunctionCalibrationLeftWink, NULL);
			cv::namedWindow("CalibrationRightWink");
			cv::setMouseCallback("CalibrationRightWink", mouseEventFunctionCalibrationRightWink, NULL);

			if (!face_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"))
			{
				//std::cout << "Could not load the cascade" << endl;
				//return 0;
			}
			if (!eyes_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"))
			{
				//std::cout << "Could not load the cascade" << endl;
				//return 0;
			}


			if (!cap.isOpened())
			{
				//std::cout << "cannot open camera";
				//return 0;

			}

		Recalibrate:
			//std::cout << "Adjust your face so that it is on the center of the screen then click on the window." << endl;
			while (input == 's')
			{
				cap >> image;
				cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
				face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, cv::Size(250, 250));
				if (faces.empty() == false)
				{
					cv::rectangle(image, faces[0], cv::Scalar(0, 255, 255));
				}
				cv::imshow("Calibration", image);
				cv::waitKey(5);
				//timer1->Start();
			}

			if (faces.empty() == false && input == 'g')
			{
				//Find the portion of the face that contains the eyes
				int eye_region_width = faces[0].width * (kEyePercentWidth / 100.0);
				int eye_region_height = faces[0].width * (kEyePercentHeight / 100.0);
				int eye_region_top = faces[0].height * (kEyePercentTop / 100.0);
				cv::Rect leftEyeRegion(faces[0].width * (kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);
				cv::Rect rightEyeRegion(faces[0].width - eye_region_width - faces[0].width * (kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);


				cv::Point topLeftCornerLeftEye(leftEyeRegion.x + faces[0].x, leftEyeRegion.y + faces[0].y);
				cv::Point bottomRightCornerLeftEye(leftEyeRegion.x + leftEyeRegion.width + faces[0].x, leftEyeRegion.y + faces[0].y + leftEyeRegion.height);

				cv::Point topLeftCornerRightEye(rightEyeRegion.x + faces[0].x, rightEyeRegion.y + faces[0].y);
				cv::Point bottomRightCornerRightEye(rightEyeRegion.x + rightEyeRegion.width + faces[0].x, rightEyeRegion.y + faces[0].y + rightEyeRegion.height);


				cv::rectangle(image, topLeftCornerLeftEye, bottomRightCornerLeftEye, cv::Scalar(255, 255, 255));
				cv::rectangle(image, topLeftCornerRightEye, bottomRightCornerRightEye, cv::Scalar(255, 255, 255));


				//Find the center of the eyes and then use that to be able to get a better approximation of where the eyes are located
				cv::Mat faceROI = grey_Image(faces[0]);
				cv::Mat faceROI_Colour = image(faces[0]);


				cv::Mat eye1 = faceROI(leftEyeRegion);
				cv::Mat eye2 = faceROI(rightEyeRegion);
				cv::Mat eye_colour1 = faceROI_Colour(leftEyeRegion);
				cv::Mat eye_colour2 = faceROI_Colour(rightEyeRegion);


				//Scale the image to a faster size
				cv::Mat eye_ROI1;
				cv::Mat eye_ROI2;
				//scaleToFastSize(eye1, eye_ROI1);
				//scaleToFastSize(eye2, eye_ROI2);

				cv::Point eye_c1 = objective_function(eye1);
				cv::Point eye_c2 = objective_function(eye2);

				//imshow("faces", faceROI);

				//Calculate the eye height and radius
				//Use that to calibrate where the eyes are


				int radius1 = find_Radius(eye_c1, eye1);
				int radius2 = find_Radius(eye_c2, eye2);
				cv::Point start1(0, 0);
				cv::Point end1(0, 0);
				cv::Point start2(0, 0);
				cv::Point end2(0, 0);

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
				cv::circle(eye_colour1, eye_c1, radius1, cv::Scalar(0, 0, 255), 1);
				cv::circle(eye_colour2, eye_c2, radius2, cv::Scalar(0, 0, 255), 1);
				//Draw the eye_height
				line(eye_colour1, start1, end1, cv::Scalar(0, 255, 0), 2, 8);
				line(eye_colour2, start2, end2, cv::Scalar(0, 255, 0), 2, 8);

				cv::imshow("debugWindow1", eye_colour1);
				cv::imshow("debugWindow2", eye_colour2);
				//timer1->Stop();
				cv::waitKey(5);
				//timer1->Start();
				//std::cout << "Does this look like an accurate picture of the eyes. g for yes and s for no" << endl;
				//cin >> input;
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
				//timer1->Stop();
				cv::waitKey(5);
				//timer1->Start();
				cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
				face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, cv::Size(250, 250));
				if (faces.empty() == false)
				{
					cv::rectangle(image, faces[0], cv::Scalar(0, 255, 255));
					int dumby = 0;
					//timer1->Stop();
					findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, dumby, eyeHeightWinkRight);
					//timer1->Start();
					//cout << "Eye Height For Winking Right: " << eyeHeightWinkRight << endl;
				}
				cv::imshow("CalibrationRightWink", image);
			}
			while (input == 'l')
			{
				cap >> image;
				//timer1->Stop();
				cv::waitKey(5);
				//timer1->Start();
				cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
				face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, cv::Size(250, 250));
				if (faces.empty() == false)
				{
					cv::rectangle(image, faces[0], cv::Scalar(0, 255, 255));
					int dumby = 0;
					//timer1->Stop();
					findEyeHeightAndDisplay(image, grey_Image, faces, xProportionLeft, yProportionLeft, xProportionRight, yProportionRight, eyeHeightProportionLeft, eyeHeightProportionRight, eyeHeightWinkLeft, dumby);
					//timer1->Start();
					//cout << "Eye Height For Winking Left: " << eyeHeightWinkLeft << endl;
				}
				cv::imshow("CalibrationLeftWink", image);
			}
			calibration_flag = 1;
		}

		//While true

		int flagWinkDetection = false;
		//Draw the region of the eyes on the image and then extract them from the image to do the colour channel selection and exponential transform
		cap >> image;
		int eyeHeightLeft;
		int eyeHeightRight;

		//timer1->Stop();
		cv::waitKey(5);
		//timer1->Start();

		//Split the image into the channels and take only the red channel for processing
		std::vector<cv::Mat> rgbChannels(3);
		cv::split(image, rgbChannels);
		grey_Image = rgbChannels[2];


		face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, cv::Size(250, 250));
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
					//cout << "wink" << endl;
					//cout << "Left Eye Height: " << eyeHeightLeft << endl;
					//cout << "Right Eye Height: " << eyeHeightRight << endl;
					flagWinkDetection = true;
					//This is where the clicking event will be handled
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0 , 0 , 0 , 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
				else if (eyeHeightLeft > eyeHeightWinkLeft && eyeHeightRight <= eyeHeightWinkRight)
				{
					//cout << "wink" << endl;
					//cout << "Left Eye Height: " << eyeHeightLeft << endl;
					//cout << "Right Eye Height: " << eyeHeightRight << endl;
					flagWinkDetection = true;
					//This is where the clicking event will be handled
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
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

		//Must adjust this considering that we changed the timer
		//We have to change this from a TAB command to something that will go to the next button
		//In order to do this we must first create an array of buttons
		//First we have to create a new list as a global variable
		//Then after creating the new list we have to populate it using the buttons (this can be done when starting up the program)
		//Then we have to iterate the cursor position to the be the position of the next button

		tab++;
		this->Text = tab.ToString();
		if (tab > 10) {
			tab = 0;
			//We have to change the position of the cursor to the adjacent button
			//Get the list to just keep looping around and then you can assign it non-stop
			int x_coordiante_of_cursor;
			int y_coordinate_of_cursor;
			System::Drawing::Point dummy_Point;
			dummy_Point.X = list_of_Keyboard_Buttons_X.front();
			dummy_Point.Y = list_of_Keyboard_Buttons_Y.front();
			x_coordiante_of_cursor = dummy_Point.X;
			y_coordinate_of_cursor = dummy_Point.Y;

			//Now we have to loop the button around to be able to grad the next front
			list_of_Keyboard_Buttons_X.push_back(list_of_Keyboard_Buttons_X.front());
			list_of_Keyboard_Buttons_X.pop_front();

			list_of_Keyboard_Buttons_Y.push_back(list_of_Keyboard_Buttons_Y.front());
			list_of_Keyboard_Buttons_Y.pop_front();

			System::Windows::Forms::Cursor::Position = System::Drawing::Point(this->Location.X + x_coordiante_of_cursor + 15, this->Location.Y + y_coordinate_of_cursor + 40);
			//int poo = 1;
		}
		timer1->Start();
	}

	private: System::Void timer2_Tick(System::Object^ sender, System::EventArgs^ e) {
		//This is going to be a test timer to test the cursor
		tab++;
		this->Text = tab.ToString();
		if (tab > 10) {
			tab = 0;
			//We have to change the position of the cursor to the adjacent button
			//Get the list to just keep looping around and then you can assign it non-stop
			int x_coordiante_of_cursor;
			int y_coordinate_of_cursor;
			System::Drawing::Point dummy_Point;
			dummy_Point.X = list_of_Keyboard_Buttons_X.front();
			dummy_Point.Y = list_of_Keyboard_Buttons_Y.front();
			x_coordiante_of_cursor = dummy_Point.X;
			y_coordinate_of_cursor = dummy_Point.Y;

			//Now we have to loop the button around to be able to grad the next front
			list_of_Keyboard_Buttons_X.push_back(list_of_Keyboard_Buttons_X.front());
			list_of_Keyboard_Buttons_X.pop_front();

			list_of_Keyboard_Buttons_Y.push_back(list_of_Keyboard_Buttons_Y.front());
			list_of_Keyboard_Buttons_Y.pop_front();

			System::Windows::Forms::Cursor::Position = System::Drawing::Point(this->Location.X + x_coordiante_of_cursor + 15, this->Location.Y + y_coordinate_of_cursor + 40);
			int poo = 1;
		}
	}
	private: System::Void textBox2_TextChanged_1(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void T_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "T";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void Y_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "Y";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void U_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "U";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void I_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "I";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void O_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "O";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void P_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "P";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}

	private: System::Void A_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "A";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
private: System::Void S_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "S";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void D_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "D";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "F";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void G_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "G";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void H_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "H";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void J_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "J";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void K_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "K";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void L_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "L";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void Z_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "Z";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void X_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "X";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void C_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "C";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void V_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "V";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void B_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "B";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void button12_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "N";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void M_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "M";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void comma_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + ",";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void Period_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + ".";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void CAPS_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}
};
	}

