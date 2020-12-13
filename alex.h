#ifndef __ALEX_H
#define __ALEX_H

enum Atomi
{
  ID,
  STR,
  TYPE_STR,
  VAR,
  FUNCTION,
  IF,
  ELSE,
  WHILE,
  END,
  RETURN,
  INT,
  TYPE_INT,
  REAL,
  TYPE_REAL,
  COLON,
  SEMICOLON,
  LPAR,
  RPAR,
  COMMA,
  OR,
  AND,
  NOT,
  NOTEQUAL,
  ASSIGN,
  EQUAL,
  LESS,
  ADD,
  SUB,
  MUL,
  DIV,
  FINISH
}; // toate codurile atomilor din Quick, fara SPACE si COMMENT

extern const char *numeAtomi[];

typedef struct
{
  int cod;   //codul atomului
  int linie; //linia atomului respectiv
  union
  {
    char s[100]; //pt ID-uri si string-uri
    double r;    //pentru real
    int i;       //pentru int
  };             // codul atomului ( ID, INT, ...)
} Atom;

Atom atomi[10000]; // vectorul cu atomii extrasi din fisierul de intrare
int nAtomi;        // numarul de atomi din vectorul atomi

char bufin[30001];
char *pch; // cursor la caracterul curent din bufin

void addAtom(enum Atomi cod);
int getNextTk();
void afisare_atomi();

#endif //__ALEX_H
