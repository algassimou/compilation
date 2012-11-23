%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "interprete.h"
%}

%token NOMBRE PLUS MULT PV PARANTHESE_OUVRANTE PARANTHESE_FERMANTE
%token AFF ID

%union {
  int ival ;
  char *cval ;
}

%type <ival> NOMBRE e t f inst
%type <cval> ID inst_aff 
%%
s : s inst | inst
inst : inst_aff PV {$$ = 0 ;}| e PV  {printf ("%d" , $1) ;}
e : e PLUS t {$$ = $1 + $3;}
e : t {$$ = $1 ;}
t : t MULT f {$$ = $1 * $3; }
t : f {$$ = $1;}
f : PARANTHESE_OUVRANTE e  PARANTHESE_FERMANTE {$$ = $2;}
f : NOMBRE {$$ = $1;}
f : ID 
{
  Couple * c = search_symbole(&tableSymboles, $1) ;
  if ( c != NULL) {
    $$ = VAL_SYMBOLE((Symbole *) COUPLE_SECOND(c)) ;
  } else 
    $$ = 0 ;
}

inst_aff : ID AFF e 
{
  Symbole *s = NULL ;
  if ((s = malloc (sizeof(Symbole))) != NULL) {
    NAME_SYMBOLE(s) = $1 ;
    VAL_SYMBOLE(s) = $3 ;
    
    insert_symbole(&tableSymboles, $1 , s) ;
  }
}

%%

int yyerror (char *s) {
  return 0 ;
}

int main (void) {
  interprete_init() ;
  yyparse() ;
  return 0 ;
}
