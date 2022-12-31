/*
Foxnit - Wrapper for runit init system
Copyright (C) 2022 Ján Gajdoš 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "services.h"
#include "../config.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

static int sort_algo(const void *a, const void *b) {
    return strcmp(*(const char**) a, *(const char**) b);
}

RunitService* list_services() {
    char **names = (char**) malloc(sizeof(char*));
    int count = 0;
    int real_count = 0;

    DIR *dir;
    struct dirent *ent;
    dir = opendir(servicesPATH);
    if (dir) {
        while ((ent = readdir(dir))) {
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
                continue;
            }
            names[count] = (char*) malloc(strlen(ent->d_name) + 1);
            strcpy(names[count], ent->d_name);
            count++;
            names = realloc(names, sizeof(char*) * (count + 1));
        }
        closedir(dir);
    }

    real_count = count;
    
    dir = opendir(runningServicesPATH);
    if (dir) {
        while ((ent = readdir(dir))) {
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
                continue;
            }
            names[count] = (char*) malloc(strlen(ent->d_name) + 1);
            strcpy(names[count], ent->d_name);
            count++;
            names = realloc(names, sizeof(char*) * (count + 1));
        }
        closedir(dir);
    }
    
    qsort(names, count, sizeof(char*), sort_algo);
    RunitService *services = malloc(sizeof(RunitService) * (real_count + 1));
    for (int i = 0, j = 0; i < count && j < real_count; i++, j++) {
        if (i + 1 < count) {
            if (!strcmp(names[i], names[i + 1])) {
                services[j].name = names[i];
                services[j].running = 1;
                i++;
                continue;
            }
        }
        services[j].name = names[i];
        services[j].running = 0;
    }
    memset(&services[real_count], 0, sizeof(RunitService));

    return services;
}

void register_service(char *name) {
    char *base = (char*) malloc(strlen(servicesPATH) + strlen(name) + 1);
    strcpy(base, servicesPATH);
    strcat(base, name);
    char *target = (char*) malloc(strlen(runningServicesPATH) + strlen(name) + 1);
    strcpy(target, runningServicesPATH);
    strcat(target, name);

    printf("Registering service %s\n", name);
    int ret = symlink(base, target);
    if (ret == -1) {
        printf("Failed to register service %s\n", name);
        printf("Error: %s\n", strerror(errno));
    } else {
        printf("Done\n");
    }

    free(base);
    free(target);
}

void unregister_service(char *name) {
    printf("Unregistering service %s\n", name);

    char *base = (char*) malloc(strlen(runningServicesPATH) + strlen(name) + 1);
    strcpy(base, runningServicesPATH);
    strcat(base, name);
    int ret = unlink(base);

    if (ret == -1) {
        printf("Failed to unregister service %s\n", name);
        printf("Error: %s\n", strerror(errno));
    } else {
        printf("Done\n");
    }

    free(base);
}

void start_service(char *name) {
    printf("Starting service %s\n", name);

    char *cmd = (char*) malloc(strlen("sv up ") + strlen(name) + 1);
    strcpy(cmd, "sv up ");
    strcat(cmd, name);

    system(cmd);

    free(cmd);
    printf("Done\n");
}

void stop_service(char *name) {
    printf("Stopping service %s\n", name);

    char *cmd = (char*) malloc(strlen("sv down ") + strlen(name) + 1);
    strcpy(cmd, "sv down ");
    strcat(cmd, name);

    system(cmd);

    free(cmd);
    printf("Done\n");
}

void restart_service(char *name) {
    printf("Restarting service %s\n", name);

    char *cmd = (char*) malloc(strlen("sv restart ") + strlen(name) + 1);
    strcpy(cmd, "sv restart ");
    strcat(cmd, name);

    system(cmd);

    free(cmd);
    printf("Done\n");
}

void status_service(char *name) {
    printf("Status of service %s\n", name);

    char *cmd = (char*) malloc(strlen("sv status ") + strlen(name) + 1);
    strcpy(cmd, "sv status ");
    strcat(cmd, name);

    system(cmd);

    free(cmd);
    printf("Done\n");
}