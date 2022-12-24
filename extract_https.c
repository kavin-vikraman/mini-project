#include <stdio.h>
#include <curl/curl.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void get_page(const char* file_name, const char* URL){
  CURL* curl;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, URL);
  FILE* file = fopen(file_name, "w");
  curl_easy_setopt( curl, CURLOPT_WRITEDATA, file);
  curl_easy_perform( curl );
  curl_easy_cleanup( curl );
  fclose(file);
}
char * strcasestr(char * haystack, char *needle)
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
char** extract_hlinks(const char* file_name, int *lin){
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
  char** links = malloc(sizeof(char*)*100);
  for(int i=0; i<100; i++) {
    links[i] = (char*)malloc(sizeof(char)*100);
  }
  // char* links[100];
  int size = sizeof(char*);
  while ((begin = strcasestr(begin, "<a href=\"")) != NULL) {
    begin += 9;
    end = strchr(begin, '"');
    char* st = (char *)malloc(sizeof(char*)*((int)(end-begin))*10);
    int ind=0;
    if (end != NULL) {
      sprintf(st,"'%.*s'", (int) (end - begin), begin);
      strcpy(links[(*lin)++],st);
      begin = end + 1;
    }
    free(st);
  }
  free(str);
  return links;
}
char** validate_links(char** links,int *lin,int *final_ind){
  char** final_list = malloc(sizeof(char*)*100);
  for(int i=0; i<100; i++) {
    final_list[i] = (char*)malloc(sizeof(char)*100);
  }
  regex_t pattern;
  int value = regcomp( &pattern, "^https://[a-z].*" , 0);
  // printf("%d\n",*lin);
  for(int i=0;i<(*lin);i++)
  {
    char* one_link = links[i];
    char* cleaned_list = malloc(sizeof(char*)*100);
    int clean_ind=0;
    for(int j=0;j<strlen(one_link);j++)
    {
      if(one_link[j] == ' ' || one_link[j] == '"' || one_link[j] == '\'') continue; 
      // printf("%c",one_link[j]);
      cleaned_list[clean_ind++] = one_link[j]; 
    }
    cleaned_list[clean_ind] = '\0';
    
    value = regexec( &pattern ,cleaned_list, 0, NULL, 0);
    if (!value) {
      final_list[(*final_ind)++] = cleaned_list;
      // printf("%s\n",cleaned_list);
    }
    // printf("%d\n",*final_ind);
  } 
  return final_list;
}

int main(void)
{
  int n=1000;
  int a=10;
  const char* file_name = "HTML.txt";
  const char* URL = "https://github.com/";
  char** links;
  get_page(file_name,URL);
  int lind=0;
  links = extract_hlinks(file_name,&lind);

  char ** final_list;
  int find=0;
  if(lind>0)
  final_list = validate_links(links,&lind,&find);
  else
  printf("The Given URL has No Links");

  for(int i=0;i<find;i++){
    printf("%s\n",final_list[i]); //list all https links in the website given>
  }
  return 0;
}
