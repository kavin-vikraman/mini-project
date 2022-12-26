#include <stdio.h>
#include <curl/curl.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
// pthread_mutex_t mutx;

//Give Your Requirements in below two lines
char* URL_list[] = {"https://www.google.com/","https://github.com/","https://codeforces.com/"};
int depth_final = 3;
//Rst of the Programs

int count =1;
const char** crawled;
int temp=0;
int * crawl_ind = &temp;
const int usize = sizeof(URL_list)/sizeof(URL_list[0]);
const char* file_name = "HTML.txt";
int depth[20] ={0};

int check( char* URL){
  int flag =0;
  for(int i=0;i<*crawl_ind;i++){
    if(strcmp(URL, crawled[i]) == 0) 
    {
      flag=1;
      break;
    }  
  }
  return flag;
}
void get_page( char* URL){
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
char** extract_hlinks( int *lin){
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
  char** links = malloc(sizeof(char*)*1000);
  for(int i=0; i<1000; i++) {
    links[i] = (char*)malloc(sizeof(char)*1000);
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
  char** final_list = malloc(sizeof(char*)*1000);
  for(int i=0; i<1000; i++) {
    final_list[i] = (char*)malloc(sizeof(char)*1000);
  }
  regex_t pattern;
  int value = regcomp( &pattern, "^https://[a-z].*com" , 0);
  
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
void *dfs_crawler( int pargs){
  long ind= (long )pargs; 
  if(depth[ind]==(depth_final+1) || check(URL_list[ind])) return NULL; 
  
  pthread_t thread[100];
  char** links;
  crawled[(*crawl_ind)++] = URL_list[ind];
  // pthread_mutex_lock(&mutx);  
  printf("%d. \t  Depth: %d\t Thread: %ld\t Link: %s\n",count++,depth[ind],ind+1,URL_list[ind]);
  get_page(URL_list[ind]);
  int lind=0;
  links = extract_hlinks(&lind);
  char ** final_list;
  int find=0;
  // printf("%d\n",lind);
  if(lind>0)
  final_list = validate_links(links,&lind,&find);
  else
  return NULL;

  // pthread_mutex_destroy(&mutx); 
  for(int i=0;i<find;i++){
    // sleep(0.01);
    URL_list[ind] = final_list[i];
    if(depth[ind]+1<=depth_final)
    {
        depth[ind]+=1;
        dfs_crawler(ind);
        depth[ind]-=1;
    }
  }
}
int main(void)
{
  int depth=0; 
  pthread_t thread[usize];
  crawled = (const char**)malloc(sizeof(char*)*1000);
  for(int i=0; i<1000; i++) {
    crawled[i] = (const char*)malloc(sizeof(char*)*1000);
  }
  for(int i=0;i<usize;i++){
    dfs_crawler(i);
  }
  return 0;
}
