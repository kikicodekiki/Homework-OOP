#pragma once
#include <fstream>
#include "myString.h"
#include "PartialFunction.h"

class PartialFunctionFactory {
public :
    static PartialFunction* createFunctionFromFile (const MyString& fileName);
private:
    static PartialFunction* constructFromBinary (std::ifstream& ifs);

    static PartialFunction* firstCriteria (std::ifstream& ifs, int16_t N );
    static PartialFunction* secondCriteria (std::ifstream& ifs, int16_t N );
    static PartialFunction* thirdCriteria (std::ifstream& ifs, int16_t N );

   static PartialFunction* handleFourthCase (std::ifstream& ifs, int16_t N);
   static PartialFunction* handleFifthCase (std::ifstream& ifs, int16_t N);
};
