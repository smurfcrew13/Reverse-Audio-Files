//  Main program
//		Reverse an audio file.
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "linkedStack.h"
#include "linkedQueue.h"

int main(int argc, char *argv[])
{
	std::ifstream inputFile;
	std::ofstream outputFile; 

	if (argc != 3){
		if (argc == 1){			
			std::cout << "Usage: ./reverse <inputFile> <outputFile>" << std::endl;
		} else {
			std::cerr << "Error, must provide input and output file names." << std::endl;
		}
		return EXIT_FAILURE;
	}

	inputFile.open(argv[1]);
	if(!inputFile.is_open()){
		std::cout << "Error, unable to open input file: " << argv[1] << std::endl;
			return EXIT_SUCCESS;
		}

	outputFile.open(argv[2]);
	if(!outputFile.is_open()){
		std::cout << "Error, unable to open output file: " << argv[2] << std::endl;
			return EXIT_SUCCESS;
	}

	std::string	bars(50, '-');
	std::cout << "Audio Reversal Program" << std::endl << std::endl;

	const std::string expectedHeader = "; Sample Rate";
	std::string holdString;
	std::getline(inputFile, holdString);

    if(holdString.find(expectedHeader) == std::string::npos){
		std::cerr << "Invalid audio data file" << std::endl;
		return EXIT_FAILURE;
	}
	
	//tokenize holdString
	std::stringstream ss (holdString);
	unsigned int sampleRate;
	ss >> sampleRate;
	//	outputFile << holdString << std::endl;
	if (ss.fail()) {
		std::cerr << "Error, unable to read sample rate from header" << std::endl;
		return EXIT_FAILURE;
	}

	std::getline(inputFile, holdString);
	outputFile << holdString << std::endl;

	linkedQueue<double> dQueue(sampleRate/2);
	linkedStack<double> dStack(sampleRate/2);
	double	sample1 = 0.0, sample2 = 0.0;

	// read input file, store samples
	while(std::getline(inputFile, holdString)){
		std::stringstream lineStream (holdString);
		lineStream >> sample1 >> sample2;

        if(lineStream.fail()){
            std::cerr << "Warning: Skipping invalid data line: " << holdString << std::endl;
			continue;
		}
		dStack.push(sample2);
		dQueue.addItem(sample1);
	}

	// show count of all samples
	std::cout << "Count = " << dQueue.queueCount() << std::endl;

	while(!dStack.isEmptyStack() && !dQueue.isEmptyQueue()){
		outputFile << " " << dQueue.front() << " " << dStack.top() << std::endl;
		dQueue.deleteItem();
		dStack.pop();
	}

	// check for mismatch data structures
    if(!dStack.isEmptyStack() || !dQueue.isEmptyQueue()){
		std::cerr << "Warning: Mismatched stack/queue sizes at completion\n";
	}

	inputFile.close();
	outputFile.close();

	std::cout << bars << std::endl << "Reversal completed." << std::endl;

	return	EXIT_SUCCESS;
}