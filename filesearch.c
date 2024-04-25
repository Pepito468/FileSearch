#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define ROOT_FOLDER "/"
#define HOME_FOLDER "/home"

//buffer size
#define BUFFER_SIZE 1024

int folder_search(char *directory_name, char* file_to_find){
    //declaring structures
    struct dirent *directory_entry;
    DIR *directory; 

    char directory_name_buffer[BUFFER_SIZE];

    int file_count = 0;
    int recursive_file_count = 0;

    //opening the directory
    directory = opendir(directory_name);
    if (!directory){
        //printf("Invalid directory (or you don't have permissions to open it).\n");
        return 0;
    }

    //looking for file
    while ((directory_entry = readdir(directory))){
        //if the entry is a folder itself, check every file in that (calls to recursive function)
        if (directory_entry -> d_type == DT_DIR){
            //check if file is in the entry name
            if (strstr(directory_entry -> d_name, file_to_find) != NULL){
                printf("[D] %s found at location %s\n", directory_entry -> d_name, directory_name);
                file_count++;
            }
            //avoid . and ..
            if (strcmp(directory_entry -> d_name, ".") != 0 && strcmp(directory_entry -> d_name, "..") != 0){
                //prepare buffer to be sent to the recursive call
                //ignore first "/" symbol
                //if (strcmp(directory_name, "/") != 0)
                strncpy(directory_name_buffer, directory_name, BUFFER_SIZE-1);
                if (strcmp(directory_name, "/") != 0)
                    strncat(directory_name_buffer, "/", BUFFER_SIZE - strlen(directory_name_buffer) - 1);
                strncat(directory_name_buffer, directory_entry -> d_name, BUFFER_SIZE - strlen(directory_name_buffer)- 1);
                //if the buffer is full, skip the file to avoid an infinite truncating loop
                if (strlen(directory_name_buffer) == BUFFER_SIZE - 1){
                    printf("Name buffer full. Skipping a file...\n");
                    continue;
                }

                //recursive call
                recursive_file_count = folder_search(directory_name_buffer, file_to_find);

                //update number of files found
                file_count += recursive_file_count;
            }
        } else {
            //printf("[F] %s\n", directory_entry -> d_name);
            if (strstr(directory_entry -> d_name, file_to_find) != NULL){
                printf("[F] %s found at location %s\n", directory_entry -> d_name, directory_name);
                file_count++;
            }
    //file_count++;
    }
}
    //closing directory
    closedir(directory);

    //returning the number of files
    return file_count;
}

int main(){

    int file_count = 0;
    int choice;
    char search_directory[BUFFER_SIZE];
    char file_to_find[BUFFER_SIZE];

    //printf("Welcome to FileSearch.\nInput the file to be searched trough your entire machine: ");
    printf("Welcome to FileSearch.\nInput the file to be searched: ");
    scanf("%63s", file_to_find);
    printf("[0] Search Root (\"/\")\n[1] Search your home\n[2] Custom Folder\nInput: ");
    scanf("%d", &choice);
    switch (choice){
        case 0:
            strncpy(search_directory, ROOT_FOLDER, BUFFER_SIZE-1);
            break;
        case 1:
            strncpy(search_directory, HOME_FOLDER, BUFFER_SIZE-1);
            break;
        case 2:
            printf("Enter the folder name (enter the entire path like '/home/user/something'): ");
            scanf("%s", search_directory);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
    //call to function
    file_count = folder_search(search_directory, file_to_find);

    printf("Found %d files.\n", file_count);


    return 0;
}
