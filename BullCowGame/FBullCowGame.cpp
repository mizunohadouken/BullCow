#pragma once
#include "FBullCowGame.h"
#include <map>

// to make Unreal friendly
#define TMap std::map
using FString = std::string;
using int32 = int;

FBullCowGame::FBullCowGame(){Reset();} // default constructor

int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
bool FBullCowGame::IsGameWon() const { return bGameIsWon;}

void FBullCowGame::SetHiddenWord(int32 WordLength)
{
	TMap<int32, FString> WordLengthToWord{ {3,"map"},{4,"pain"},{5,"plain"},{6,"planet"},{7,"stumped"} };
	MyHiddenWord = WordLengthToWord[WordLength];
}

int32 FBullCowGame::GetMaxTries() const 
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,4},{4,7},{5,10},{6,16},{7,20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
//	const FString HIDDEN_WORD = "planet";
//	MyHiddenWord = HIDDEN_WORD;


	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess))//if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess))	//if the guess isn't all lowecase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength())// if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
	return EGuessStatus::OK;
}

//reeives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry ++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				if (MHWChar == GChar)
				{// increment bulls if they're in the same place
					BullCowCount.Bulls++; //increment bulls
				}
				else
				{
					BullCowCount.Cows++; //must be a cow
				}
			}
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; //setup our map
	for (auto Letter : Word)	 //for all letters of the word
	{
		Letter = tolower(Letter); //handle mixed upper/lower case
		if (LetterSeen[Letter])   // if the letter is in the map
		{
			return false;// we do not have an isogram
		}
		else
		{
			LetterSeen[Letter] = true;// add the later to the map as seen
		}
	}
	return true;  // for eample in case where \0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	for (auto Letter : Word)
	{
		if (!islower(Letter)) // if not a lowercase
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
