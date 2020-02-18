//
//  main.c
//  Ajanvaraus_projekti
//
//  Created by Alarik Sutter on 02/04/2019.
//  Copyright © 2019 Alarik Sutter. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projekti.h"

int len = 0; //Global integer to define the amount of appointments in schedule.

//Add_appointments-function compares the date and time of the new appointment to the previous appointments in the list.
//If the date isn't in use the date will be added. After the appointment has been added, the list will be re-arranged according to the date.
Schedule* add_appointment(Schedule *a, char description[20], int month, int day, int time) {
    int b = 0;
    for (int i = 0; i < len; i++){
        int check = 0;
        if (a[i].Month == month)
            check++;
        if (a[i].Day == day)
            check++;
        if (a[i].Time == time)
            check++;
        if (check == 3) {
            b++;
        }
    }
    if (b != 0) {
        printf("Kyseinen ajankohta on jo varattu.\n");
        return a;
    }
    else {
        len++;
        a = realloc(a, (len + 1) * sizeof(Schedule));
        strcpy(a[len - 1].description, description);
        a[len - 1].Day = day;
        a[len - 1].Month = month;
        a[len - 1].Time = time;
        if (len > 1) {
            int c = 0;
            for (int b = 0; b < len; b++) {
                c++;
                if (month < a[len - c].Month) {
                    strcpy(a[len - b].description, a[len - c].description);
                    a[len - b].Month = a[len - c].Month;
                    a[len - b].Day = a[len - c].Day;
                    a[len - b].Time = a[len - c].Time;
                    strcpy(a[len - c].description, description);
                    a[len - c].Month = month;
                    a[len - c].Day = day;
                    a[len - c].Time = time;
                }
                else if (month == a[len - c].Month && day < a[len - c].Day) {
                    strcpy(a[len - b].description, a[len - c].description);
                    a[len - b].Month = a[len - c].Month;
                    a[len - b].Day = a[len - c].Day;
                    a[len - b].Time = a[len - c].Time;
                    strcpy(a[len - c].description, description);
                    a[len - c].Month = month;
                    a[len - c].Day = day;
                    a[len - c].Time = time;
                }
                else if (month <= a[len - c].Month && day <= a[len - c].Day && time < a[len - c].Time) {
                    strcpy(a[len - b].description, a[len - c].description);
                    a[len - b].Month = a[len - c].Month;
                    a[len - b].Day = a[len - c].Day;
                    a[len - b].Time = a[len - c].Time;
                    strcpy(a[len - c].description, description);
                    a[len - c].Month = month;
                    a[len - c].Day = day;
                    a[len - c].Time = time;
                }
            }
            printf("Tapahtuma lisätty aikatauluun!\n");
            return a;
        }
        else {
            printf("Tapahtuma lisätty aikatauluun!\n");
            return a;
        }
    }
}

//This function deletes the wanted appointment from the list, with inputting the date and time of the appointment.
Schedule* delete_appointment(Schedule *a, int month, int day, int time) {
    int i = 0;
    int b = -5;
    for ( ; i < len; i++) {
        int check = 0;
        if (a[i].Month == month)
            check++;
        if (a[i].Day == day)
            check++;
        if (a[i].Time == time)
            check++;
        if (check == 3)
            b = i;
    }
    if (b != -5) {
        Schedule *New = malloc(sizeof(Schedule));
        int c = 0;
        for ( ; c < b; c++) {
            New = realloc(New, (c + 1) * sizeof(Schedule));
            strcpy(New[c].description, a[c].description);
            New[c].Month = a[c].Month;
            New[c].Day = a[c].Day;
            New[c].Time = a[c].Time;
        }
        for (int d = b + 1; d < len; d++) {
            New = realloc(New, (d + 1) * sizeof(Schedule));
            strcpy(New[c].description, a[d].description);
            New[c].Month = a[d].Month;
            New[c].Day = a[d].Day;
            New[c].Time = a[d].Time;
            c++;
        }
        free(a);
        printf("Tapahtuma %d.%d klo %d poistettu!\n", day, month, time);
        len--;
        return New;
    }

    else {
        printf("Sinulla ei ole tapahtumaa %d.%d klo %d.\n", day, month, time);
        return a;
    }
}

//This function just goes through the schedule list one by one and prints their wanted info.
void print_schedule(Schedule *a) {
    for (int i = 0; i < len; i++) {
        printf("%s %d.%d klo %d\n", a[i].description, a[i].Day, a[i].Month, a[i].Time);
    }
}

//Writes the used info to a file from the list one by one.
int save_schedule (char *name, Schedule *a) {
    FILE *f = fopen(name, "w");
    if (!f) {
        printf("Tiedostoon %s tallentaminen epäonnistui.\n", name);
        return 0;
    }
    if (ferror(f)) {
        printf("Tiedostoon %s tallentaminen epäonnistui.\n", name);
        return 0;
    }
    for (int i = 0; i < len; i++) {
        fprintf(f, "%s %d %d %d\n", a[i].description, a[i].Month, a[i].Day, a[i].Time);
    }
    fclose(f);
    printf("Aikataulu tallennettu tiedostoon %s.\n", name);
    return 1;
}

//Reads data from a selected file by scanning the file row by row and inserting them in to a new list of appointments.
//Also frees the earlier list.
Schedule* read_schedule (char *name, Schedule *a) {
    FILE *f = fopen(name, "r");
    if (!f) {
        printf("Tiedoston %s avaaminen epäonnistui.\n", name);
        return a;
    }
    if (ferror(f)) {
        printf("Tiedoston %s avaaminen epäonnistui.\n", name);
        return a;
    }
    int month, day, time;
    char description[20];
    free(a);
    Schedule *New = malloc(sizeof(Schedule));
    int i = 0;
    while (fscanf(f, "%s %d %d %d", description, &month, &day, &time) == 4) {
        New = realloc(New, (i + 1) * sizeof(Schedule));
        strcpy(New[i].description, description);
        New[i].Month = month;
        New[i].Day = day;
        New[i].Time = time;
        i++; 
    }
    fclose(f);
    printf("Tiedeosto %s avattu.\n", name);
    return New;
}

//Help for the user with the different commands in the app.
void print_commandhelp () {
    printf("Tervetuloa ajanvarausjärjestelmään.\n");
    printf("Komennolla 'A' voit lisätä tapahtuman aikatauluusi.\n");
    printf("Komennolla 'D' voit poistaa aiemmin lisätyn tapahtuman.\n");
    printf("Komennolla 'L' näet aikataulusi.\n");
    printf("Komennolla 'W' voit tallentaa aikataulusi.\n");
    printf("Komennolla 'O' voit avata aiemmin tallentamasi aikataulun.\n");
    printf("Komennolla 'Q' suljet ajanvarausjärjestelmän.\n");
    printf("Anna komentoja:\n");
}



// The main function is in charge of scanning the commands with loop.
//It also checks that the selected times are valid.
int main(void) {
    print_commandhelp();
    Schedule *New;
    New = malloc(sizeof(Schedule));
    New->description[0] = 0;
    int q = 0;
    while(q == 0) {
        char string[20];
        char name[50];
        char command = '\0';
        int month, day, time; 
        scanf(" %c", &command);
        if (command == 'A') {
            scanf(" %s %d %d %d", string, &month, &day, &time);
            if (month < 13 && month > 0 && day < 32 && day > 0 && time < 25 && time > 0) {
                New = add_appointment(New, string, month, day, time);
            }
            else {
                printf("Tarkista annettu ajankohdan syöte!\n");
            }
        }
        else if (command == 'D') {
            scanf(" %d %d %d", &month, &day, &time);
            if (month < 13 && month > 0 && day < 32 && day > 0 && time < 25 && time > 0) {
                New = delete_appointment(New, month, day, time);
            }
            else {
                printf("Tarkista annettu ajankohdan syöte!\n");
            }
        }
        else if (command == 'L') {
            print_schedule(New);
        }
        else if (command == 'W') {
            scanf(" %s", name);
            save_schedule(name, New);
        }
        else if(command == 'O') {
            scanf(" %s", name);
            New = read_schedule(name, New);
        }
        else if (command == 'Q') {
            q++;
        }
    }
    free(New);
    printf("Sammutetaan ohjelmaa.\n");
    return 0;
}
