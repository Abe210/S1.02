#ifndef __PERSON_H__
#define __PERSON_H__
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINITION D'UN TYPE PERSON : a person
// Element: un prénom unique (Ex: ALain=Alain mais Alain!=Marc)
//min_w = "Minimum year of appearance" 
struct person{
    char *name; //Hash key
    int sex[2]; //sex[0]=nb_women & sex[1]=nb_men => nb_total = sex[0] + sex[1]
    int birthdate[4]; // birthdate[0]=min_w, birthdate[1]=max_w, birthdate[2]=min_m & birthdate[3]=max_m
    struct person *next;
};
typedef struct person person;

//List
//Array of List 
//A person can be easily found by index = hash("name")
// -During collision, if hash("Name") = hash("Name1") then the person is 
//  chained on the List otherwise the List is in front
typedef struct person *Liste;

//check if Liste is empty, return true if empty, false otherwise
bool EstVide(Liste L);

//Get the next linked list
Liste GetSucc(Liste L);

//dynamically create a Person element
Liste create_person(char* name, int sexe, int birthdate,int number);

#endif