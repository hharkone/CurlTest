#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

//global constants specific to your THETA. Change for your camera.
const string THETA_ID = "THETAYL00135573";
const string THETA_PASSWORD = "password";
const char* THETA_URL = "http://192.168.137.118:80/osc/";

string responseData;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    for (int c = 0; c<size*nmemb; c++)
    {
        responseData.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

CURLcode postJSON(CURL* curl, char* jsonString)
{
    CURLcode res;

    struct curl_slist *headers = NULL;
    curl_slist_append(headers, "Accept: application/json");
    curl_slist_append(headers, "Content-Type: application/json");
    curl_slist_append(headers, "charsets: utf-8");

    curl_easy_setopt(curl, CURLOPT_URL, THETA_URL);
    curl_easy_setopt(curl, CURLOPT_USERPWD, "THETAYL00135573:password");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    res = curl_easy_perform(curl);

    cout << "Response:" << endl << responseData << endl;
    cin.get();

    curl_easy_cleanup(curl);

    return res;
}

int main(int argc, char* argv[])
{
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl == NULL)
    {
        return 128;
    }
    
    char* jsonStartSession = "{ \"name\" : \"camera.startSession\", \"parameters\": {} }";
    char* jsonPreview      = "{ \"name\" : \"camera._getLivePreview\", \"parameters\": { \"sessionId: 1 } }";
    res = postJSON(curl, jsonStartSession);
    //res = postJSON(curl, jsonPreview);

    //curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    //curl_easy_perform(curl);

    curl_global_cleanup();

    SYSTEM("PAUSE");

    return 0;  
}
