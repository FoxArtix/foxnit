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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/services.h"

void print_help() {
    printf("Foxnit is a runit wrapper, something similar to systemctl, rc-update, etc.\n\n");
    printf("Usage: foxnit [COMMAND] [OPTION] SERVICE_NAME\n");
    printf("Options:\n");
    printf("  -h, --help\t\t\tPrint this help message.\n");
    printf("  -v, --version\t\t\tPrint version.\n");
    printf("\nUsage:\n");
    printf("  foxnit \e[37;1mservices\e[0m\t\tList all services.\n");
    printf("  foxnit \e[37;1mservices\e[0m register\tRegister and run a service.\n");
    printf("  foxnit \e[37;1mservices\e[0m unregister\tUnregister and stop service.\n");
    printf("  foxnit \e[37;1mservices\e[0m start\t\tStart a service.\n");
    printf("  foxnit \e[37;1mservices\e[0m stop\t\tStop a service.\n");
    printf("  foxnit \e[37;1mservices\e[0m restart\tRestart a service.\n");
    printf("  foxnit \e[37;1mservices\e[0m status\tGet status of a service.\n");
}

void check_root() {
    if (getuid() != 0) {
        printf("You must be root to run foxnit.\n");
        exit(1);
    }
}

short nullcheck(void *ptr, int size) {
    unsigned char *ptr2 = (unsigned char*) ptr;
    for (int i = 0; i < size; i++) {
        if (ptr2[i]) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("No arguments provided. Use -h or --help for help.\n");
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("Foxnit is a runit wrapper, something similar to systemctl, rc-update, etc.\n\n");
        printf("Foxnit version 0.1.0\n");
        return 0;
    }

    if (strcmp(argv[1], "services") == 0) {
        if (argc == 2) {
            int i;
            printf("Listing services\n");
            RunitService *s = list_services();
            for (i = 0; !nullcheck(&s[i], sizeof(RunitService)); i++);
            printf("Found %d services\n================\n", i);
            for (int j = 0; j < i; j++) {
                printf("%s %s\n", s[j].name, s[j].running ? "(running)" : "");
                free(s[j].name);
            }
            printf("================\n");
            return 0;
        }

        if (strcmp(argv[2], "register") == 0) {
            if (argc == 4) {
                check_root();
                register_service(argv[3]);
                return 0;
            }
        }

        if (strcmp(argv[2], "unregister") == 0) {
            if (argc == 4) {
                check_root();
                unregister_service(argv[3]);
                return 0;
            }
        }

        if (strcmp(argv[2], "start") == 0) {
            if (argc == 4) {
                check_root();
                start_service(argv[3]);
                return 0;
            }
        }

        if (strcmp(argv[2], "stop") == 0) {
            if (argc == 4) {
                check_root();
                stop_service(argv[3]);
                return 0;
            }
        }

        if (strcmp(argv[2], "restart") == 0) {
            if (argc == 4) {
                check_root();
                restart_service(argv[3]);
                return 0;
            }
        }

        if (strcmp(argv[2], "status") == 0) {
            if (argc == 4) {
                check_root();
                status_service(argv[3]);
                return 0;
            }
        }
    }

    printf("Invalid arguments provided. Use -h or --help for help.\n");
    return 0;
}