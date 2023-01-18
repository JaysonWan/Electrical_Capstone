// EyebrowDetectionAlgorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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

char input = 's';

//CONSTANTS
const int kEyePercentWidth = 25;
const int kEyePercentHeight = 30;
const int kEyePercentTop = 25;
const int kEyePercentSide = 13;

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


int main()
{
	Mat image;
	double yforheadProportion;
	vector<Rect> faces;
	VideoCapture cap(0);
	Mat grey_Image;

	//Set the window
	namedWindow("Calibration");
	setMouseCallback("Calibration", mouseEventFunction, NULL);

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

	//Have to calibrate for the eye region and then use that to find the region of  the face the contains only the eyebrows
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

		int yHeightOfForheadLeft = (eye_c1.y + leftEyeRegion.y) - (radius1 + 3);
		int yHeightOfForheadRight = (eye_c2.y + rightEyeRegion.y) - (radius2 + 3);

		int forheadHeight = 0;
		//Find the biggest forhead height
		if (yHeightOfForheadLeft > yHeightOfForheadRight)
		{
			forheadHeight = yHeightOfForheadLeft;
			yforheadProportion = (double)forheadHeight /  (double)faceROI.rows;
		}
		else
		{
			forheadHeight = yHeightOfForheadRight;
			yforheadProportion = (double)forheadHeight / (double)faceROI.rows;
		}

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

	while (true)
	{
		cap >> image;
		waitKey(10);
		cv::cvtColor(image, grey_Image, cv::COLOR_BGR2GRAY);
		face_cascade.detectMultiScale(grey_Image, faces, 1.3, 3, 0, Size(250, 250));

		std::vector<cv::Mat> rgbChannels(3);
		cv::split(image, rgbChannels);
		cv::Mat grey_Image = rgbChannels[2];

		if (faces.empty() == false)
		{
			Mat faceROI = grey_Image(faces[0]);
			Rect forheadPortion(0.3* (double)faceROI.cols, 0, faceROI.cols - (0.3 * (double)faceROI.cols), (double)faceROI.rows* yforheadProportion);
			Mat forheadImage = faceROI(forheadPortion);
			imshow("test", forheadImage);
			waitKey(30);

			//Iterate through the rest of the algorithm

			//Blur the image
			//MAKE SURE TO ADJUST THE PARAMETERS OF THE BLURRING
			cv::GaussianBlur(forheadImage, forheadImage, cv::Size(5, 5), 0, 0);

			for (int y = 0; y < forheadImage.rows; ++y) {
				unsigned char* row = forheadImage.ptr<unsigned char>(y);
				for (int x = 0; x < forheadImage.cols; ++x) {
					row[x] = (255 - row[x]);
				}
			}

			//Take the exponential transform of the image
			for (int y = 0; y < forheadImage.rows; ++y) {
				unsigned char* row = forheadImage.ptr<unsigned char>(y);
				for (int x = 0; x < forheadImage.cols; ++x) {
					uchar placeHolder = row[x] * ((double)log(255) / 255.0);
					row[x] = exp(placeHolder);
				}
			}


			//Binarize the image
			//First calculate the average of the intensity
			//Second calculate the standard deviation of the pixels
			int amountOfPixelsinImage;
			amountOfPixelsinImage = forheadImage.cols * forheadImage.rows;
			int sum = 0;
			double average;

			for (int y = 0; y < forheadImage.rows; ++y) {
				unsigned char* row = forheadImage.ptr<unsigned char>(y);
				for (int x = 0; x < forheadImage.cols; ++x) {
					sum += row[x];
				}
			}

			average = (double)sum / (double)amountOfPixelsinImage;

			//Find the standard deviation
			double sumStd = 0.0;

			for (int y = 0; y < forheadImage.rows; ++y) {
				unsigned char* row = forheadImage.ptr<unsigned char>(y);
				for (int x = 0; x < forheadImage.cols; ++x) {
					sumStd += pow((row[x] - average), 2);
				}
			}

			sumStd = sumStd / amountOfPixelsinImage;
			sumStd = sqrt(sumStd);

			for (int y = 0; y < forheadImage.rows; ++y) {
				unsigned char* row = forheadImage.ptr<unsigned char>(y);
				for (int x = 0; x < forheadImage.cols; ++x) {
					if (row[x] > (average + (0.9 * sumStd)))
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
			int areaIncomingImage;
			areaIncomingImage = forheadImage.cols * forheadImage.rows;
			

			params.minArea = 0.01 * (double)areaIncomingImage;

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
			vector<KeyPoint> keypoints;
			detector->detect(forheadImage, keypoints);

			imshow("Shit", forheadImage);



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
