#pragma once
#include <string>

using namespace std; 

class PropertyValue {
    string value; 
    string type;  
public:
    PropertyValue() = default;
    PropertyValue(int v) : value(to_string(v)), type("int") {}  
    PropertyValue(float v) : value(to_string(v)), type("float") {}
    PropertyValue(const string& v) : value(v), type("string") {}

    string getType() const { return type; }
    int asInt() const { return stoi(value); }     
    float asFloat() const { return stof(value); } 
    string asString() const { return value; }
};
