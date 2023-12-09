#pragma once
#include <string>
#include "token.h"

tokens parseNextLine(std::istream& stream,const std::string& forbidden = "\t");
