#include <stdio.h>
#include <stdlib.h>

#include "machine.h"

int main (int argc, char *argv[]){
  Machine m ;
  int r = machine_init (&m) ;
  if (r == -1)
    return -1 ;

  /*machine_gen_code_arg(&m, push, 10) ;
    machine_gen_code_arg(&m, push, 10) ;*/

  /*  machine_gen_chaine(&m, "entrer un nombre entier ") ;
  machine_gen_code_arg(&m, push, 0) ;
  machine_gen_code(&m, in) ;

  machine_gen_chaine(&m, "entrer un nombre entier ") ;
  machine_gen_code_arg(&m, push, 1) ;
  machine_gen_code(&m, in) ;

  machine_gen_code_arg(&m, push, 1) ;
  machine_gen_code(&m, cont) ;

  machine_gen_code_arg(&m, push, 0) ;
  machine_gen_code(&m, cont) ;

  machine_gen_code(&m, les) ;
  machine_gen_chaine(&m, "result vaut ") ;
  machine_gen_code(&m, out) ;
  machine_gen_code(&m, halt) ;*/

  /*machine_gen_code_arg(&m, jp, 17);
  machine_gen_chaine(&m, "Hello world!") ;
  machine_gen_code(&m, ret) ;


  machine_gen_code_arg(&m, push, 0) ;
  machine_gen_code(&m, savebp);
  machine_gen_code_arg(&m, call, 2) ;
  machine_gen_code(&m, rstrbp);
  machine_gen_chaine(&m, "\n je suis diallo algassimou\n") ;
  machine_gen_code(&m, halt) ;*/


  machine_gen_code_arg(&m, jp, 29);

  machine_gen_code_arg(&m, libp, -3);
  machine_gen_code_arg(&m, libp, -2);
  machine_gen_chaine(&m, "n vaut : ") ;
  machine_gen_code(&m, cont);
  machine_gen_code(&m, out);
  machine_gen_code_arg(&m, libp, -3);
  machine_gen_code(&m, cont);
  machine_gen_code_arg(&m, libp, -2);
  machine_gen_code(&m, cont);
  machine_gen_code(&m, add);
  machine_gen_code_arg(&m, move, 1);
  machine_gen_code(&m, ret) ;

  machine_gen_code_arg(&m, inc, 1);
  machine_gen_code_arg(&m, push, 3) ;
  machine_gen_code(&m, savebp);
  machine_gen_code_arg(&m, call, 2) ;
  machine_gen_code(&m, rstrbp);
  machine_gen_code_arg(&m, dec, 1);
  machine_gen_chaine(&m, "le return vaut : ") ;
  machine_gen_code(&m, out) ;
  machine_gen_code(&m, halt) ;

  machine_execute (&m) ;
  return 0 ;
}
