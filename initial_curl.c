#include <stdio.h>
#include <curl/curl.h>
int main(void)
{
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://jovial-pasca-d1a6be.netlify.app/");
    res = curl_easy_perform(curl);
  }
  printf("\n");
  return 0;
}
