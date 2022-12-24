#include <stdio.h>
#include <curl/curl.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char * strcases(char * haystack, char *needle)
{
  while (*haystack) {
    char * ha = haystack;
    char * ne = needle;
    while (tolower(*ha) == tolower(*ne)) {
      if (!*++ne)
        return haystack;
      ha += 1;
    }
    haystack += 1;
  }
  return NULL;
}

int main(void)
{
  int n=1000;
  int a=10;
  const char* file_name = "HTML.txt";
  // const char* URL = "https://www.google.com/";
  const char* URL = "https://incomparable-melomakarona-cf76f6.netlify.app/";
  CURL* curl;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, URL);
  FILE* file = fopen(file_name, "w");
  curl_easy_setopt( curl, CURLOPT_WRITEDATA, file);
  curl_easy_perform( curl );
  curl_easy_cleanup( curl );
  fclose(file);

  // To get file size, I used fseek() and ftell().
  FILE* leng = fopen(file_name , "r");
  fseek(leng, 0L, SEEK_END);
  long int res = ftell(leng);
  fclose(leng);



  FILE* read = fopen(file_name , "r");
  char* str = (char *)malloc(sizeof(char*)*res);
  int ind=0;
  char c= getc(read);
  while(c != EOF){
    str[ind++] = c;
    c= getc(read);
  }
  str[ind] = '\0';
  fclose(read);


  char *html = str;
  char * begin = html;
  char * end;
  
  char** links = malloc(sizeof(char*)*13);
  for(  int i=0; i<13; i++ ) {
    links[i] = (char*)malloc(sizeof(char)*100);
  }
  
  int size = sizeof(char*);
  int lind=0;
  while ((begin = strcases(begin, "<a href=\"")) != NULL) {
    begin += 9; 
    end = strchr(begin, '"');
    char* st = (char *)malloc(sizeof(char*)*((int)(end-begin))*10);
    int ind=0;
    if (end != NULL) {
      sprintf(st,"'%.*s'\n", (int) (end - begin), begin);
      strcpy(links[lind++],st);
      begin = end + 1;
    }
    else {
      puts("invalid url");
      return -1;
    }
    free(st);
  }
  for(int i=0;i<lind;i++){
    printf("%s\n",links[i]); //Print all Hyperlinks in the given URL.
  }
  free(str);
  return 0;
}
