#pragma once

#include <map>
#include <string>
#include "IRbuilder.h"

// const static std::map<std::string, BinaryInst(*)(Value *, Value*)> binary_convert_table = {
//     {"+", BinaryInst::create_add},
// };

const static std::map<std::string, CmpInst*(Value *, Value*)> cmp_convert_table = {

};