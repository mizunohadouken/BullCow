//
//  main.cpp
//  BullCowGame
//
//  Created by Andrew on 9/5/17.
//  Copyright © 2017 Andrew. All rights reserved.
//

/* This is the console executable that makes use of the BullCow class
This acts as the view in a MVC pattern and is responsible for all
user interaction.  For game logic, see the FBullCowGame class.

*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PrintIntro();
void PlayGame();
FText GetValidGuess();
int32 GetWordLength();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game which we re-use across plays

//entry point for application
int main()
{
    bool bPlayAgain = false;   
    do
    {
    PrintIntro();
    PlayGame();
    bPlayAgain = AskToPlayAgain();
    }
    while(bPlayAgain);

    
    std::cout << std::endl;
    return 0; //exit application
}

void PrintIntro()
{
    std::cout << "Welcome to Bulls and Cows, a crappy game.\n";
	int32 UserWordLength = GetWordLength();
	BCGame.SetHiddenWord(UserWordLength);//set the hidden word based on user inputted word length

	int32 WORD_LENGTH = BCGame.GetHiddenWordLength();
    std::cout << "Can you guess the " << WORD_LENGTH;
    std::cout << " letter isogram?\n";
    std::cout << std::endl;
    return;
}

//plays a single game to completion
void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

    // loop asking for guesses while the game is NOT won
	// and there are still tries remaining
    while(!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
    {
        FText Guess = GetValidGuess();

		// submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << std::endl;

        std::cout << std::endl;
    }
	PrintGameSummary();
	return;
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon() == true)// check if current turn is greater than max tries
	{
		std::cout << "Congratuations, you won!\n";
	}
	else// anything else than or equal to max is win condition
	{
		std::cout << "Too bad, please play again\n";
	}
	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do
	{
		//get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry <<" of " << BCGame.GetMaxTries();
		std::cout << ". Please enter your guess:";
		std::getline(std :: cin,Guess);

		// check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter in all lower case\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); //keep looping until we get no errors
	return Guess;
}

int32 GetWordLength()
{
	int32 WordLength = 0;

	//get player to enter desired word length
	std::cout << "How long of a word would you like to try? (3-7 letters): ";
	std::cin >> WordLength;
//	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	return WordLength;
}

bool AskToPlayAgain()
{
    std::cout << "Do you want to play again? (y/n)";
    FText Response = "";
    std::getline(std :: cin, Response);
	std::cout << std::endl;

    return (Response[0] == 'y' || Response[0] =='Y');
}

