#include <iostream>

int firstColour, secondColour; // the two colours that the mandelbrot set will be made up of

int main() {
	std::cout << "CMP 202 Mandelbrot Set Generator - 2021 Isaac Basque-Rice" << std::endl;

	std::string firstColourName;
	std::string secondColourName;
	
	// colour values
	const int white = 0xFFFFF;
	const int black = 0x000000;
	const int red = 0xFF0000;
	const int orange = 0xFFA500;
	const int yellow = 0xFFFF00;
	const int green = 0x00FF00;
	const int blue = 0x0000FF;
	const int indigo = 0x4B0082;
	const int violet = 0x8F00FF;

	std::cout << "Colours: \n 1: White \n 2: Black \n 3: Red \n 4: Orange \n 5: Yellow \n 6: Green \n 7: Blue \n 8: Indigo \n 9: Violet" << std::endl;

	std::cout << "Please choose your first colour (1-9): " << std::endl;

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

	std::cout << "Please choose your second colour (1-9): " << std::endl;

	std::cin >> secondColour;

	switch (secondColour) {
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
		default: secondColour = black; secondColourName = "White";
	}

	std::cout << "Generating a " << firstColourName << " and " << secondColourName << " Mandelbrot Set..."

	return 0;
}
