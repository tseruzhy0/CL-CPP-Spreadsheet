/**
 * John Beres CIS 340 Assignment 2
 * Create a spreadsheet program that will can store values
 * and perform basic mathamatical operation on cell values
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

struct cell
{
    /**
     * Each cell is treated as a struct object
     * we will overload the setValue method based on whether
     * we are recieving a value from another cell or a constant numeric value
     */
	
	double getIsString()
	{
		return isString;
	}

	std::string getCellLabel()
	{
		return cellValue;
	}

	std::string getToString()
	{
		return toString;
	}

	void setValue(double x, std::string selectionType)
	{
		std::ostringstream os;
		std::string temp;

		value = x;
		next = this;
		mathOperator = ' ';

		os << x;
		temp = os.str();

		toString = selectionType + "\n" + temp;

		isString = -1;
	}

    void setValue(double x, std::string selectionType, std::string location)
    {
		std::ostringstream os;
		std::string temp;
        value = x;
        next = this;
        mathOperator = ' ';

		os << x;
		temp = os.str();

		toString = selectionType + "\n" + location + "\n" + temp;

		isString = -1;
    }

	void setValue(std::string x, std::string selectionType, std::string location)
	{
		cellValue = x;

		toString = selectionType + "\n" + location + "\n" + x;

		isString = 0;
	}

    void setValue(cell& x, std::string value, std::string selectionType, std::string location)
    {
        next = &x;
        mathOperator = ' ';

		toString = selectionType + "\n" + location + "\n" + value;

		isString = -1;
    }

    void setValue(const char* mOper, double vOne, double vTwo, std::string selectionType, std::string input)
    {
        mathOperator = *mOper;

        variableOne = vOne;
        ptrValOne = this;

        variableTwo = vTwo;
        ptrValTwo = this;

		toString = selectionType + "\n" + input;

		isString = -1;
    }

    void setValue(const char* mOper, cell& vOne, double vTwo, std::string selectionType, std::string input)
    {
        mathOperator = *mOper;

        ptrValOne = &vOne;

        variableTwo = vTwo;
        ptrValTwo = this;

		toString = selectionType + "\n" + input;

		isString = -1;
    }

    void setValue(const char* mOper, cell& vOne, cell& vTwo, std::string selectionType, std::string input)
    {
        mathOperator = *mOper;
        ptrValOne = &vOne;
        ptrValTwo = &vTwo;

		toString = selectionType + "\n" + input;

		isString = -1;
    }

    void setValue(const char* mOper, double vOne, cell& vTwo, std::string selectionType, std::string input)
    {
        mathOperator = *mOper;

        variableOne = vOne;
        ptrValOne = this;

        ptrValTwo = &vTwo;

		toString = selectionType + "\n" + input;

		isString = -1;
    }

    /**
     * The get variable methods will check to see if the operands
     * are refrences to another cell's value or a numeric value
     */
    double getVariableOne()
    {
        double temp;

        if ( this == ptrValOne )
        {
            return variableOne;
        }
        else
        {
            temp = (*ptrValOne).getValue();

            return temp;
        }
    }

    double getVariableTwo()
    {
        double temp;

        if ( this == ptrValTwo )
        {
            return variableTwo;
        }
        else
        {
            temp = (*ptrValTwo).getValue();

            return temp;
        }
    }
    /**
     * Each time this method is called it will recalculate the value
     * of the cell
     */
    double getValue()
    {
        switch(mathOperator)
        {
            case '+' :
                value = getVariableOne() + getVariableTwo();
                break;              
            case '-' :
                value = getVariableOne() - getVariableTwo();
                break;
                
            case '*' :
                value = getVariableOne() * getVariableTwo();
                break;              

            case 'x' :
                value = getVariableOne() * getVariableTwo();
                break;              

            case '/' :
                if ( 0 == getVariableTwo() )
                {
                    std::cout << "| div0";
                    break;
                }
                else
                {
                    value = getVariableOne() / getVariableTwo();
                }

                break; 

            default :
                if ( this == next)
                {
                    break;
                }
                else
                {
                    value = (*next).getValue();
                }
                break;             
        }

        return value;
    }

    private:
        cell *ptrValOne;
        cell *ptrValTwo;
        cell *next;
        double value;
        double variableOne, variableTwo;
        char mathOperator;
		int isString;
		std::string cellValue;
		std::string cellEntryType;
		std::string toString;
};

    
cell spreadsheet[10][10];
void initializeSpreadsheet();
bool isNumber(const char* x);
void getCellLocation(std::string input);
void setCellValue(std::string x, double value, std::string selectionType);
void setCellLabel(std::string x, std::string value, std::string selectionType);
void calculateExpression(std::string input, std::string selectionType);
void print();
void open(std::string file);
void save(std::string file);
int tempStorageLocation[2];

int main()
{
    /**
        * The main method will run a basic menu selection and call the various
        * functions needed to add data to the spreadsheet
    */

    std::string menuSelection = "";
    double value;
	std::string label;
    std::string input;

    initializeSpreadsheet();
    print();

    while(menuSelection != "q")
    {
        std::cout << "Please choose an option: Print (p), Save (s), Open (o), Data Entry (d), Quit (q): " << std::endl;
        std::cin >> menuSelection;

        if ( "q" == menuSelection )
        {
            return 0;
        }
        else if ( "d" == menuSelection )
        {
            std::cout << "Please choose and option:Enter data (ed), Enter Formula (ef), Enter Label (el): " << std::endl;
            std::cin >> menuSelection;

            if( "ed" == menuSelection )
            {
                std::cout << "Please Enter a Cell designater ie. A1: " << std::endl;
                std::cin >> input;

                std::cout << "Please Enter a Numeric Value: " << std::endl;
                std::cin >> value;

                setCellValue(input, value, "ed");

            }
            else if ( "ef" == menuSelection )
            {
                std::cout << "Please enter an expression in the format of: Destination Operator Cell/Number, Cell/Number: " << std::endl;;
                std::cin.ignore();
                getline(std::cin, input);
                
                calculateExpression(input, "ef");
            }
			else if ( "el" == menuSelection )
			{
				std::cout << "Please Enter a Cell designater ie. A1: " << std::endl;
                std::cin >> input;

                std::cout << "Please Enter a Cell Label: " << std::endl;
                std::cin >> label;

                setCellLabel(input, label, "el");

            }

        }
		else if ( "s" == menuSelection )
        {
            std::cout << "Please enter a file name to save data" << std::endl;;
            std::cin.ignore();
            getline(std::cin, input);

			save(input);
        }
		else if ( "o" == menuSelection )
        {
            std::cout << "Please enter a file name to load data" << std::endl;;
            std::cin.ignore();
            getline(std::cin, input);

			open(input);
        }
        else if ( "p" == menuSelection )
        {
            print();
        }
    }

    return 0;
}

void initializeSpreadsheet()
{
    /**
     * Initialize the spreadsheet values to zero
     */
    for ( int i = 0; i < 10; ++i )
    {
        for ( int f = 0; f < 10; ++f )
        {
            spreadsheet[i][f].setValue(0.0, "init");
        }
    }
}

bool isNumber(const char* x)
{
    /**
     * Check to see if the a character is a number
     * by checking its ascii value
     */
    if ( !( 47 < int(*x) && 58 > int(*x) ) )
    {
        return false;
    }

    return true;
}

void getCellLocation(std::string input)
{
    /**
     * Uses ascii values to determin if somthing is a 
     * number or letter then it will convert them to a 
     * the letters and number into the correspond integer
     * values
     */
    
    int x = 0;
    int y = 0;
    std::string::iterator pos;

    for ( pos = input.begin(); pos != input.end(); ++pos)
    {
        if ( 47 < int(*pos) && 58 > int(*pos) )
        {
            x = ( int(*pos) - 48);
        }
        else if ( 64 < int(*pos) && 91 > int(*pos) )
        {
            y = ( int(*pos) - 65);
        } 
        else if ( 96 < int(*pos) && 123 > int(*pos) )
        {
            y = ( int(*pos) - 97);
        }
    }

    tempStorageLocation[0] = x;
    tempStorageLocation[1] = y;
    
}

void setCellLabel(std::string x, std::string value, std::string selectionType)
{
	int xy[2];
    
    getCellLocation(x);
    xy[0] = tempStorageLocation[0];
    xy[1] = tempStorageLocation[1];

    spreadsheet[xy[0]][xy[1]].setValue(value, selectionType, x);
}

void setCellValue(std::string x, double value, std::string selectionType)
{
    /**
     * Set a cell to a specific numeric value
     */
    int xy[2];
    
    getCellLocation(x);
    xy[0] = tempStorageLocation[0];
    xy[1] = tempStorageLocation[1];

    spreadsheet[xy[0]][xy[1]].setValue(value, selectionType, x);
}

void calculateExpression(std::string input, std::string selectionType)
{
    /**
     * Method will take an entire expression, tokenize it
     * then assign the destination cell the value based on 
     * operator and operands
     */
    const std::string delims(" \t,;");
    int destination[2], valOneDest[2], valTwoDest[2];
    double valueOne, valueTwo;
    const char* mathOperator;
    std::vector<std::string> tokens;
    
    
    /**
     * Tokenize the input string and add each part of the expression 
     * into a vector of type string
     */
    std::string::size_type lastPos = input.find_first_not_of(delims, 0);
    std::string::size_type pos     = input.find_first_of(delims, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(input.substr(lastPos, pos - lastPos));
        lastPos = input.find_first_not_of(delims, pos);
        pos = input.find_first_of(delims, lastPos);
    }

    //get the destination cells array position 
    getCellLocation(tokens[0]);
    destination[0] = tempStorageLocation[0];
    destination[1] = tempStorageLocation[1];

    //store the math operator
    mathOperator = tokens[1].c_str();
    

    if( isNumber(tokens[2].c_str()) && isNumber(tokens[3].c_str()) )
    {
        //If both operands are numeric simply convert and pass the values
        valueOne = atof(tokens[2].c_str());
        valueTwo = atof(tokens[3].c_str());

        spreadsheet[destination[0]][destination[1]].setValue(mathOperator, valueOne, valueTwo, selectionType, input);
    }
    else if ( isNumber(tokens[2].c_str()) )
    {
        //if the first operand is a number then convert
        valueOne = atof(tokens[2].c_str());

        //Get the cell location for the second operand
        getCellLocation(tokens[3]);
        valTwoDest[0] = tempStorageLocation[0];
        valTwoDest[1] = tempStorageLocation[1];
 
        //pass the value and a refence of the second operands cell
        spreadsheet[destination[0]][destination[1]].setValue(mathOperator, valueOne, 
                *( *( spreadsheet + valTwoDest[0] ) + valTwoDest[1]), selectionType, input);
    }
    else if ( isNumber(tokens[3].c_str()) )
    {
        //if the first operand is not a number then get the cell location
        getCellLocation(tokens[2]);
        valOneDest[0] = tempStorageLocation[0];
        valOneDest[1] = tempStorageLocation[1];

        //convert the second operand to numeric
        valueTwo = atof(tokens[3].c_str());
        
        //pass the value and the refrence
        spreadsheet[destination[0]][destination[1]].setValue(mathOperator, 
                *( *( spreadsheet + valOneDest[0] ) + valOneDest[1]) , valueTwo, selectionType, input);
    }
    else
    {
        //Both operands are refrences to cells
        getCellLocation(tokens[2]);
        valOneDest[0] = tempStorageLocation[0];
        valOneDest[1] = tempStorageLocation[1];

        getCellLocation(tokens[3]);
        valTwoDest[0] = tempStorageLocation[0];
        valTwoDest[1] = tempStorageLocation[1];
 
        //pass both refrences
        spreadsheet[destination[0]][destination[1]].setValue(mathOperator, 
                *( *( spreadsheet + valOneDest[0] ) + valOneDest[1]), 
                *( *( spreadsheet + valTwoDest[0] ) + valTwoDest[1]), selectionType, input);
    }
}

void print()
{
    std::cout << std::endl;

    std::cout << std::setfill('-') << std::setw(93) << "-" << std::endl;

    //print the column designators
    std::cout << std::setfill(' ') << std::setw(4) << "| A" 
        << std::setw(9) << "| B" << std::setw(9) << "| C" 
        << std::setw(9) << "| D" << std::setw(9) << "| E" 
        << std::setw(9) << "| F" << std::setw(9) << "| G" 
        << std::setw(9) << "| H" << std::setw(9) << "| I" 
        << std::setw(9) << "| J" << std::setw(8) << "|" << std::endl;

    std::cout << std::setfill('-') << std::setw(93) << "-" << std::endl;
    

    for ( int i = 0; i < 10; ++i )
    {
        //print the column seperators
        std::cout << std::setfill(' ') << std::setw(2) << "|";
        for ( int k = 0; k < 9; ++k )
        {
            std::cout << std::setw(9) << "|";
        }
        std::cout << std::setw(10) << "|" << std::endl;

        //print the row number
        std::cout << i;
		
        //print the cell values recalculating any formulas
        for ( int f = 0; f < 10; ++f )
        {
			std::cout << std::setfill(' ') << "|" << std::setw(8);
			if(spreadsheet[i][f].getIsString() == 0)
			{
				std::cout << spreadsheet[i][f].getCellLabel();
			}
			else
			{
				std::cout << spreadsheet[i][f].getValue();
			}
        }
        std::cout << " |" << std::endl;

        //print column seperators
        std::cout << std::setw(2) << "|";
        for ( int k = 0; k < 9; ++k )
        {
            std::cout << std::setw(9) << "|";
        }
        std::cout << std::setw(10) << "|" << std::endl;

        std::cout << std::setfill('-') << std::setw(93) << "-" << std::endl;
        
    }
}

	void save(std::string file)
	{
		std::ofstream myfile;
		myfile.open(file);

		for ( int i = 0; i < 10; i++ )
		{
			for ( int k = 0; k < 10; k++ )
			{
				myfile << spreadsheet[i][k].getToString() << std::endl;
			}
		}
		myfile.close();
	}

void open(std::string file)
{
		std::ifstream myfile;
		myfile.open(file);
		double value;
		std::string input;
		std::string selectionType;
		std::string location;
		std::string cellLabel;
		std::string formula;
		std::string temp;

		initializeSpreadsheet();

		while(!myfile.eof())
		{
			std::getline(myfile, input);

			if(input == "init")
			{
				//do nothing
			}
			else if (input == "ed")
			{
				selectionType = input;

				std::getline(myfile, input);
				location = input;

				std::getline(myfile, input);
				value = atof(input.c_str());

				setCellValue(location, value, selectionType);
			}
			else if (input == "ef")
			{
				selectionType = input;

				std::getline(myfile, input);
				formula = input;

				calculateExpression(formula, selectionType);
			}
			else if(input == "el")
			{
				selectionType = input;

				std::getline(myfile, input);
				location = input;

				std::getline(myfile, input);
				cellLabel = input;

				setCellLabel(location, cellLabel, selectionType);
			}
		}
	}



    




















