#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "alex.h"

int nAtomi = 0;
int linie = 1;

const char *numeAtomi[] = {"ID",
                           "STR",
                           "TYPE_STR",
                           "VAR",
                           "FUNCTION",
                           "IF",
                           "ELSE",
                           "WHILE",
                           "END",
                           "RETURN",
                           "INT",
                           "TYPE_INT",
                           "REAL",
                           "TYPE_REAL",
                           "COLON",
                           "SEMICOLON",
                           "LPAR",
                           "RPAR",
                           "COMMA",
                           "OR",
                           "AND",
                           "NOT",
                           "NOTEQUAL",
                           "ASSIGN",
                           "EQUAL",
                           "LESS",
                           "ADD",
                           "SUB",
                           "MUL",
                           "DIV",
                           "FINISH"};

void addAtom(enum Atomi cod)
{
  atomi[nAtomi].cod = cod;
  atomi[nAtomi].linie = linie;
  nAtomi++;
}

int getNextTk() // get next token (atom lexical)
{
  int state = 0; // starea curenta
  char buf[100];
  int n = 0; //nr caractere din buf
  for (;;)
  {
    char ch = *pch; // caracterul curent
                    //printf("#%d %c(%d)\n", state, ch, ch); // pt debugging
    // cate un case pentru fiecare stare din diagrama
    switch (state)
    {
    case 0:
      // testam toate tranzitiile posibile din acea stare
      if (isalpha(ch) || ch == '_')
      {
        state = 1;
        pch++;
        buf[n++] = ch;
      }
      else if (isdigit(ch))
      {
        state = 3;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '\"')
      {
        state = 8;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '#')
      {
        state = 10;
      }
      else if (ch == '|')
      {
        state = 11;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == ')')
      {
        state = 13;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '(')
      {
        state = 14;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == ':')
      {
        state = 15;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == ';')
      {
        state = 16;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == ',')
      {
        state = 17;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '&')
      {
        state = 18;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '<')
      {
        state = 20;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '+')
      {
        state = 21;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '=')
      {
        state = 22;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '!')
      {
        state = 25;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '-')
      {
        state = 28;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '*')
      {
        state = 29;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '/')
      {
        state = 30;
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '\0')
      {
        state = 31;
      }
      else if (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
      {
        pch++;
        if (ch == '\n')
          linie++;
      }
      break;
    case 1:
      if (isalnum(ch) || ch == '_')
      {
        pch++;
        buf[n++] = ch;
      }
      else
      {
        state = 2;
      }
      break;
    case 2:
      buf[n] = '\0'; //incheiere sir
      if (strcmp(buf, "var") == 0)
      {
        addAtom(VAR);
        return VAR;
      }
      else if (strcmp(buf, "function") == 0)
      {
        addAtom(FUNCTION);
        return FUNCTION;
      }
      else if (strcmp(buf, "if") == 0)
      {
        addAtom(IF);
        return IF;
      }
      else if (strcmp(buf, "else") == 0)
      {
        addAtom(ELSE);
        return ELSE;
      }
      else if (strcmp(buf, "while") == 0)
      {
        addAtom(WHILE);
        return WHILE;
      }
      else if (strcmp(buf, "end") == 0)
      {
        addAtom(END);
        return END;
      }
      else if (strcmp(buf, "return") == 0)
      {
        addAtom(RETURN);
        return RETURN;
      }
      else if (strcmp(buf, "int") == 0)
      {
        addAtom(TYPE_INT);
        return TYPE_INT;
      }
      else if (strcmp(buf, "real") == 0)
      {
        addAtom(TYPE_REAL);
        return TYPE_REAL;
      }
      else if (strcmp(buf, "str") == 0)
      {
        addAtom(TYPE_STR);
        return TYPE_STR;
      }
      else
      {
        addAtom(ID); // adauga atomul gasit in lista de atomi
        strcpy(atomi[nAtomi - 1].s, buf);
        return ID;
      }
    case 3:
      if (isdigit(ch))
      {
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '.')
      {
        state = 5;
        pch++;
        buf[n++] = ch;
      }
      else
      {
        state = 4;
      }
      break;
    case 4:
      //buf[n] = '\0';
      addAtom(INT);
      atomi[nAtomi - 1].i = atoi(buf);
      return INT;
    case 5:
      if (isdigit(ch))
      {
        state = 6;
        pch++;
        buf[n++] = ch;
      }
    case 6:
      if (isdigit(ch))
      {
        pch++;
        buf[n++] = ch;
      }
      else
      {
        state = 7;
      }
      break;
    case 7:
      //	buf[n] = '\0';
      addAtom(REAL);
      atomi[nAtomi - 1].r = atof(buf);
      return REAL;
    case 8:
      if (ch != '\"')
      {
        pch++;
        buf[n++] = ch;
      }
      else if (ch == '\"')
      {
        pch++;
        buf[n++] = ch;
        state = 9;
      }
      break;
    case 9:
      buf[n] = '\0';
      addAtom(STR);
      strcpy(atomi[nAtomi - 1].s, buf);
      return STR;
    case 10:
      while ((*pch) != '\n')
        pch++;
      return -1;
    case 11:
      if (ch == '|')
        state = 12;
    case 12:
      addAtom(OR);
      return OR;
    case 13:
      addAtom(RPAR);
      return RPAR;
    case 14:
      addAtom(LPAR);
      return LPAR;
    case 15:
      addAtom(COLON);
      return COLON;
    case 16:
      addAtom(SEMICOLON);
      return SEMICOLON;
    case 17:
      addAtom(COMMA);
      return COMMA;
    case 18:
      if (ch == '&')
        state = 19;
      break;
    case 19:
      addAtom(AND);
      return AND;
    case 20:
      addAtom(LESS);
      return LESS;
    case 21:
      addAtom(ADD);
      return ADD;
    case 22:
      if (ch == '=')
        state = 23;
      else
      {
        state = 24;
      }
      break;
    case 23:
      addAtom(EQUAL);
      return EQUAL;
    case 24:
      addAtom(ASSIGN);
      return ASSIGN;
    case 25:
      if (ch == '=')
        state = 26;
      else
      {
        state = 27;
      }
      break;
    case 26:
      addAtom(NOTEQUAL);
      return NOTEQUAL;
    case 27:
      addAtom(NOT);
      return NOT;
    case 28:
      addAtom(SUB);
      return SUB;
    case 29:
      addAtom(MUL);
      return MUL;
    case 30:
      addAtom(DIV);
      return DIV;
    case 31:
      addAtom(FINISH);
      return FINISH;
    default:
      printf("Stare invalida %d\n", state);
    }
  }
}

void afisare_atomi()
{
  for (int j = 1; j <= linie; j++)
  {
    printf("[%d]~ ", j);
    for (int i = 0; i < nAtomi; i++)
    {
      if (atomi[i].linie == j)
      {
        if (numeAtomi[atomi[i].cod] == "INT")
        {
          printf("%s:%d ", numeAtomi[atomi[i].cod], atomi[i].i);
        }
        else if (numeAtomi[atomi[i].cod] == "REAL")
        {
          printf("%s:%f ", numeAtomi[atomi[i].cod], atomi[i].r);
        }
        else if (numeAtomi[atomi[i].cod] == "STR")
        {
          printf("%s:%s ", numeAtomi[atomi[i].cod], atomi[i].s);
        }
        else if (numeAtomi[atomi[i].cod] == "ID")
        {
          printf("%s:%s ", numeAtomi[atomi[i].cod], atomi[i].s);
        }
        else
        {
          printf("%s ", numeAtomi[atomi[i].cod]);
        }
      }
    }
    printf("\n");
  }
}