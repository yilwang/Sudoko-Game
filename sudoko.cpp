/***********************************************************************
* Program:
*    Project Sudoko
*
* Summary:
*    This program is to make the game, Sudoku!
************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <ctype.h>
using namespace std;


void getFileName(string &filename);
bool readFile(string filename,int board[][9]);
void displayOptions();
void displayBoard(int board[][9]);
char getCommand();
void interact(int board[][9], char option );
void editSquare(int board[][9]);
void showPossibleValue(int board[][9]);
string writeNewFile(int board[][9]);
void modifyBoard(int board[][9], int row, int col, int value);
bool isValueValid( int row, int col, int board[][9], int value);
void computeValue(int row, int col, int board[][9], int possibleValue[]);


/*********************************************************
 * This function is where the computer starts to read.
 *********************************************************/
int main()
{
   string filename;
   // get the filename from user
   getFileName(filename);

   int board[9][9];
   // read the file
   readFile(filename, board);

   // display the options
   displayOptions();

   // display the board
   displayBoard(board);

   // get the command
   char option = getCommand();

   //interact with the ommand
   while (option != 'Q')
   {
      interact(board,option);
      option = getCommand();
   }

   // end? or prompt user for filename and save
   writeNewFile(board);

   return 0;
}

/*******************************************************
 * This function gets the file name from the user.
 *******************************************************/
void getFileName(string &filename )
{
   cout << "Where is your board located? ";
   cin >> filename;
}

/*****************************************************
 * This function reads the file input by the user.
 ****************************************************/
bool readFile(string filename,int board[][9])
{
   // open the file
   ifstream fin(filename.c_str());

   // check if fail
   if (fin.fail())
   {
      cout << "error file!";
      return false;
   }

   // read the file

   while ( !fin.eof())
   {
      for (int i = 0; i < 9; i++ )
      {
         for (int j = 0; j < 9; j++ )
         {
            fin >> board[i][j];

            //cout << board[i][j] << endl;
         }
      }
   }
   //close the file
   fin.close();

   return true;
}

/*****************************************************
 * This function displays the options to the user.
 *****************************************************/
void displayOptions()
{
   cout << "Options:" << endl;
   cout << "   ?  Show these instructions" << endl;
   cout << "   D  Display the board" << endl;
   cout << "   E  Edit one square" << endl;
   cout << "   S  Show the possible values for a square" << endl;
   cout << "   Q  Save and Quit" << endl;
   cout << endl;
}

/*****************************************************
 * This function displays the boards to the user.
 *****************************************************/
void displayBoard(int board[][9])
{
   for ( int i = 0 ; i < 9 ; i++ )
   {
      if (i == 0)
      {
         cout << "   A B C D E F G H I" << endl;
      }  
      cout << i + 1 << "  ";
      
      for ( int j = 0; j < 9 ; j++ )
      {
         if (board[i][j] != 0)
            cout << board[i][j];
         else
            cout << " ";

         if (j == 2 || j == 5)
            cout << "|";
         else if (j == 8)
            cout << "\n";
         else
            cout << " ";

      }
      //cout << endl;

      if (i == 2 || i == 5)
         cout << "   -----+-----+-----" << endl;

   }
   cout << endl;
}

/*****************************************************
 * This function gets the command from the user.
 *****************************************************/
char getCommand()
{
   char sign;
   cout << "> ";
   cin >> sign;

   if ( sign != '?' && sign != 'D' && sign != 'E' && sign != 'S' && sign != 'Q')
   {
      cout << "The command is invalid!" << endl;
      //cout << "> ";
      cin >> sign;
   }
   return sign;
}


/***************************************************************
 * This function gets the usr nput for an option.(Play the game)
 ***************************************************************/
void interact(int board[][9], char option )
{
   switch ( option )
   {
      case '?':
         displayOptions();
         cout << "\n";
         break;
      case 'D':
         displayBoard(board);
         break;
      case 'E':
         editSquare(board);
         break;
      case 'S':
         showPossibleValue(board);
         break;
      case 'Q':
         writeNewFile(board);
         break;
   }
}

/***************************************************************
 * This function writes the data into a new file.
 ***************************************************************/
string writeNewFile(int board[][9])
{
   string newFile;
   cout << "What file would you like to write your board to: ";
   cin >> newFile;

   // declare the output stream;
   ofstream fout;

   //open the file;
   fout.open(newFile.c_str());

   //write the data into the new file
   for ( int i = 0; i < 9 ; i++ )
   {
      for ( int j = 0; j < 9 ; j++ )
      {
         // DEBUG
         // cout << i << ", " << j << ": " << board[i][j] << "; ";
         fout << board[i][j] << " ";
      }
      fout << "\n";
   }

   cout << "Board written successfully\n";

   //close the stream;
   fout.close();

   return newFile;
}


/***************************************************************
 * This function edits the square.
 ***************************************************************/
void editSquare(int board[][9])
{
   string location;
   cout << "What are the coordinates of the square: " ;
   cin >> location;

   int value;
   int col = toupper(location[0]) - 'A';
   int row = location[1] - '1';

   // check if the location is filled or not
   if (board[row][col] != 0)
   {
      cout << "ERROR: Square '" << location << "' is filled" << endl;
      //cout << endl;
   }
   else
   {
      cout << "What is the value at '" << location << "': ";
      cin >> value;

      if ( value < 1 || value > 9)
      {
         cout << "This is an invalid value!" << endl;
         cout << "What is the value at '" << location << "': ";
         cin >> value;
         //input value and location
         // modifyBoard(board, row, col, value);
      }
      else if (!isValueValid(row, col, board, value))
      {
         cout << "ERROR: Value \'" << value << "\' in square \'"
              << (char)toupper(location[0]) << location[1] 
              << "\' is invalid" << endl;
         cout << endl;     
         return;
      }
      //cout << endl;
      //input value and location
      modifyBoard(board, row, col, value);
   }
   cout << endl;
   //input value and location
   //modifyBoard(board, row, col, value);

}


/***************************************************************
 * This function modifies the value into the board.
 ***************************************************************/
void modifyBoard( int board[][9] ,int row, int col, int value)
{
   // input the value to the location
   board[row][col] = value;     
}

/***************************************************************
 * This function shows the possible values of a certain square.
 ***************************************************************/
void showPossibleValue(int board[][9])
{
   char location[2];
   int value[9] = {1,2,3,4,5,6,7,8,9};
   cout << "What are the coordinates of the square: ";
   cin >> location;
   
   int col = toupper(location[0]) - 'A';
   //cout << col << " ";
   
   int row = location[1] - '1';
   //cout << row << endl;

   bool isFirst = true;
   cout << "The possible values for " << "'" 
        << (char)toupper(location[0]) << location[1] << "' are:"; 
   
   computeValue( row , col , board, value);
   
   for (int l = 0; l < 9 ; l++)
   {
      if (value[l] != '\0' ) 
      {
         if (isFirst)  
         {
            cout << " " << value[l] ;
            isFirst = false;
         }    
         else
            cout << ", " << value[l] ;
      }
   }
   cout << endl;
   cout << endl;
}

/***************************************************************
 * This function checks if the value is valid.
 ***************************************************************/
bool isValueValid( int row, int col, int board[][9], int value)
{
   int values[9] = {1,2,3,4,5,6,7,8,9};
   computeValue(row, col, board, values);
   if ( values[value - 1] == 0 )
      return false;
   else 
      return true;          
}


/***************************************************************
 * This function gets the possible values.
 ***************************************************************/
void computeValue(int row, int col, int board[][9], int possibleValue[])
{
   // step 0 - initializes the pass array

   // Step 1 - processes the row
   for (int c = 0; c < 9; c++ )
   {
      //cout << board[row][c] <<endl;
      for (int k = 0; k < 9; k++)
      {   
         if ( board[row][c] == possibleValue[k] )
         {
            possibleValue[k] = '\0';
      
            // cout << board[row][c] <<endl;
            // cout << possibleValue[k] << endl;
            // cout << endl; 
         }
      }
   }

   // Step 2 - processes the column
   for (int r = 0 ; r < 9 ; r++ )
   {
      for (int n = 0; n < 9; n++)
      {
         if ( board[r][col] == possibleValue[n] )
         {
            possibleValue[n] = '\0';
            //cout << board[r][col];
         }
      }
   }

   // step 3 - processes the small square
   int boxRow = (row / 3) * 3;
   int boxCol = (col / 3) * 3;

   for (int i = boxRow ; i < boxRow + 3; i++)
   {
      for (int j = boxCol ; j < boxCol + 3 ; j++)
      {
         for (int m = 0 ; m < 9 ; m++ )
            if ( board[i][j] == possibleValue[m])
            {
               possibleValue[m] = '\0';
            }
      }  
   }

}











© 2022 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
