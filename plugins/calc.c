#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 4. Через TYPEDEF задать имена, для удобства

typedef struct {
    double* num;
    int     top,
            size;
} Num;

typedef struct {
    char* act;
    int   top,
          size;
} Act;

void numPush (Num* num, double n) {
    if (num->top > num->size - 2) {
        num->size += 5;
        num->num = realloc (num->num , num->size * sizeof (double));
    }    
    num->num [num->top] = n;
    ++num->top;
}

void actPush (Act* act, char a) {    
    if (act->top > act->size - 2) {
        act->size += 5;
        act->act = realloc (act->act , act->size);
    }
    act->act [act->top] = a;
    ++act->top;
}

char actPop (Act* act) {
    --act->top;
    return act->act [act->top];
}

int checkNum (const char * str, Num *num, double sign) {
    char * p;
    if ( str [0] <= '9' && str [0] >= '0' ) {
        numPush (num, sign * atof (str));
        p = strpbrk (str, "+-/*^()");
        if (p == NULL) return strlen (str);
        return (strlen (str) - strlen (p));
    }
    return 0;
}

int numMath (Num* num, char operation) {
    if (num->top < 2) {
        return 1;
    }
    switch (operation) {
        case '+' :
            num->num [num->top - 2] = num->num [num->top - 2] + num->num [num->top - 1];
            break;
        case '-' :
            num->num [num->top - 2] = num->num [num->top - 2] - num->num [num->top - 1];
            break;
        case '/' :
            num->num [num->top - 2] = num->num [num->top - 2] / num->num [num->top - 1];
            break;
        case '*' :
            num->num [num->top - 2] = num->num [num->top - 2] * num->num [num->top - 1];
            break;
        case '^' :
            num->num [num->top - 2] = pow (num->num [num->top - 2], num->num [num->top - 1]);
            break;
        default :
            return 1;

    }
    --num->top;
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

void initialize (const char * str, Num *num, Act *act) {
    num->top  = act->top  = 0;
    num->size = act->size = 5;
    num->num = calloc (num->size, sizeof (double));
    act->act = calloc (act->size, sizeof (char));
}

double calc (const char * str, int * status) {
    char pop;
    int step,
        // метка для чтения чисел, после прочтения операций
        expectAct = 0,
        len = strlen (str);
    double  resault,
            sign;
    Num num;
    Act act;
    initialize (str, &num, &act);

    for (int i = 0; i < len; ++i){

        if ( expectAct == 0) {
            if (str [i] == '(') {
                actPush (&act, str [i]);
                continue;
                // Save expectAct == 0.
            }
            sign = 1.0;
            if (str [i] == '-') {
                ++i;
                sign = -1.0;
            }
            if (step = checkNum (str + i, &num, sign)) {
                i += step - 1;
            }
            expectAct = 1;
            continue;
        }

        if (str [i] == ')') {
            while (1) {
                if (act.top == 0) {
                    *status = 1;
                    printf ("Check brackets.\n");
                    return 0.0;
                }
                pop = actPop (&act);
                if (pop != '(') {
                    if (numMath (&num, pop) == 1) {
                            printf ("Check operators 1 \n");
                            *status = 1;
                            return 0.0;
                    }
                }
                else {
                    break;
                }
            } 
            // save expectAct == 1;
            continue;
        }
        expectAct = 0;

        if ( act.top == 0 ||  act.act [act.top - 1] == '(' ) {
            actPush (&act, str [i]);
            continue;
        }

        while (isBigger (act.act [act.top - 1], str [i])) {
            if (numMath (&num, actPop(&act)) == 1) {
                    printf ("Check operators.\n");
                    *status = 1;
                    return 0.0;
            }
            if (act.top == 0 || act.act [act.top - 1] == '(') break;
        }
        actPush (&act, str[i]);
    }

    while(1) {
        if (act.top == 0) break;
        if (numMath (&num, actPop (&act)) == 1) {
                    printf ("Check operators.\n");
                    *status = 1;
                    return 0.0;
        }
    }

    *status = 0;
    resault = num.num[0];
    free (num.num);
    free (act.act);
    
    return resault;
}
