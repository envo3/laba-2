#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    double * Number;
    int top, size;
} Number;

typedef struct {
    char * Action;
    int top, size;
} Action;

void NumberPush (Number* Number, double n) {
    if (Number->top > Number->size - 2) {
        Number->size += 10;
        Number->Number = realloc (Number->Number , Number->size * sizeof (double));
    }    
    Number->Number [Number->top] = n;
    ++Number->top;
}

void ActionPush (Action* Action, char a) {    
    if (Action->top > Action->size - 2) {
        Action->size += 10;
        Action->Action = realloc (Action->Action , Action->size);
    }
    Action->Action [Action->top] = a;
    ++Action->top;
}

char ActionPop (Action* Action) {
    --Action->top;
    return Action->Action [Action->top];
}

int NumberMath (Number* Number, char operation) {
    if (Number->top < 2) {
        return 1;
    }
    --Number->top;
    if (operation == '+') {
        Number->Number [Number->top - 1] = Number->Number [Number->top - 1] + Number->Number [Number->top];
    } else if (operation == '-') {
        Number->Number [Number->top - 1] = Number->Number [Number->top - 1] - Number->Number [Number->top];
    } else if (operation == '/') {
        Number->Number [Number->top - 1] = Number->Number [Number->top - 1] / Number->Number [Number->top];
    } else if (operation == '*') {
        Number->Number [Number->top - 1] = Number->Number [Number->top - 1] * Number->Number [Number->top];
    } else if (operation == '^') {
        Number->Number [Number->top - 1] = pow (Number->Number [Number->top - 1], Number->Number [Number->top]);
    } else {
        return 1;
    }
    return 0;
}

int priority (char op) {
    if ( op == '+' || op == '-' ) {
        return 1;
    }
    if( op == '/' || op == '*' ) {
        return 2;
    }
    return 3;
}

int isBigger (char big, char lit) {
    return priority (big) >= priority (lit);
}

void initialize (Number *Number, Action *Action) {
    Number->top  = Action->top  = 0;
    Number->size = Action->size = 10;
    Number->Number = calloc (Number->size, sizeof (double));
    Action->Action = calloc (Action->size, sizeof (char));
}

double calc (const char * str, int * status) {
    char pop;
    int step,
        // метка для чтения чисел, после прочтения операций
        expectAction = 0,
        len = strlen (str);
    double  resault = 0;
    Number Number;
    Action Action;
    initialize (&Number, &Action);

    for (int i = 0; i < len; ++i){
        if ( expectAction == 0) {
            if (str [i] == '(') {
                ActionPush (&Action, str [i]);
            } else if (isdigit(str[i])||(str[i] == '-' && isdigit(str[i + 1]))) {
                NumberPush (&Number, atof (str + i));
                i = strpbrk (str + i, "+-/*^()") - str - 1;
                expectAction = 1;
            } else {
                *status = 1;
                printf ("Expect number.\n");
                break;                
            }
        } else if (str [i] == ')') {
            while (1) {
                if (Action.top == 0) {
                    *status = 1;
                    printf ("Check brackets.\n");
                    break;
                }
                pop = ActionPop (&Action);
                if (pop != '(') {
                    if (NumberMath (&Number, pop)) {
                        printf ("Check operators.\n");
                        *status = 1;
                        break;
                    }
                } else {
                    break;
                }
            }
        } else {
            expectAction = 0;
            if ( Action.top == 0 ||  Action.Action [Action.top - 1] == '(' ) {
                ActionPush (&Action, str [i]);
            } else {
                while (((Action.top > 0 && Action.Action [Action.top - 1] != '(')) 
                    && isBigger (Action.Action [Action.top - 1], str [i])) {
                    if (NumberMath (&Number, ActionPop(&Action))) {
                        printf ("Check operators.\n");
                        *status = 1;
                        break;
                    }
                }
                ActionPush (&Action, str[i]);            
            }
        }
    }
    while ((Action.top > 0)) {
        if (NumberMath (&Number, ActionPop (&Action))) {
            printf ("Check operators.\n");
            *status = 1;
            break;
        }
    }
    if (*status == 0) {
        resault = Number.Number[0];
    }
    free (Number.Number);
    free (Action.Action);
    return resault;
}