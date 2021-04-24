// Credit for Mandelbrot set generation algorithm and file write algorithm to Adam Sampson
// (with a few tweaks by me)

#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <complex>
#include <thread>

typedef std::chrono::steady_clock theClock; // alias for clock type that's going to be used

// size of image
const int width = 1280;
const int height = 960;

int MAX_IT = 500; // the amount of times we iterate before we determine a point isn't in the set

uint32_t image[height][width]; // image data represented as 0xRRGGBB

// the two colours that the mandelbrot set will be made up of
int firstColour;
int secondColour;

std::string filename;

void write_txt(int threads, int time, const std::string& colourOne, const std::string& colourTwo) {
	std::ofstream outfile;

	outfile.open("output\\index.txt", std::ios_base::app); // append instead of overwrite
	outfile << filename <<
	": \n Resolution: " << width << "*" << height <<
	"\n Colours: " << colourOne << " & " << colourTwo <<
	"\n Number of threads: " << (threads + 1) <<
	"\n Time Taken: " << time << "ms \n\n";
}

// write mandelbrot to .tga file
void write_tga() {
	auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	filename = "output\\mandelbrot-" + std::to_string(timeNow) + ".tga"; // each file can have a unique filename

	std::ofstream outfile(filename, std::ofstream::binary);

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

	// converted to range based thanks to clang-tidy :)
	for (auto & y : image) {
		for (unsigned int x : y) {
			uint8_t pixel[3] = {
					static_cast<uint8_t>(x & 0xFF), // blue channel
					static_cast<uint8_t>(x >> 8 & 0xFF), // green channel
					static_cast<uint8_t>(x >> 16 & 0xFF), // red channel
			};
			outfile.write((const char *)pixel, 3);
		}
	}

	outfile.close();

	// error handling
	if (!outfile)
	{
		// An error has occurred at some point
		std::cout << "Error writing to " << filename << std::endl;
		exit(1);
	}

}

// Render the Mandelbrot set into the image array.
// The parameters specify the region on the complex plane to plot.
void compute(double left, double right, double top, double bottom, int start, int end) {

	for (int x = start; x <= end; ++x) {
		for (int y = 0; y < height; ++y) {

			// Work out the point in the complex plane that corresponds to this pixel in the output image
			std::complex<double> c(left + x * (right - left) / width, top + (y * (bottom - top) / height));

			// Start off z at (0, 0)
			std::complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units away from (0, 0), or we've iterated too many times.
			int it = 0;
			while (abs(z) < 2.0 && it < MAX_IT) {
				z = (z * z) + c;
				++it;
			}

			if (it == MAX_IT) {
				// z didn't escape the circle therefore point is in mandelbrot set
				image[y][x] = firstColour;
			} else {
				// z escaped within < MAX_IT, the point isn't in the set
				image[y][x] = secondColour;
			}
		}
	}
}

int main() {
	std::cout << "CMP 202 Mandelbrot Set Generator - 2021 Isaac Basque-Rice" << std::endl;

	std::string firstColourName;
	std::string secondColourName;
	
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

	std::cout << "Colours: \n 1: White \n 2: Black \n 3: Red \n 4: Orange \n 5: Yellow \n 6: Green \n 7: Blue \n 8: Indigo \n 9: Violet" << std::endl;

	std::cout << "Please choose your foreground colour (1-9): " << std::endl;

	std::cin >> firstColour;

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

	std::cout << "Please choose your background colour (1-9): " << std::endl;

	std::cin >> secondColour;

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

	int threadNumIn = 0;
	std::cout << "How many threads would you like to use? (must be a factor of " << width << "):" << std::endl;

	while (true) {
		std::cin >> threadNumIn;
		if (width % threadNumIn != 0) {
			std::cout << "That number is not a factor of " << width << ", please try again" << std::endl;
		} else {
			break;
		}
	}

	std::cout << "Generating a " << firstColourName << " and " << secondColourName << " Mandelbrot Set, using " << threadNumIn << " threads..." << std::endl;

	double left = -2;
	double right = 1;
	double top = 1.125;
	double bottom = -1.125;

	theClock::time_point start = theClock::now();

	int threadNum = threadNumIn - 1; // array indexes start at zero so only valid numbers are a factor of width - 1
	const int chunkSize = width/threadNum; // get the size of each "chunk" that's being calculated by each thread

	auto *threads = new std::thread[threadNum];

	for (int i = 0; i < threadNum; ++i) {
		threads[i] = std::thread(compute, left, right, top, bottom, (0 + chunkSize * i), chunkSize + chunkSize * i);
	}

	for (int i = 0; i < threadNum; ++i) {
		threads[i].join();
	}

	write_tga();

	theClock::time_point end = theClock::now();

	auto timeTaken = duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Time taken to generate: " << timeTaken << "ms" << std::endl;

	write_txt(threadNum, timeTaken, firstColourName, secondColourName);

	return 0;
}
