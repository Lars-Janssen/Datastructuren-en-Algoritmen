#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "stack.h"

char* removeSpaces(char *input);
bool correct(char *input);
bool checkParen(char *input);
void toPost(char *input);
int tokenType(char input);

/*
    Dit krijgt de argumenten en roept de andere functies aan.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s \"infix_expr\"\n", argv[0]);
        return 1;
    }
    char *input = argv[1];
    input = removeSpaces(input);
    if(correct(input) == false || checkParen(input) == false){
        return 1;
    }
    toPost(input);
    return 0;
}

/*
    Dit haalt de spaties uit de input door een nieuwe
    chararray te maken en daar alle niet-spatie tekens
    in te zetten.
*/
char* removeSpaces(char *input){
    int newInputIndex = 0;
    char *newInput = input;
    for (int i = 0; input[i]; i++)
    {
        if(input[i] != ' '){
            newInput[newInputIndex] = input[i];
            newInputIndex++;
        }
    }
    newInput[newInputIndex] = 0;
    return newInput;
}

/*
    Dit kijkt of de inut wel geldig is.
*/
bool correct(char *input){
    for (size_t i = 0; input[i]; i++)
    {
        if(input[i] != '+' && input[i] != '-' &&
           input[i] != '*' && input[i] != '/' &&
           input[i] != '^' && input[i] != '~' &&
           input[i] != '(' && input[i] != ')' &&
           input[i] != '\0' &&
           isdigit(input[i]) == 0){
               printf("%c", input[i]);
               return false;
           }
    }
    return true;
}

/*
    Dit kijkt of de haakjes van de input kloppen.
*/
bool checkParen(char *input){
    int j = 0;
    for (int i = 0;input[i]; i++)
    {
        if(input[i] == '('){
            j++;
        }
        if(input[i] == ')'){
            j--;
        }
        if(j<0){
            return false;
        }
    }
    if(j == 0){
        return true;
    }
    else{
        return false;
    }
}

/*
    Dit zet de input om in post-polish notatie en print deze.
    Deze functie is wat lang, maar omdat het vooral een grote
    if-statement is en overal de stack nodig is splits ik hem niet op.
*/
void toPost(char *input){
    /*
        Dit maakt een stack waar we de operators in gaan zetten
        en houdt bij of we al een teken hebben geprint, want
        voor het eerste teken hoort geen spatie te staan.
    */
    struct stack *operator = stack_init();
    bool firstNonParen = false;

    /*
        Dit is het algoritme. Zie Shunting-yard algorithm
        voor meer informatie.
    */
    for (int i = 0; input[i]; i++){
        int type = tokenType(input[i]);
        /*
            Dit voert de juiste stap uit op basis van het teken.
        */
        if(type == 0){
            if(i == 0 || isdigit(input[i-1]) > 0 || firstNonParen == false){
                printf("%c", input[i]);
                firstNonParen = true;
            }
            else{
                printf(" %c", input[i]);
            }
        }
        if(type == 1){
            while(stack_empty(operator) == 0 && stack_peek(operator) != '('){
                printf(" %c", stack_pop(operator));
            }
            stack_push(operator, input[i]);
        }
        if(type == 2){
            while((stack_peek(operator) == '*' || stack_peek(operator) == '/'
               || stack_peek(operator) == '^' || stack_peek(operator) == '~')
               && stack_peek(operator) != '('){
                printf(" %c", stack_pop(operator));
            }
            stack_push(operator, input[i]);
        }
        if(type == 3){
            while(stack_peek(operator) == '~' && stack_peek(operator) != '('){
                printf(" %c", stack_pop(operator));
            }
            stack_push(operator, input[i]);
        }
        if(type == 4){
            stack_push(operator, input[i]);
        }
        if(type == 5){
            stack_push(operator, input[i]);
        }
        if(type == 6){
            while(stack_peek(operator) != '('){
                printf(" %c", stack_pop(operator));
            }
            stack_pop(operator);
        }
    }

    while(stack_empty(operator) == 0){
        printf(" ");
        printf("%c", stack_pop(operator));
    }
    printf("\n");
    stack_cleanup(operator);
}

/*
    Dit bekijkt wat voor teken er opp dit moment ingelezen wordt.
*/
int tokenType(char input){
    if(isdigit(input) > 0){
        return 0;
    }
    if(input == '+' || input == '-'){
        return 1;
    }
        if(input == '*' || input == '/'){
        return 2;
    }
    if(input == '^'){
        return 3;
    }
    if(input == '~'){
        return 4;
    }
    if(input == '('){
        return 5;
    }
    if(input == ')'){
        return 6;
    }
    return -1;
}