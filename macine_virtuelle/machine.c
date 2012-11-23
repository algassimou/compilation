#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "machine.h"

static void error_fatal (char *mesg) {
  fprintf(stderr , "Erreur fatal %s\n", mesg);
  exit(-1);
}

static void warning (char *mesg) {
  fprintf(stderr , "Erreur fatal %s\n", mesg);
}

static int puissance (int v1, int v2){
  if (v1 == 0 && v2 == 0)
    error_fatal("elévation à la puissance") ;

  long res = v1 ;

  while (v2--)
    res *= v1 ;

  if (res > INT_MAX)
    warning("Pw debordement de capacité");

  return (int) res ;
}

int machine_init (Machine *m) {
  assert (m != NULL);
  
  if ((m->segment_code = malloc (SEGMENT_CODE_SIZE * sizeof(int))) == NULL)
    return -1 ;

  m->IP = 0 ;

  if ((m->segment_data = malloc (SEGMENT_DATA_SIZE * sizeof(int))) == NULL)
    return -1 ;

  m->SP = 0 ;
  m->BP = 0 ;

  return 0 ;
}

int machine_execute (Machine *m) {
  assert (m != NULL);

  m->IP = 0 ;
  
  int op = m->segment_code[m->IP] ;
  int v1, v2 ;
  char c ;
  int stop = 0 ;

  while (!stop) {
    switch (op) {
    case add :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = v1 + v2 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case and :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = v1 & v2 ;
      m->SP -- ;
      m->IP ++ ;      
      break ;

    case call :
      m->segment_data[m->SP] = m->IP + 2 ;
      m->IP = m->segment_code[m->IP+1] ;
      m->SP ++ ;
      break ;

    case calls :
      break ;

    case cont :
      v1 = m->segment_data[m->segment_data[m->SP-1]] ;
      m->segment_data[m->SP-1] = v1 ;
      m->IP ++ ;
      break ;

    case cpy :
      v1 = m->segment_code[m->IP+1];
      v2 = m->segment_data[m->SP-1] ;

      for(c = 0 ; c < v1 ; c++)
	m->segment_data[m->SP-1+c] = m->segment_data[v2+c];

      m->SP += (v1-1);
      m->IP += 2 ;
      break ;
      
    case dec :
      v1 = m->segment_code [m->IP+1] ;
      m->SP -= v1;
      if (m->SP < 0)
	error_fatal ("pointeur de pile incorrect") ;

      m->IP += 2 ;
      break ;

    case divi :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = v1 / v2 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case eq :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      if (v1 == v2)
	m->segment_data[m->SP-2] = 1 ;
      else 
	m->segment_data[m->SP-2] = 0 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case gt :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      if (v1 < v2)
	m->segment_data[m->SP-2] = 1 ;
      else 
	m->segment_data[m->SP-2] = 0 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case halt :
      stop = 1 ;
      break ;

    case in :
      scanf ("%d", &v1);
      m->segment_data[m->segment_data[m->SP-1]] = v1 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case inc :
      v1 = m->segment_code [m->IP+1] ;
      m->SP += v1;
      if (m->SP >= SEGMENT_DATA_SIZE)
	error_fatal ("pointeur de pile incorrect") ;

      m->IP += 2 ;
      break ;

    case jp :
      m->IP = m->segment_code[m->IP+1];
      break ;

    case jf :
      if (m->segment_data[m->SP-1] == 0)
	m->IP = m->segment_code[m->IP+1];
      else 
	m->IP += 2 ;

      m->SP -- ;
      break ;

    case jl :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      if (v1 > v2)
	m->IP = m->segment_code[m->IP+1];
      else 
	m->IP += 2 ;

      m->SP -= 2 ;
      break ;

    case jg :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      if (v1 < v2)
	m->IP = m->segment_code[m->IP+1];
      else 
	m->IP += 2 ;

      m->SP -= 2 ;
      break ;

    case les :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      if (v1 > v2)
	m->segment_data[m->SP-2] = 1 ;
      else 
	m->segment_data[m->SP-2] = 0 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case libp :
      v1 = m->segment_code[m->IP+1] ;
      //v2 = m->segment_data[m->BP + v1] ; pour la donnée et
      v2 = m->BP + v1 ; // pour l'adresse

      m->segment_data[m->SP] = v2 ;
      m->SP ++ ;
      m->IP += 2 ;
      break ;

    case move :
      v1 = m->segment_code[m->IP+1] ;
      v2 = m->segment_data[m->SP - v1 - 1] ;

      for (c = 0 ; c < v1 ; c++)
	m->segment_data[v2+c] = m->segment_data[m->SP - v1 + c] ;

      m->SP -= (v1 + 1) ;
      m->IP += 2 ;
      break ;

    case mult :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = v1 * v2 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case neg :
      v1 = m->segment_data[m->SP-1] ;
      m->segment_data[m->SP-1] = -v1 ;
      m->IP ++ ;
      break ;

    case not :
      v1 = m->segment_data[m->SP-1] ;
      if (v1 == 0)
	m->segment_data[m->SP-1] = 1 ;
      else 
	m->segment_data[m->SP-1] = 0 ;
      m->IP ++ ;
      break ;

    case or :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = v1 | v2 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case out :
      fprintf (stdout, "%d\n", m->segment_data[m->SP-1]) ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case outc :
      m->IP++ ;
      while ((c = m->segment_code[m->IP++]) != 0)
	fprintf (stdout, "%c", c);
      break ;

    case pow :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = puissance (v2,v1);
      m->SP -- ;
      m->IP ++ ;
      break ;

    case push :
      m->segment_data[m->SP] = m->segment_code[m->IP+1] ;
      m->SP ++ ;
      m->IP += 2 ;
      break ;

    case pushr:
      break ;

    case ret :
      m->IP = m->segment_data[m->SP-1] ;
      m->SP -- ;
      break ;

    case rstrbp :
      m->BP =m->segment_data[m->SP-1] ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    case savebp :
      m->segment_data[m->SP] = m->BP ;
      m->BP = m->SP + 1 ;
      m->SP ++ ;
      m->IP ++ ;
      break ;

    case sub :
      v1 = m->segment_data[m->SP-1] ;
      v2 = m->segment_data[m->SP-2] ;
      m->segment_data[m->SP-2] = v1 - v2 ;
      m->SP -- ;
      m->IP ++ ;
      break ;

    default :
      error_fatal ("instruction inconnu") ;
    }

    op = m->segment_code[m->IP] ;
  }
  return 0 ;
}

int machine_gen_code (Machine *m, int op){
  assert(m != NULL) ;
  //assert(op >= add && op <= sub) ;

  if (m->IP == SEGMENT_CODE_SIZE) 
    error_fatal("Segment de code plein") ;
  
  m->segment_code[m->IP] = op ;
  m->IP++ ;
  return 0 ;
}


int machine_gen_code_arg (Machine *m, int op, int arg){
  assert(m != NULL) ;
  //assert(op >= add && op <= sub) ;

  if (m->IP == SEGMENT_CODE_SIZE) 
    error_fatal("Segment de code plein") ;
  
  m->segment_code[m->IP] = op ;
  m->segment_code[m->IP+1] = arg ;
  m->IP += 2 ;
  return 0 ;
}

int machine_gen_chaine (Machine *m, const char *chaine) {
  assert(m != NULL) ;
  assert(chaine != NULL);

  if (m->IP == SEGMENT_CODE_SIZE) 
    error_fatal("Segment de code plein") ;

  m->segment_code[m->IP++] = outc ;  

  while (*chaine) {
    m->segment_code[m->IP++] = *chaine ;
    chaine++ ;
  }

  m->segment_code[m->IP++] = *chaine ;  
  return 0 ;
}
