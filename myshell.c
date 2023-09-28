#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>




void throwError(){
 char error_message[30] = "An error has occurred\n";
 write(STDOUT_FILENO, error_message, strlen(error_message));
}








char** parse(char* token) {
 char *subtoken;
 char *saveptr2, *saveptr3;
 int num_subtokens = 0;
 char token_copy[strlen(token)];
 strcpy(token_copy, token);
 subtoken = strtok_r(token, "\t ", &saveptr2);
 while(subtoken != NULL){
     num_subtokens++;
     subtoken = strtok_r(NULL, "\t ", &saveptr2);
 }
 char **subtoken_list = malloc(num_subtokens * sizeof(char*));
 subtoken = strtok_r(token_copy, "\t ", &saveptr3);
 int i = 0;
 while(subtoken != NULL){
     int subtoken_len = strlen(subtoken);
     subtoken_list[i] = malloc((subtoken_len) * sizeof(char));
     strcpy(subtoken_list[i++], subtoken);
     subtoken = strtok_r(NULL, "\t ", &saveptr3);
 }
 subtoken_list[num_subtokens] = NULL;
  return subtoken_list;
}








//return 0 if fail, 1 is success
int launch(char **args){


  pid_t pid = fork();
 if(pid < 0){ //error
      //printf("error in pid < 0\n");
     throwError();
     return 0;
 }
 else if(pid == 0){ //child


       //printf("here 5!!");
       //printf("running execvp with: %s \n", args[0]);
     if (execvp(args[0], args) == -1) {
     
       //printf("error in pid == 0, %s\n", args[0]);
         throwError();
         exit(0);
     }
 }
 else { // parent
     wait(NULL);
 }
 return 1;
}




int cd(char** args){


  
  if (args[2] != NULL){
      //printf("here!\n");
      throwError();
  }
  else if (args[1] == NULL)
  {
      if (chdir(getenv("HOME")) == -1)
      {
          //printf("here!\n");
          throwError();
      }
  }
  else
  {
      if (chdir(args[1]) == -1)
      {
          //printf("here!\n");
          throwError();
      }
  }


  return 0;
}




void myPrint(char *msg)
{
 write(STDOUT_FILENO, msg, strlen(msg));
}




int main(int argc, char *argv[])
{
  char cmd_buff[514];
  char *pinput;
  int batchmode = 0;
  FILE *fp;
  if (argc > 2) {
       throwError();
      exit(1);
  }
  else if(argc == 2){
      batchmode = 1;
      fp = fopen(argv[1], "r");
      if (fp == NULL) {
          throwError();
          exit(1);
      }
  }
  while (1) {
      if(batchmode) {
          if(fgets(cmd_buff, 1010, fp) == NULL) { //check the length value
              break;
          }
           //if(strlen(cmd_buff) > 514){
          //     cmd_buff[513] = '\0';
          //}
         
           if(strlen(cmd_buff) > 514){
               myPrint(cmd_buff);
               //printf("here 1!!\n");
               cmd_buff[513] = '\0';
               throwError();
               continue;
          }


          else {
           //printf("{%s}", cmd_buff);
           int is_all_whitespace = 1;
           int i = 0;
           while(cmd_buff[i] != '\0'){
               if(!(cmd_buff[i] == 32 || cmd_buff[i] == '\t' || cmd_buff[i] == '\v' || cmd_buff[i] == '\n')){
                   //printf("its not all whitespace\n");
                   is_all_whitespace = 0;
                   break;
               }
               i++;
           }
           if (!is_all_whitespace){
               //printf("here2!!!, cmd_buff is -%c-\n", cmd_buff[1]);
               myPrint(cmd_buff);
           }
          


        
          //myPrint("\n");
          }
      }
      else{
          myPrint("myshell> ");
          pinput = fgets(cmd_buff, 1000, stdin);
          if(pinput == NULL){
              throwError();
          }
          if(strlen(cmd_buff) > 514){
           myPrint(cmd_buff);
           //printf("here 1!!\n");
           cmd_buff[513] = '\0';
           throwError();
           continue;
          }
          //printf("cmd buff len is %ld\n", strlen(cmd_buff));
          //if (cmd_buff[strlen(cmd_buff) - 1] != '\n') {
           //printf("was too long\n");
           //myPrint(cmd_buff);
           //throwError();
          // continue;
          //}


      }
   
  cmd_buff[strlen(cmd_buff) - 1] = '\0';
  char *token;
  char *saveptr1;
     /* get the first token */
     token = strtok_r(cmd_buff, ";", &saveptr1);
     //printf("token is: %s\n", token);
     /* walk through other tokens */
     while( token != NULL ) {


           if(token[strlen(token)] == '\n'){
               token[strlen(token)] = '\0';
           }


           //deal with tokens that are just blanks
              
           int is_all_whitespace = 1;
           int i = 0;
           while(token[i] != '\0'){
               if(!(token[i] == 32 || token[i] == '\t' || token[i] == '\v')){
                   is_all_whitespace = 0;
                   break;
               }
               i++;
           }
           if(is_all_whitespace){
               //printf("made it here\n");
               token = strtok_r(NULL, ";", &saveptr1);
               continue;
           }
          
           //printf("the token is: <%s>\n", token);
            //printf("token is -%d-: \n", token[1]);
             // printf("strlen token is -%ld-: \n", strlen(token));
         char** subtoken_list = parse(token);
        //printf("subtoken_list 0 is %s: \n", subtoken_list[0]);
         //int j = 0;
         //  printf("\n");
         //  while(subtoken_list[j] != NULL) {
                   //printf("here 4!!!");
         //      printf("- %s - \n", subtoken_list[j++]); //why print?
          // }


      //loop through subtoken list and skip the whole line
      int k = 0;
      int hasarrow = 0;
      while(subtoken_list[k] != NULL){
           int j = 0;
          
           while(subtoken_list[k][j] != '\0'){
               if(subtoken_list[k][j] == '>'){
                   hasarrow = 1;
                   break;
               }
               j++;
           }
           k++;
      }
      if(hasarrow){
           throwError();
           token = strtok_r(NULL, ";", &saveptr1);
           continue;


      }
      if (strcmp(subtoken_list[0], "exit") == 0) {
       if ((subtoken_list[1] != NULL)) {
           throwError();
          
       }
       else {
             exit(0);
       }
         }
       else if (strcmp(subtoken_list[0], "pwd") == 0) {
           if (subtoken_list[1] != NULL) {
               throwError();
           }
           else {
              //printf("in pwd statement\n");
           char cwd[514];
           if (getcwd(cwd, sizeof(cwd)) != NULL) {
               myPrint(cwd);
               myPrint("\n");
             }
           else {
                 throwError();
             }
           }
       }
       else if (strcmp(subtoken_list[0], "cd") == 0) {
           cd(subtoken_list);
         }
         //execvp(subtoken_list[0], subtoken_list);
         else if(launch(subtoken_list) == 0){
              //printf("problem is here!\n");
              //printf("launch sobtoken list error!\n");
             throwError();
         }
         free(subtoken_list);
         //printf("launch works!\n");
         token = strtok_r(NULL, ";", &saveptr1); // try with ";\n"
         
     }
     //printf("made it here!");
  
  
  }
 if(batchmode){
      fclose(fp);
 }
 return 0;
}










