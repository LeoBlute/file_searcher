#include <stdio.h>
#include <string.h>
#include <windows.h>

static char global_target_string[256] = {0};

static char contains_target_string(char* str) {
   return strstr(str, global_target_string) != NULL;
}

static char is_undesirable_directory(const char* str) {
   return (strlen(str) == 1 && str[0] == '.') || (strlen(str) == 2 && str[1] == '.');
}

static const char r[2] = "\\";

static void append_two_path(char* buffer, const char* path1, const char* path2) {
   const int path1_len = strlen(path1);
   const int path2_len = strlen(path2);
   memcpy(buffer                , path1, path1_len);
   memcpy(buffer + path1_len + 2, path2, path2_len + 1);
   buffer[path1_len + 0] = r[0];
   buffer[path1_len + 1] = r[0];
}

int recursive_search(char* dir) {
   WIN32_FIND_DATA file_data;
   HANDLE find_handle;
   char path[4096] = {};

   append_two_path(path, dir, "*.*");
   if((find_handle = FindFirstFile(path, &file_data)) == INVALID_HANDLE_VALUE) {
      return 0;
   }

   do {
      append_two_path(path, dir, file_data.cFileName);

      if(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && !is_undesirable_directory(file_data.cFileName)) {
         recursive_search(path);
      } else if(contains_target_string(file_data.cFileName)) {
         printf("File: %s\n", path);
      }
   } while(FindNextFile(find_handle, &file_data));

   FindClose(find_handle);

   return 1;
}

int main(int argc, char *argv[])
{
   if(argc < 2) {
      printf("File Argument not provided\n");
      return 1;
   }
   strcpy(global_target_string, argv[1]);

   recursive_search(".");

   return 0;
}

