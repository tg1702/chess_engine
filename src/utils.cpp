#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include "types.h"
#include "utils.h"

   
	

namespace utils{
    std::vector<std::string> split_string(std::string str){
	
		std::stringstream ss(str);
		std::istream_iterator<std::string> begin(ss), end;
		return std::vector<std::string> (begin, end);

	}

    
};