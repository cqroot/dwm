#include "autostart.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static void RunBlockAutoStartSh(const char *pathPrefix)
{
    const char shFile[] = "autostart_blocking.sh";
    char *path = ecalloc(1, strlen(pathPrefix) + strlen(shFile) + 2);
    if (sprintf(path, "%s/%s", pathPrefix, shFile) <= 0) {
        free(path);
        return;
    }

    if (access(path, X_OK) == 0) {
        system(path);
    }
    free(path);
}

static void RunAutoStartSh(const char *pathPrefix)
{
    const char shFile[] = "autostart.sh";
    char *path = ecalloc(1, strlen(pathPrefix) + strlen(shFile) + 2);
    if (sprintf(path, "%s/%s", pathPrefix, shFile) <= 0) {
        free(path);
        return;
    }

    if (access(path, X_OK) == 0) {
        system(strcat(path, " &"));
    }
    free(path);
}

void RunAutoStart(void)
{
    char *home = getenv("HOME");
    char *pathPrefix = ecalloc(1, strlen(home) + 4 + 2);

    if (sprintf(pathPrefix, "%s/.bin", home) < 0) {
        free(pathPrefix);
        return;
    }

    /* check if the autostart script directory exists */
    struct stat sb;
    if (!(stat(pathPrefix, &sb) == 0 && S_ISDIR(sb.st_mode))) {
        free(pathPrefix);
        return;
    }

    /* try the blocking script first */
    RunBlockAutoStartSh(pathPrefix);
    /* now the non-blocking script */
    RunAutoStartSh(pathPrefix);

    free(pathPrefix);
}
