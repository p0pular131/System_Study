#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <locale.h>

#define BUFSIZE 2048

// 문자열 비교 함수
int stringcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int stringlen(char *str){
    int cnt=0;
    while(str[cnt]!='\0'){
        ++cnt;
    }
    return cnt;
}

char *stringtok(char *str, char *delim) {
    static char *tmp = NULL;
    int i, delim_len = stringlen(delim);

    if (str != NULL) {
        tmp = str;
    }
    if (tmp == NULL) {
        return NULL;
    }

    while (*tmp != '\0') {
        for (i = 0; i < delim_len; i++) {
            if (*tmp == delim[i]) {
                tmp++;
                break;
            }
        }
        if (i == delim_len) {
            break;
        }
    }

    if (*tmp == '\0') {
        tmp = NULL;
        return NULL;
    }

    char *start = tmp;

    while (*tmp != '\0') {
        for (i = 0; i < delim_len; i++) {
            if (*tmp == delim[i]) {
                *tmp = '\0'; 
                tmp++;       
                return start;
            }
        }
        tmp++;
    }

    // reset tmp and return last token
    tmp = NULL;
    return start;
}

int ls(char *dir_path, char *option);
int head(char *file_path, char *line);
int tail(char *file_path, char *line);
int mv(char *file_path1, char *file_path2);
int cp(char *file_path1, char *file_path2);
int pwd();

int main() {
	while(1){
		int i, cmdrlt;
		char cmd[100];
		char *argument[10];

        /* Input commands */
		scanf(" %[^\n]", cmd);

        /* Tokenize commands */
		int idx = 0;
		char *ptr = stringtok(cmd, " ");
		while(ptr != NULL){
			argument[idx++] = ptr;
			ptr = stringtok(NULL, " ");
		}
		argument[idx] = NULL;

        /* Command */
		if (stringcmp("ls", argument[0]) == 0){
			cmdrlt = ls(argument[1], argument[2]);		
		}
		else if (stringcmp("head", argument[0]) == 0) {
			if (argument[1] == NULL || argument[2] == NULL || argument[3] == NULL || stringcmp("-n", argument[1]) != 0) {
				cmdrlt = -1;
			} else {
				cmdrlt = head(argument[3], argument[2]);
			}
		}
		else if (stringcmp("tail", argument[0]) == 0) {
			if (argument[1] == NULL || argument[2] == NULL || argument[3] == NULL || stringcmp("-n", argument[1]) != 0) {
				cmdrlt = -1;
			} else {
				cmdrlt = tail(argument[3], argument[2]);
			}
		}
		else if (stringcmp("mv", argument[0]) == 0){
			cmdrlt = mv(argument[1], argument[2]);
		}
		else if (stringcmp("cp", argument[0]) == 0){
			cmdrlt = cp(argument[1], argument[2]);
		}
		else if (stringcmp("pwd", argument[0]) == 0){
			cmdrlt = pwd();
		}
		else if (stringcmp("quit", argument[0]) == 0){
			break;
		}
		else{
			printf("ERROR: invalid command\n");
		}

		if (cmdrlt == -1){
			printf("ERROR: The command is executed abnormally\n");
		}
		printf("\n");
	}
	return 0;
}

int ls(char *dir_path, char *option) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path);
    struct stat fileStat;
    char timebuf[80];
    char *namebuff[BUFSIZE];
    int first = 1;
    int file_count = 0;
    int total_blocks = 0;

    if (!dp) {
        printf("ERROR: invalid path\n");
        return -2;
    }

    while ((entry = readdir(dp)) != NULL) {
        int length = 0;
        while (entry->d_name[length] != '\0') {
            length++;  
        }

        namebuff[file_count] = (char *)malloc(length + 1);
        for (int i = 0; i <= length; i++) {  
            namebuff[file_count][i] = entry->d_name[i];
        }
        file_count++;
    }
    closedir(dp);

    // sorting
    for (int i = 0; i < file_count - 1; i++) {
        for (int j = 0; j < file_count - i - 1; j++) {
            if (stringcmp(namebuff[j], namebuff[j + 1]) > 0) {
                char *temp = namebuff[j];
                namebuff[j] = namebuff[j + 1];
                namebuff[j + 1] = temp;
            }
        }
    }
    // print total 
    if (option != NULL && stringcmp(option, "-al") == 0) {
        for (int i = 0; i < file_count; i++) {
            char full_path[BUFSIZE];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, namebuff[i]);

            if (stat(full_path, &fileStat) == 0) {
                total_blocks += fileStat.st_blocks;
            }
        }
        printf("total %d\n", total_blocks / 2);  
    }

    for (int i = 0; i < file_count; i++) {
        if (option != NULL && stringcmp(option, "-al") == 0) {
            char full_path[BUFSIZE];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, namebuff[i]);

            if (stat(full_path, &fileStat) < 0) return -1;

            printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x " : "- ");

            printf("%ld ", fileStat.st_nlink);

            struct passwd *pwd;
            struct group *grp;
            pwd = getpwuid(getuid());
            grp = getgrgid(fileStat.st_gid);
            printf("%s %s ", pwd->pw_name, grp->gr_name);
            
            printf("%4ld ", fileStat.st_size);

            strftime(timebuf, sizeof(timebuf), "%m월 %d %H:%M", localtime(&fileStat.st_mtime));
            printf("%s ", timebuf);

            printf("%s\n", namebuff[i]);
        } 
        else {
            if(stringcmp(namebuff[i], ".") == 0 || stringcmp(namebuff[i], "..") == 0) 
                continue;
            printf("%s ", namebuff[i]);
        }
        free(namebuff[i]); 
    }
    return 0;
}


int head(char *file_path, char *line) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: invalid path\n");
        return -2;
    }

    char buffer[BUFSIZE];
    int n = atoi(line) - 1;
    int bytes;
    int line_count = 0;
    while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        for (int i = 0; i < bytes; i++) {
            if (buffer[i] == '\n') line_count++;
            if (line_count > n) break;
			printf("%c", buffer[i]);
        }
        if (line_count >= n) break;
    }
    printf("\n");
    close(fd);
    return 0;
}

int tail(char *file_path, char *line) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: invalid path\n");
        return -2;
    }

    int n = atoi(line);
    off_t file_size = lseek(fd, 0, SEEK_END);
    off_t offset = file_size - 1;
    int line_count = 0;

    while (offset >= 0) {
        lseek(fd, offset, SEEK_SET);
        char c;
        if (read(fd, &c, 1) != 1) break;
        
        if (c == '\n') {
            line_count++;
            if (line_count > n) break;
        }
        offset--;
    }
    
    offset = (offset < 0) ? 0 : offset + 1; 
    lseek(fd, offset, SEEK_SET);

    char buffer[BUFSIZE];
    int bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        buffer[bytes] = '\0';  
        printf("%s", buffer);
    }

    close(fd);
    return 0;
}

int mv(char *file_path1, char *file_path2) {
	if (access(file_path1, F_OK) == -1) {
		printf("ERROR: invalid path\n");
		return -2;
    }

    if (rename(file_path1, file_path2) == -1) return -1;
    return 0;
}

int cp(char *file_path1, char *file_path2) {
    int src_fd = open(file_path1, O_RDONLY);
    if (src_fd == -1) {
        printf("ERROR: invalid path\n");
        return -2;
    }

    int dest_fd = open(file_path2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        printf("ERROR: invalid path\n");
        return -2;
    }

    char buffer[BUFSIZE];
    int bytes;
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes) != bytes) {
            close(src_fd);
            close(dest_fd);
            return -1;
        }
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}

int pwd() {
    char cwd[200];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return -1;
    printf("%s\n", cwd);
    return 0;
}
