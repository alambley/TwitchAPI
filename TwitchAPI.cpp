//g++ -std=c++11 -o TwitchAPI TwitchAPI.cpp -lcurl

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h> 

using namespace std;

string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) { //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c < size * nmemb; c++) {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

string searchrstring(string input, string search) {
    string returnthis;
    std::size_t found = input.find(search);
    if (found != std::string::npos) {
        for (int counter = found + search.size(); counter < input.size(); counter++) {
            if (input[counter] == '"' || (input[counter] == ':' && input[counter + 1] != '/')) {
            } else if (input[counter] == ',' || input[counter] == '}') {
                return returnthis;
            } else {
                returnthis.push_back(input[counter]);
            }
        }
    }
    return "";
}

CURL* getstring(string link){
    CURL* curl; //our curl 
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    curl_easy_perform(curl);
    return curl;
    //curl_easy_cleanup(curl);
    // curl_global_cleanup();
}

int main(int argc, char** argv) {
    string streamer, viewerendpoint, viewerstring, chatterendpoint, chatterstring, uptimeendpoint, uptimestring;  
    if(argc != 2){
        cout << "Must supply name of streamer as command line argument.\n";
        return -1;
    }
    streamer = argv[1];
    chatterendpoint = "http://tmi.twitch.tv/group/user/" + streamer + "/chatters";
    viewerendpoint = "https://api.twitch.tv/kraken/streams/" + streamer;
    uptimestring = "https://nightdev.com/hosted/uptime.php?channel=" + streamer;
      
    CURL* curl;
    curl = getstring(viewerendpoint);
    viewerstring = data;
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    data = "";
    
    curl = getstring(chatterendpoint);
    chatterstring = data;
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    data = "";
    
    curl = getstring(uptimestring);
    uptimestring = data;
    curl_easy_cleanup(curl);
    curl_global_cleanup();
   
    viewerstring = searchrstring(viewerstring, "viewers");
    chatterstring =  searchrstring(chatterstring, "chatter_count");
            
    if (viewerstring.size() != 0) {
        cout << "Amount of " << streamer << " viewers: " << viewerstring << '\n';
        cout << "Amount of " << streamer << " chatters: " << chatterstring << '\n';
        cout << "Uptime: " << uptimestring;
    } else {
        cout << streamer << " is not streaming right now." << '\n';
    }

    return 0;
}