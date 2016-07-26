#define _CRT_SECURE_NO_DEPRECATE

#include "utilities.h"
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

const int MAXWORDS = 10000;
const int MAXPROBELEN = 100;


/*
 Determines the number of rocks and pebbles from a probe word.
 Uses two cstring arguments for the secret word and the probe word.
 Passes two int values by reference for modification: rocks and pebbles
 */
void findRocksAndPebbles(char word[MAXWORDLEN + 1], char probe[MAXPROBELEN], int& r, int& p)
{
    //a placeholder value that tells us what characters we have already considered to be rocks and pebbles
    const char usedCharValue = '0';
    
    //reset values for rocks and pebbles
    r = 0;
    p = 0;
    
    //create copies of the cstrings that can be modified when determining rocks and pebbles
    char answer[MAXWORDLEN + 1];
    strcpy(answer, word);
    char guess[MAXWORDLEN + 1];
    strcpy(guess, probe);
    
    //checks for rocks in the words, and then changes the characters that are seen as rocks
    for (int i = 0; answer[i] != '\0' && guess[i] != '\0'; i++)
    {
        if (answer[i] == guess[i])
        {
            answer[i] = usedCharValue;
            guess[i] = usedCharValue;
            r++;
        }
    }
    
    //in the remaining characters, checks for pebbles and changes the characters that are seen as pebbles
    for (int k = 0; guess[k] != '\0'; k++)
    {
        if (guess[k] != usedCharValue) // if the probe word character has not already been used
        {
            for (int w = 0; answer[w] != '\0'; w++) // check every character in the secret word
            {
                if (answer[w] != usedCharValue && answer[w] == guess[k]) // if the secret character has not been used already and matches a probe character
                {
                    answer[w] = usedCharValue;
                    guess[k] = usedCharValue;
                    p++;
                }
            }
        }
    }
}


int manageOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
    int numTries = 0;
    int rocks = 0;
    int pebbles = 0;
    
    if (nWords <= 0 || wordnum < 0 || wordnum >= nWords) // invalid values for nWords and/or wordnum
        return -1;
    
    char word[MAXWORDLEN + 1];
    strcpy(word, words[wordnum]);
    
    for (;;)
    {
        char probe[MAXPROBELEN] = "";
        
        cout << "Probe word: ";
        cin.getline(probe, MAXPROBELEN);
        
        if (strlen(probe) < MINWORDLEN || strlen(probe) > MAXWORDLEN) // check to make sure probe word length is from 4 to 6
        {
            cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
            continue; // if the probe word is of invalid length, continue to next ieration
        }
        
        bool validProbe = true; // bool value to record validity of the probe word
        
        for (int i = 0; probe[i] != '\0'; i++) // check every character in probe word to see it's a lowercase letter
        {
            if (!(islower(probe[i]))) // if character is not lower case letter
            {
                validProbe = false;
                cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
                break; // stop checking the characters in the probe word because the probe is invalid
            }
        }
        if (!validProbe) // if probe is invalid, continue to next iteration
            continue;
        
        validProbe = false; // assume the word is not in the list of words
        
        for (int i = 0; i < nWords; i++) // check elements in the list to see if word is there
        {
            if ( strcmp(words[i], probe) == 0) // if the probe word is in the list, it is a valid probe word
            {
                validProbe = true;
            }
        }
        
        if (!validProbe) // if the probe word is not in the list, exit iteration and ask for another probe word
        {
            cout << "I don't know that word." << endl;
            continue;
        }
        
        numTries++; // probe word is valid and so user has done a try
        
        if ( strcmp(word, probe) == 0) // user has correctly guessed the word
        {
            break; // exit the guessing part of the function (the continuous loop)
        }
        
        findRocksAndPebbles(word, probe, rocks, pebbles);
        cout << "Rocks: " << rocks << ", Pebbles: " << pebbles << endl;
    }
    return numTries;
}


int main()
{
    //load the word list
    char wordList[MAXWORDS][MAXWORDLEN + 1];
    int nWords = loadWords(wordList, MAXWORDS);
    
    if (nWords < 1) // text file was not found or contained zero words
    {
        cout << "No words were loaded, so I can't play the game." << endl;
        return 0;
    }
    
    // prompt user for the number of rounds to be played
    int numRounds;
    cout << "How many rounds do you want to play? ";
    cin >> numRounds;
    
    if (numRounds < 0) // invalid value for the number of rounds
    {
        cout << "The number of rounds must be positive." << endl;
        return 0;
    }
    
    cout << endl;
    cin.ignore(10000, '\n');
    
    double total = 0;
    int min = 0;
    int max = 0;
    
    // the average has two decimal points
    cout.setf(ios::fixed);
    cout.precision(2);
    
    for (int i = 1; i <= numRounds; i++)
    {
        cout << "Round " << i << endl;
        
        int wordNum = randInt(0, nWords - 1);
        
        cout << "The secret word is " << strlen(wordList[wordNum]) << " letters long." << endl;
        
        //*******************************************************************************
        // the following line is for testing purposes
        //cout << wordList[wordNum] << endl;
        
        int numTries = manageOneRound(wordList, nWords, wordNum);
        
        if (numTries == 1)
        {
            cout << "You got it in 1 try." << endl;
        }
        else
        {
            cout << "You got it in " << numTries << " tries." << endl;
        }
        
        total += numTries; // add this round's tries to the running total of tries
        
        if (i == 1) // the first round numTries is the minimum and maximum
        {
            min = numTries;
            max = numTries;
        }
        else // every round after the first
        {
            // determine if numTries is a new minimum or new maximum
            if (numTries < min)
                min = numTries;
            if (numTries > max)
                max = numTries;
        }
        // (total/i) is the average. It is the total number of tries divided by the number of rounds completed
        cout << "Average: " << (total/i) << ", minimum: " << min << ", maximum: " << max << endl;
        
        cout << endl;
    }
}
