#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    FILE *file;

    printf("Entering to Writer.c \n");
    printf("arg[0] %s \n",argv[0]);
    printf("arg[1] %s \n",argv[1]);
    printf("arg[2] %s \n",argv[2]);
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);

    if (argc != 3) {
        printf("Returning as I got less number of argument");
        syslog(LOG_ERR, "Usage: %s <file>\n", argv[1]);
        closelog();
        return 1;
    }

    file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Returning as file is NULL");
        syslog(LOG_ERR, "Error opening file '%s' for writing.\n", argv[1]);
        closelog();
        return 1;
    }

    syslog(LOG_DEBUG, "Opened file '%s' for writing.\n", argv[1]);

    fprintf(file, "%s\n", argv[2]);

    fclose(file);
    syslog(LOG_DEBUG, "Closed file '%s'.\n", argv[1]);

    printf("Text has been written to the file '%s'.\n", argv[1]);

    closelog();
    return 0;
}

