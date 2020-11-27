/*
   Based on original code by David May.
   Modified by Tom Deakin, 2020
 */

#include "stdio.h"

/* Function defintions */
void load(char *filename);
int getbyte();
int gethex();
void stop();


#define true     1
#define false    0

/* Hex 8 Opcodes */
#define i_ldam   0x0
#define i_ldbm   0x1
#define i_stam   0x2

#define i_ldac   0x3
#define i_ldbc   0x4
#define i_ldap   0x5

#define i_ldai   0x6 
#define i_ldbi   0x7 
#define i_stai   0x8

#define i_br     0x9
#define i_brz    0xA 
#define i_brn    0xB

#define i_brb    0xC

#define i_add    0xD
#define i_sub    0xE

#define i_pfix   0xF

/* Input file */
FILE *codefile;

/* 256 bytes of processor memory */
unsigned char mem[256];
unsigned char *pmem = (unsigned char *) mem;

/* Registers */
unsigned char pc;

unsigned char areg;
unsigned char breg;
unsigned char oreg;

unsigned char inst;

/* Simulator variables */
unsigned int running;
/* Number of cycles executed */
unsigned int cycles;

int main(int argc, char *argv[])
{
  printf("\n");

  /* Check for input file */
  if (argc != 2)
  {
    printf("Usage: %s a.hex\n", argv[0]);
    return 1;
  }
  
  /* Load the program into memory from the file */
  load(argv[1]);

  running = true;

  oreg = 0; 
  pc = 0;

  cycles = 0;


  while (running)
  {
    cycles += 1;

    inst = pmem[pc];

    pc = pc + 1;	

    oreg = oreg | (inst & 0xf);	
	
    switch ((inst >> 4) & 0xf)
    {
	  case i_ldam: areg = mem[oreg]; oreg = 0; break;
          case i_ldbm: breg = mem[oreg]; oreg = 0; break;
	  case i_stam: mem[oreg] = areg; oreg = 0; break;	  
		  
	  case i_ldac: areg = oreg; oreg = 0; break;
	  case i_ldbc: breg = oreg; oreg = 0; break;
	  case i_ldap: areg = pc + oreg; oreg = 0; break;
		  
	  case i_ldai: areg = mem[areg + oreg]; oreg = 0; break;
	  case i_ldbi: breg = mem[breg + oreg]; oreg = 0; break;
	  case i_stai: mem[breg + oreg] = areg; oreg = 0; break;	  
		  
	  case i_br:   stop(); pc = pc + oreg; oreg = 0; break;
	  case i_brz:  if (areg == 0) pc = pc + oreg; oreg = 0; break;
	  case i_brn:  if (areg > 127) pc = pc + oreg; oreg = 0; break;	  

	  case i_brb:  pc = breg; oreg = 0; break; 
		  
	  case i_add:  areg = areg + breg; oreg = 0; break; 
	  case i_sub:  areg = areg - breg; oreg = 0; break;	  
		  
	  case i_pfix: oreg = oreg << 4; break;
	};
  	
  }
	
}	
		  		  		  
void load(char *filename)
{
  int n;
  int i;
  codefile = fopen(filename, "rb");
  n = getbyte();
  i = 0;	
  while (n != EOF)
  { pmem[i] = n;
	n = getbyte();
	i = i + 1;
  };
}

int getbyte()
{
  int high;
  high = gethex();
  if (high == EOF)
	return EOF;
  else
    return (high << 4) | gethex();
}

int gethex()
{
  int h;
  h = fgetc(codefile);
  while ((h == ' ') || (h == '\n')) 
	h = fgetc(codefile);
  if (h == EOF)
	return EOF;
  else	  
  if ((h >= 'A') && (h <= 'F'))  
	return (h - 'A') + 10;
  else 
	return h - '0';
}

void stop()
{
  if (oreg == 0xFE)
  {
    printf("\ncycles = %d\n", cycles);
    printf("\nareg = %d (%X)\n", areg, areg&0xff);
    running = false; 

    printf("Memory:\n");
    int i;
    for (i = 0; i < 256; i++)
    {
      printf("%d: %X\n", i, mem[i] & 0xff);
    }
  }	  
}



