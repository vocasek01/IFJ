#include "scanner.h" 

typedef enum{
    E,       // Equals  "="
    S,       // Smaller "<"
    L,       // Larger  ">"
    F,       // Finish of expression
    X = -1   // Error   " "
} Rule;

Rule precedence[14][14] = {

    /*         +,   -,   *,   /,   (,   ),   >,   <,   >=,  <=,  ==,  ~=,  i,   $   */
    /* +  */  {L,   L,   S,   S,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* -  */  {L,   L,   S,   S,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* *  */  {L,   L,   L,   L,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* /  */  {L,   L,   L,   L,   S,   L,   L,   L,   L,   L,   L,   L,   S,   L},   
    /* (  */  {S,   S,   S,   S,   S,   E,   S,   S,   S,   S,   S,   S,   S,   X},  
    /* )  */  {L,   L,   L,   L,   X,   L,   L,   L,   L,   L,   L,   L,   X,   L},   
    /* >  */  {S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* <  */  {S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* >= */  {S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* <= */  {S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* == */  {S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* ~= */  {S,   S,   S,   S,   S,   L,   X,   X,   X,   X,   X,   X,   S,   L},  
    /* i  */  {L,   L,   L,   L,   X,   L,   L,   L,   L,   L,   L,   L,   F,   L},   
    /* $  */  {S,   S,   S,   S,   S,   X,   S,   S,   S,   S,   S,   S,   S,   F},   
 
};

Rule find_rule(TokenType a, TokenType b);