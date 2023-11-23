#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void print_file_info(const char *filename);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Sử dụng: %s <tên tệp hoặc thư mục> ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        printf("Thông tin cho: %s\n", argv[i]);
        print_file_info(argv[i]);
        printf("\n");
    }

    return 0;
}

void print_file_info(const char *filename) {
    struct stat fileStat;

    if (stat(filename, &fileStat) < 0) {
        perror("Lỗi");
        return;
    }

    printf("Tệp/Thư mục: %s\n", filename);
    printf("Loại Tệp: ");
    if (S_ISREG(fileStat.st_mode)) {
        printf("Tệp thường\n");
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("Thư mục\n");
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("Liên kết tượng trưng\n");
    } else {
        printf("Khác\n");
    }

    printf("Số liên kết: %ld\n", (long)fileStat.st_nlink);

    struct passwd *pwd = getpwuid(fileStat.st_uid);
    struct group *grp = getgrgid(fileStat.st_gid);

    if (pwd != NULL) {
        printf("Chủ sở hữu: %s\n", pwd->pw_name);
    } else {
        printf("Chủ sở hữu: %d\n", fileStat.st_uid);
    }

    if (grp != NULL) {
        printf("Nhóm: %s\n", grp->gr_name);
    } else {
        printf("Nhóm: %d\n", fileStat.st_gid);
    }

    printf("Quyền truy cập: ");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("Thời gian truy cập lần cuối: %s", ctime(&fileStat.st_atime));
}
