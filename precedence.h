#include "scanner.h" 

typedef enum{
    E,       // Equals  "="
    S,       // Smaller "<"
    L,       // Larger  ">"
    X = -1   // Error   " "
} Rule;

Rule precedence[14][14] = {

    /*         +,   -,   *,   /,   (,   ),   >,   <,   >=,  <=,  ==,  ~=,  i,   $   */
    /* +  */  {L,   L,   S,   S,   S,   L,   L,   L,   L,   L,   L,   L,   L,   L},   
    /* -  */  {L,   L,   S,   S,   S,   L,   L,   L,   L,   L,   L,   L,   L,   L},   
    /* *  */  {L,   L,   L,   L,   S,   L,   L,   L,   L,   L,   L,   L,   L,   L},   
    /* /  */  {L,   L,   L,   L,   S,   L,   L,   L,   L,   L,   L,   L,   L,   L},   
    /* (  */  {S,   S,   S,   S,   S,   L,   E,   E,   E,   E,   E,   E,   S,   E},  
    /* )  */  {L,   L,   L,   L,   E,   L,   L,   L,   L,   L,   L,   L,   E,   L},   
    /* >  */  {S,   S,   S,   S,   S,   E,   L,   L,   L,   L,   L,   L,   S,   L},  
    /* <  */  {S,   S,   S,   S,   S,   E,   L,   L,   L,   L,   L,   L,   S,   L},  
    /* >= */  {S,   S,   S,   S,   S,   E,   L,   L,   L,   L,   L,   L,   S,   L},  
    /* <= */  {S,   S,   S,   S,   S,   E,   L,   L,   L,   L,   L,   L,   S,   L},  
    /* == */  {S,   S,   S,   S,   S,   E,   L,   L,   L,   L,   L,   L,   S,   L},  
    /* ~= */  {S,   S,   S,   S,   S,   E,   L,   L,   L,   L,   L,   L,   S,   L},  
    /* i  */  {L,   L,   L,   L,   E,   L,   L,   L,   L,   L,   L,   L,   E,   L},   
    /* $  */  {S,   S,   S,   S,   S,   E,   S,   S,   S,   S,   S,   S,   S,   E},   
 
};

Rule find_rule(TokenType a, TokenType b);