//
// Netstrings.cpp
// Netstrings
//
//  Created by Jason Kuan on 29/04/15.
//  Copyright (c) 2015 Jason Kuan. All rights reserved.
//

#include <iostream>
#include <string>
using namespace::std;

string encodeNetstring(string payload) {
    unsigned long len = payload.length();
    if(!len) return "ERROR_EMPTY_STRING";
    string s = to_string(len);
    string netstring = s + ":" + payload + ",";
    
    return netstring;
}

string decodeNetstring(string netstring) {
    if (netstring.length() < 3) return "NETSTRING_ERROR_TOO_SHORT";
    
    //if (netstring.length() > ?) return "NETSTRING_ERROR_TOO_LONG";
    
    long semicolonIndex = netstring.find(':');
    if (semicolonIndex < 0) return "NETSTRING_ERROR_NO_COLON";
    
    string getLength = netstring.substr(0, semicolonIndex);
    int payloadLength = atoi(getLength.c_str());            //Convert string to int
    if (payloadLength < 1) return "NETSTRING_ERROR_LEADING_ZERO";
    
    string payload = netstring.substr(semicolonIndex+1);
    if (!payload.length()) return "NETSTRING_ERROR_NO_LENGTH";
    
    if (payload.substr(payload.length() -1) == ",") payload.erase(payload.length()-1); //remove the comma
    if (payload.length() != payloadLength) return "NETSTRING_ERROR_INVALID_LENGTH";
    
    return payload;
}
int main(int argc, const char * argv[]) {

    string payload = "what will this do?";
    string netstring = encodeNetstring(payload);
    
    cout << encodeNetstring(payload) << endl;
    cout << decodeNetstring(netstring) << endl; 
    
    return 0;
    
}
