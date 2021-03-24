// Credit for Mandelbrot set generation algorithm to Adam Sampson

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <complex>
#include <thread>

using std::cout;
using std::endl;
using std::string;
using std::to_string;
using std::cin;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::ofstream;
using std::complex;
using std::thread;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


typedef steady_clock theClock; // alias for clock type that's going to be used

// size of image
const int width = 1280;
const int height = 960;

int MAX_IT = 1024; // the amount of times we iterate before we determine a point isn't in the set

uint32_t image[height][width]; // image data represented as 0xRRGGBB

// the two colours that the mandelbrot set will be made up of
int firstColour;
int secondColour;

void write_tga() {
	auto timeNow = system_clock::to_time_t(system_clock::now());
	string filename;
	filename = "mandelbrot-" + to_string(timeNow) + ".tga";

	ofstream outfile(filename, ofstream::binary);

	uint8_t header[18] = {
			0, //no image ID
			0, //no colour map
			2, //uncompressed 24-bit image
			0, 0, 0, 0, 0, //empty colour map specification
			0, 0, //X origin
			0, 0, //Y origin
			width & 0xFF, width >> 8 & 0xFF, //width
			height & 0xFF, height >> 8 & 0xFF, //height
			24, //bits per pixel
			0, //image descriptor
	};
	outfile.write((const char *)header, 18);

	for (auto & y : image)
	{
		for (unsigned int x : y)
		{
			uint8_t pixel[3] = {
					static_cast<uint8_t>(x & 0xFF), // blue channel
					static_cast<uint8_t>((x >> 8) & 0xFF), // green channel
					static_cast<uint8_t>((x >> 16) & 0xFF), // red channel
			};
			outfile.write((const char *)pixel, 3);
		}
	}

		outfile.close();
		if (!outfile)
		{
			// An error has occurred at some point
			cout << "Error writing to " << filename << endl;
			exit(1);
		}

	}

void compute(double left, double right, double top, double bottom, int start, int end) {

	for (int x = start; x <= end; ++x) {
		for (int y = 0; y < height; ++y) {
			complex<double> c(left + x * (right - left) / width, top + (y * (bottom - top) / height));
			complex<double> z(0.0, 0.0);

			int it = 0;
			while (abs(z) < 2.0 && it < MAX_IT) {
				z = (z * z) + c;
				++it;
			}

			if (it == MAX_IT) {
				image[y][x] = firstColour;
			} else {
				image[y][x] = secondColour;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	cout << "CMP 202 Mandelbrot Set Generator - 2021 Isaac Basque-Rice" << endl;

	string firstColourName;
	string secondColourName;
	
	// colour values
	const int white = 0xFFFFFF;
	const int black = 0x000000;
	const int red = 0xFF0000;
	const int orange = 0xFFA500;
	const int yellow = 0xFFFF00;
	const int green = 0x00FF00;
	const int blue = 0x0000FF;
	const int indigo = 0x4B0082;
	const int violet = 0x8F00FF;

	cout << "Colours: \n 1: White \n 2: Black \n 3: Red \n 4: Orange \n 5: Yellow \n 6: Green \n 7: Blue \n 8: Indigo \n 9: Violet" << endl;

	cout << "Please choose your foreground colour (1-9): " << endl;

	cin >> firstColour;

	switch (firstColour) {
		case 1: firstColour = white; firstColourName = "White";
			break;
		case 2: firstColour = black; firstColourName = "Black";
			break;
		case 3: firstColour = red; firstColourName = "Red";
			break;
		case 4: firstColour = orange; firstColourName = "Orange";
			break;
		case 5: firstColour = yellow; firstColourName = "Yellow";
			break;
		case 6: firstColour = green; firstColourName = "Green";
			break;
		case 7: firstColour = blue; firstColourName = "Blue";
			break;
		case 8: firstColour = indigo; firstColourName = "Indigo";
			break;
		case 9: firstColour = violet; firstColourName = "Violet";
			break;
		default: firstColour = white; firstColourName = "White";
			break;
	}

	cout << "Please choose your background colour (1-9): " << endl;

	cin >> secondColour;

	switch (secondColour) {
		case 1: secondColour = white; secondColourName = "White";
			break;
		case 2: secondColour = black; secondColourName = "Black";
			break;
		case 3: secondColour = red; secondColourName = "Red";
			break;
		case 4: secondColour = orange; secondColourName = "Orange";
			break;
		case 5: secondColour = yellow; secondColourName = "Yellow";
			break;
		case 6: secondColour = green; secondColourName = "Green";
			break;
		case 7: secondColour = blue; secondColourName = "Blue";
			break;
		case 8: secondColour = indigo; secondColourName = "Indigo";
			break;
		case 9: secondColour = violet; secondColourName = "Violet";
			break;
		default: secondColour = black; secondColourName = "Black";
	}

	cout << "Generating a " << firstColourName << " and " << secondColourName << " Mandelbrot Set..." << endl;

	double left = -2;
	double right = 1;
	double top = 1.125;
	double bottom = -1.125;

	theClock::time_point start = theClock::now();

	const int threadNum = 7; // indexes start at zero so factor of 1280 - 1
	const int chunkSize = width/threadNum;

	thread *threads[threadNum];

	for (int i = 0; i < threadNum; ++i) {
		threads[i] = new thread(compute, left, right, top, bottom, (0 + chunkSize * i), chunkSize + chunkSize * i);
	}

	for (int i = 0; i < threadNum; ++i) {
		threads[i]->join();
		delete(threads[i]);
	}

	write_tga();

	theClock::time_point end = theClock::now();

	auto timeTaken = duration_cast<milliseconds>(end - start).count();
	cout << "Time taken to generate: " << timeTaken << "ms" << endl;

	return 0;
}
