#ifndef __MACHINE__
#define __MACHINE__

struct _Machine {
  int *segment_code ;
  int *segment_data ;
  int IP, SP, BP ;
} ;

typedef struct _Machine Machine ;

#define SEGMENT_CODE_SIZE 1024
#define SEGMENT_DATA_SIZE 1024
enum OpCode {add = 1, and, call, calls, cont, cpy, dec, divi, eq, gt,
	     halt, in, inc, jp, jf, jl, jg, les, libp, move, 
	     mult, neg, not, or, out, outc, pow, push, pushr, ret,
	     rstrbp, savebp, sub} ;

int machine_init (Machine *m) ;
int machine_execute (Machine *m) ;
int machine_gen_code (Machine *m, int op); 
int machine_gen_code_arg (Machine *m, int op, int arg); 
int machine_gen_chaine (Machine *m, const char *chaine);

#endif /* __MACHINE__ */
