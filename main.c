// Windows includes
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define getch() _getch()
#endif

// POSIX includes
#ifndef _WIN32
#define BUFSIZ 512
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define getch() getchar()
#define scanf_s scanf
#include <unistd.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define random() (rand() % 100)

#define MSG_ENTRIES "How many \"easter egg\" entries to add?\n> "
#define MSG_INVALIDBUBBLEAMOUNT "Invalid usage. Requires 1 or more easter eggs.\n"
#define MSG_POPMESSAGE "What message should display when \"popping\" a bubble?\n> "
#define MSG_EASTEREGG "Enter easter egg %d\n> "
#define MSG_MEMORY "Insufficient memory.\n"
#define MSG_WEIGHT "What percentage of bubbles should be easter eggs? (e.g. 25)\n> "
#define MSG_PAUSE "Press any key to continue . . . "

#define SPOILERTAG "||"
#define MESSAGELIMIT 2000


// Prompt for integer.
static int prompti(const char *format, ... );

// Prompt for float.
static float promptf(const char* format, ...);

// Prompt for string.
static void prompts(char* output, const char* format, ...);

int main(void)
{
	// Seed random number generator.
	srand((unsigned int)time(NULL));

	// Ask for how many entries.
	size_t bubbleAmount = prompti(MSG_ENTRIES);

	// Calculate easter egg weight between them all
	unsigned char eggWeight = (unsigned char)(100.0f / (float)bubbleAmount);
	
	// Sanity check
	if(bubbleAmount < 1)
	{
		printf(MSG_INVALIDBUBBLEAMOUNT);
		return 0;
	}

	// Prompt for the pop message
	char popMessage[BUFSIZ] = {'\0'};
	prompts(popMessage, MSG_POPMESSAGE);

	// Create array of pointers corresponding to amount
	char** easterEggs = (char**)calloc(bubbleAmount, sizeof(char*));
	
	// Make sure all is well
	if(!easterEggs)
	{
		printf(MSG_MEMORY);
		return -1;
	}


	// Allocate each member
	for(size_t i = 0; i < bubbleAmount; i++)
	{
		easterEggs[i] = (char*)calloc(BUFSIZ, sizeof(char));
		
		// Make sure all is well
		if(!easterEggs[i])
		{
			printf(MSG_MEMORY);
			return -1;
		}
	}

	// Prompt for every entry
	for(size_t i = 0; i < bubbleAmount; i++)
	{
		// Prompt
		prompts(easterEggs[i], MSG_EASTEREGG, i + 1);
	}
	
	// Prompt for weight percentage
	unsigned char weightPercentage = (unsigned char)(promptf(MSG_WEIGHT));

	// Generate bubble wrap
	// Output (2000 is Message Limit)
	char outputStr[MESSAGELIMIT] = {'\0'};


	// Iterate through until we're out of chars
	{
		size_t i = 0;
		unsigned char myPercentage;
		while(i < MESSAGELIMIT - BUFSIZ)
		{
			myPercentage = random();

			// Write spoiler tag at beginning
			strcat(outputStr, SPOILERTAG);

			// Check if we got an easter egg
			if(myPercentage <= weightPercentage)
			{
				// Write a random easter egg to output
				strcat(outputStr, easterEggs[(random() % max(bubbleAmount, min(0, (bubbleAmount))))]);				
			}
			// Otherwise, put in "pop" phrase
			else
			{
				strcat(outputStr, popMessage);
			}

			// Write spoiler tag at end
			strcat(outputStr, SPOILERTAG);

			i = strlen(outputStr);
		}

	}

	// Print out the finalized message
	printf("Final message:\n\n%s\n", outputStr);



	// Deallocate each member of pointer array
	for(size_t i = 0; i < bubbleAmount; i++)
	{
		free(easterEggs[i]);
	}

	// Deallocate pointer array
	free(easterEggs);

	// Print pause message and pause
	printf(MSG_PAUSE);
	getch();

	return 0;
}

static int prompti(const char* format, ...)
{
	int instance = 0;

	// Start arglist
	va_list arg_list;
	va_start(arg_list, format);

	// Print out prompt
	vfprintf(stdout, format, arg_list);

	// Get input from user
	scanf_s("%d", &instance);

	// End arglist
	va_end(arg_list);

	// Output newline
	fputc('\n', stdout);

	return instance;
}

static float promptf(const char* format, ...)
{
	float instance = 0;

	// Start arglist
	va_list arg_list;
	va_start(arg_list, format);

	// Print out prompt
	vfprintf(stdout, format, arg_list);

	// Get input from user
	scanf_s(" %f", &instance);


	// End arglist
	va_end(arg_list);

	// Output newline
	fputc('\n', stdout);

	return instance;
}

static void prompts(char* output, const char* format, ...)
{
	// Start arglist
	va_list arg_list;
	va_start(arg_list, format);

	// Print out prompt
	vfprintf(stdout, format, arg_list);

	// End arglist
	va_end(arg_list);

	// Get input from user
	scanf_s(" %[^\n]s", output);



	// Output newline
	fputc('\n', stdout);

	return;
}
