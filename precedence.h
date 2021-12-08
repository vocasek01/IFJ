/*------------------------------------------
 Title:  parser.c
 Author: Koval Maksym
 Login:  xkoval20
 Date:   23 Nov 2021;
------------------------------------------*/

#include "scanner.h" 

typedef enum{
    E,       // Equals  "="
    S,       // Smaller "<"
    L,       // Larger  ">"
    F,       // Finish of expression
    X = -1   // Error   " "
} Rule;

int precedence[17][17] = {

    /*         #    +,   -,   *,   /,  //,  ..,   (,   ),   >,   <,   >=,  <=,  ==,  ~=,  i,   $   */
    /* #  */  {X,   L,   L,   L,   L,   L,   X,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* +  */  {S,   L,   L,   S,   S,   S,   X,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* -  */  {S,   L,   L,   S,   S,   S,   X,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* *  */  {S,   L,   L,   L,   L,   L,   X,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* /  */  {S,   L,   L,   L,   L,   L,   X,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* // */  {S,   L,   L,   L,   L,   L,   X,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* .. */  {X,   X,   X,   X,   X,   X,   L,   S,   X,   X,   X,   X,   X,   L,   L,   S,   L},   
    /* (  */  {S,   S,   S,   S,   S,   S,   S,   S,   E,   S,   S,   S,   S,   S,   S,   S,   X},  
    /* )  */  {X,   L,   L,   L,   L,   L,   L,   X,   L,   L,   L,   L,   L,   L,   L,   X,   L},   
    /* >  */  {S,   S,   S,   S,   S,   S,   X,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* <  */  {S,   S,   S,   S,   S,   S,   X,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* >= */  {S,   S,   S,   S,   S,   S,   X,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* <= */  {S,   S,   S,   S,   S,   S,   X,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* == */  {S,   S,   S,   S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* ~= */  {S,   S,   S,   S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* i  */  {X,   L,   L,   L,   L,   L,   L,   X,   L,   L,   L,   L,   L,   L,   L,   F,   L},   
    /* $  */  {S,   S,   S,   S,   S,   S,   S,   S,   X,   S,   S,   S,   S,   S,   S,   S,   F},   
 
};

int find_rule(TokenType a, TokenType b);