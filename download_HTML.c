#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>

int main(){
    const char* file_name = "HTML.txt";
    const char* URL = "https://jovial-pasca-d1a6be.netlify.app/";
    CURL* curl;
    // CURLcode res;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, URL);
    FILE* file = fopen(file_name, "w");
    curl_easy_setopt( curl, CURLOPT_WRITEDATA, file);
    curl_easy_perform( curl);
    curl_easy_cleanup( curl);
    fclose(file);

    return 0;
}
