//Pupil-Detection Software: This file will be used for the main eye-tracking portion of the software
//This piece of software will utilize the algorithm that is highlighted in the pupil tracking code

//C++ header files
#include <iostream>
#include <math.h>

//OpenCV header files 
//#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/core/types.hpp>

using namespace cv;

using namespace std;

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

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


vector<double> Compute_Displacement_Vector(const cv::Point xi, const cv::Point c) //Done
{
	//Create the displacement vector
	vector<double> d_Vector;
	//Find the magnitude
	double dMagnitude; //Setting this equal to one may be a point of error

	double dx;
	double dy;
	dx = xi.x - c.x;
	dy = xi.y - c.y;
	dMagnitude = sqrt(((dx) * (dx)) + ((dy) * (dy)));

	double Magnitude_x = dx / dMagnitude;
	double Magnitude_y = dy / dMagnitude;

	d_Vector.push_back(Magnitude_x);
	d_Vector.push_back(Magnitude_y);
	return d_Vector;
}


Point objective_function(const cv::Mat& image)
{
	//Combination of both and make a for_loop
	int r = image.rows;
	int col = image.cols;
	const int N = r * col;

	//Make sure to check this over
	double* c_value = new double[N];
	vector<double>* displacement = new vector<double>[N];
	double* dotproduct = new double[N];
	double* c_astrix_values = new double[N];

	//Counter used to index the displacement
	int counter = 0;
	int counter3 = 0;
	//Compute the gradient
	Mat gradient_X = Compute_Gradient_X(image);
	Mat gradient_Y = Compute_Gradient_X(image.t()).t();


	for (int k = 0; k < image.rows; k++) {
		for (int z = 0; z < image.cols; z++) {
			//Have to reset the counter
			counter = 0;
			Point c(k + 1, z + 1);
			for (int i = 0; i < image.rows; i++)
			{
				for (int j = 0; j < image.cols; j++)
				{
					//Point xi
					Point xi(i + 1, j + 1);
					displacement[counter] = Compute_Displacement_Vector(xi, c);

					//This is used to avoid the NaN error in the code
					if ((xi.x == c.x) && (xi.y == c.y))
					{
						displacement[counter][0] = 0;
						displacement[counter][1] = 0;
					}


					//Take the dot-product
					//MAKE SURE TO CHECK IF THE DATATYPE IS CORRECT

					dotproduct[counter] = displacement[counter][0] * gradient_X.at<double>(i, j) + displacement[counter][1] * gradient_Y.at<double>(i, j);

					//Take the square of the results
					dotproduct[counter] = (dotproduct[counter]) * (dotproduct[counter]);
					counter++;
				}
				//Make sure to check this over

			}
			//Compute the sum

			int counter2 = 0;
			double sum = 0;
			while (counter2 < N)
			{
				sum = sum + dotproduct[counter2];
				counter2++;
			}
			//Put the sum into an array
			c_astrix_values[counter3] = sum / N;
			counter3++;
		}
	}

	double max = c_astrix_values[0];
	int max_index = 0;

	//Find the max value for c_atrix_values
	for (int q = 0; q < N; q++)
	{
		if (c_astrix_values[q] > max)
		{
			max = c_astrix_values[q];
			max_index = q;
		}
	}

	//Find the coordinates of the largest value
	//Have to convert from a linear index to a 2x2 index
	double number1 = double(max_index) / double(image.cols);
	int row = ceil(number1);
	int column = max_index - ((row - 1) * image.cols);
	Point center_of_eye(row, column);
	delete[] c_value;
	delete[] displacement;
	delete[] dotproduct;
	delete[] c_astrix_values;
	cout << "eye_center_x:" << center_of_eye.x << "," <<"eye_center_y:"<< center_of_eye.y << endl;
	return center_of_eye;
}

int main() {

	Mat image;
	vector<Rect> faces;
	//namedWindow("Display window");
	VideoCapture cap(0);
	//Location of Cascade

	if (!face_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"))
	{
		cout << "Could not load the cascade" << endl;
		return 0;
	}
	if (!eyes_cascade.load("C:/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"))
	{
		cout << "Could not load the cascade" << endl;
		return 0;
	}


	if (!cap.isOpened()) 
	{
		cout << "cannot open camera";

	}

	while (true) 
	{
		cap >> image;
		waitKey(25);
		//Detect face using the haar cascade
		//Have to convert the image to grayscale before running haar cascade
		Mat image_grey;
		cvtColor(image, image_grey, COLOR_BGR2GRAY);
		equalizeHist(image_grey, image_grey);

		face_cascade.detectMultiScale(image_grey, faces);
		if (faces.empty() == false) 
		{
			//Point center(faces[0].x + faces[0].width / 2, faces[0].y + faces[0].height / 2);
			//ellipse(image, center, Size(faces[0].width / 2, faces[0].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
		
			//Detect the eyes within the face
			Mat faceROI = image_grey(faces[0]);
			vector<Rect> eyes;
			eyes_cascade.detectMultiScale(faceROI, eyes);

			if(eyes.size() >= 2)
			{
				//Point eye_center1(faces[0].x + eyes[0].x + eyes[0].width / 2, faces[0].y + eyes[0].y + eyes[0].height / 2);
				//int radius = cvRound((eyes[0].width + eyes[0].height) * 0.25);
				//circle(image, eye_center1, radius, Scalar(255, 0, 0), 4);
			
				//Point eye_center2(faces[0].x + eyes[1].x + eyes[1].width / 2, faces[0].y + eyes[1].y + eyes[1].height / 2);
				//int radius2 = cvRound((eyes[1].width + eyes[1].height) * 0.25);
				//circle(image, eye_center2, radius2, Scalar(255, 0, 0), 4);
			
				Mat eye1 = faceROI(eyes[0]);
				Mat eye2 = faceROI(eyes[1]);

				//Create a point that will be the center of the pupil
				//Point center_of_pupil_1(faces[0].x + eyes[0].x + objective_function(eye1).x, faces[0].y + eyes[0].y + objective_function(eye1).y);
				//Point center_of_pupil_2(faces[0].x + eyes[1].x + objective_function(eye2).x, faces[0].y + eyes[1].y + objective_function(eye2).y);
				//circle(image, center_of_pupil_1, 2, Scalar(255, 0, 0), 4);
				//circle(image, center_of_pupil_2, 2, Scalar(255, 0, 0), 4);

				
				Point eye_c = objective_function(eye1);
				circle(eye1, eye_c, 2, Scalar(255, 255, 255), 4);
				//imshow("Face Camera", image);
				imshow("Eyes", eye1);

				Point face(faces[0].x + eyes[0].x + eye_c.x, faces[0].y + eyes[0].y + eye_c.y);
				circle(image, face, 2, Scalar(255, 0, 0), 4);
				
				cout << "eye_width:" << eyes[0].width << "," <<"eye_height:"<< eyes[0].height << endl;
				//circle(eye1, center_of_eye1, 2, Scalar(255, 0, 0), 10);
				imshow("Display_Window", image);
				
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
