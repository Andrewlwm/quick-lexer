#include "ansin.h"
#include "alex.h"
#include <stdio.h>
#include <stdlib.h>

int idxCrtAtom = 0;

void err(char *s)
{
    fprintf(stderr, "Eroare in linia %d: %s\n", atomi[idxCrtAtom].linie, s); //afiseaza locatia atomului curent si mesajul de eroare
    exit(0);
}

int consume(int cod)
{
    if (atomi[idxCrtAtom].cod == cod)
    {
        idxCrtAtom++;
        return SUCCES;
    }
    return 0;
}

//   ? 0 sau o data
//   + minim o data
//   * poate sa lipseasca sau sa fie de mai multe ori

//program ::= ( defVar | defFunc | block )* FINISH
int program()
{ // true sau false
    int startIdx = idxCrtAtom;

    for (;;)
    {
        if (defVar())
        {
        }
        else if (defFunc())
        {
        }
        else if (block())
        {
        }
        else
            break;
    }
    if (consume(FINISH))
    {
        return SUCCES;
    } //else err("Lipseste Finish");

    idxCrtAtom = startIdx;
    return 0;
}

//defVar ::= VAR ID COLON baseType SEMICOLON
int defVar()
{
    int startIdx = idxCrtAtom;
    // pentru debugging
    //printf("#defVar %s\n", numeAtom(atomi[idxCrtAtom].cod));

    if (consume(VAR))
    {
        if (consume(ID))
        {
            if (consume(COLON))
            {
                if (baseType())
                {
                    if (consume(SEMICOLON))
                    {
                        return SUCCES;
                    }
                    else
                        err("Lipseste ; la finalul declaratiei de variabila");
                }
                else
                    err("Lipseste tipul de baza al  variabilei");
            }
            else
                err("Lipseste : dupa numele variabilei");
        }
        else
            err("Lipseste numele variabilei");
    }
    idxCrtAtom = startIdx;
    return 0;
}

//baseType ::= TYPE_INT | TYPE_REAL | TYPE_STR
int baseType()
{
    int startIdx = idxCrtAtom;
    if (consume(TYPE_INT))
    {
        return SUCCES;
    }
    else if (consume(TYPE_REAL))
    {
        return SUCCES;
    }
    else if (consume(TYPE_STR))
    {
        return SUCCES;
    }
    idxCrtAtom = startIdx;
    return 0;
}

//defFunc ::= FUNCTION ID LPAR funcParams RPAR COLON baseType defVar* block END
int defFunc()
{
    int startIdx = idxCrtAtom;

    if (consume(FUNCTION))
    {
        if (consume(ID))
        {
            if (consume(LPAR))
            {
                if (funcParams())
                {
                    if (consume(RPAR))
                    {
                        if (consume(COLON))
                        {
                            if (baseType())
                            {
                                for (;;)
                                {
                                    if (defVar())
                                    {
                                    }
                                    else
                                        break;
                                }
                                if (block())
                                {
                                    if (consume(END))
                                    {
                                        return SUCCES;
                                    }
                                    else
                                        err("Lipseste end in blocul functiei");
                                }
                                else
                                    err("Lipseste blocul functiei");
                            }
                            else
                                err("Lipseste tipul de baza in functie");
                        }
                        else
                            err("Lipseste : in definirea functiei");
                    }
                    else
                        err("Lipseste paranteza dreapta ) in definirea functiei");
                }
                else
                    err("Lipsesc argumentele functiei");
            }
            else
                err("Lipseste paranteza stanga ( in definirea functiei");
        }
        else
            err("Lipseste ID-ul in definirea functiei");
    }
    idxCrtAtom = startIdx;
    return 0;
}

//block ::= instr+    (cel putin o data)
int block()
{
    int startIdx = idxCrtAtom;

    if (instr())
    {
        for (;;)
        {
            if (!instr())
                break;
        }
        return SUCCES;
    }
    idxCrtAtom = startIdx;
    return 0;
}

//funcParams ::= ( funcParam ( COMMA funcParam )* )?
int funcParams()
{
    int startIdx = idxCrtAtom;
    if (funcParam())
    {
        for (;;)
        {
            if (consume(COMMA))
            {
                if (funcParam())
                {
                }
                else
                {
                    err("Lipsa paramaetru dupa virgula");
                    return 0;
                }
            }
            else
                break;
        }
    }
    return SUCCES;
}

//funcParam ::= ID COLON baseType
int funcParam()
{
    int startIdx = idxCrtAtom;

    if (consume(ID))
    {
        if (consume(COLON))
        {
            if (baseType())
            {
                return SUCCES;
            }
            else
                err("Lipseste tipul de baza din parametrul functiei");
        }
        else
            err("Lipseste : din parametrul functiei");
    }
    idxCrtAtom = startIdx;
    return 0;
}

//instr ::= expr? SEMICOLON
//     | IF LPAR expr RPAR block ( ELSE block )? END
//     | RETURN expr SEMICOLON
//   | WHILE LPAR expr RPAR block END
int instr()
{
    int startIdx = idxCrtAtom;

    if (expr())
    {
        if (consume(SEMICOLON))
        {
            return SUCCES;
        }
        else
            err("Lipseste ; de la finalul expresiei");
    }
    else if (consume(IF))
    {
        if (consume(LPAR))
        {
            if (expr())
            {
                if (consume(RPAR))
                {
                    if (block())
                    {
                        if (consume(ELSE))
                        {
                            if (block())
                            {
                                if (consume(END))
                                {
                                    return SUCCES;
                                }
                                else
                                    err("lipseste end la finalul blocului de dupa ELSE");
                            }
                            else
                                err("Lipseste blocul de dupa ELSE");
                        }
                        else if (consume(END))
                        {
                            return SUCCES;
                        }
                        else
                            err("Lipseste end la finalul if-ului");
                    }
                    else
                        err("Lipseste block-ul din if");
                }
                else
                    err("Lipseste paranteza dreapta )");
            }
            else
                err("Lipseste expresia dintre parantezele if-ului");
        }
        else
            err("Lipseste paranteza stanga (");
    }
    else if (consume(RETURN))
    {
        if (expr())
        {
            if (consume(SEMICOLON))
            {
                return SUCCES;
            }
            else
                err("Lipseste ; dupa expresia de dupa RETURN");
        }
        else
            err("LIpseste expresia de dupa RETURN");
    }
    else if (consume(WHILE))
    {
        if (consume(LPAR))
        {
            if (expr())
            {
                if (consume(RPAR))
                {
                    if (block())
                    {
                        if (consume(END))
                        {
                            return SUCCES;
                        }
                        else
                            err("Lipseste end la finalul block-ului din while");
                    }
                    else
                        err("Lipseste block-ul din interiorul while-ului");
                }
                else
                    err("Lipseste paranteza dreapta ) la while");
            }
            else
                err("Expresie invalida intre parantezele while-ului");
        }
        else
            err("Lipseste paranteza stanga ( la while");
    }

    idxCrtAtom = startIdx;
    return 0;
}

// expr ::= exprLogic
int expr()
{
    int startIdx = idxCrtAtom;

    if (exprLogic())
    {
        return SUCCES;
    }
    idxCrtAtom = startIdx;
    return 0;
}

//exprLogic ::= exprAssign ( ( AND | OR ) exprAssign )*
int exprLogic()
{
    int startIdx = idxCrtAtom;

    if (exprAssign())
    {
        for (;;)
        {
            if (consume(AND))
            {
                if (exprAssign())
                {
                    //return SUCCES;
                }
                else
                    err("Lipseste expresia dupa AND");
            }
            else if (consume(OR))
            {
                if (exprAssign())
                {
                    //return SUCCES;
                }
                else
                    err("Lipseste expresian dupa OR");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return 0;
}

//exprAssign ::= ( ID ASSIGN )? exprComp
int exprAssign()
{
    int startIdx = idxCrtAtom;
    if (consume(ID))
    {
        if (consume(ASSIGN))
        {
        }
        else
        {
            idxCrtAtom = startIdx;
        }
    }
    if (exprComp())
    {
        return SUCCES;
    }
    idxCrtAtom = startIdx;
    return 0;
}

//exprComp ::= exprAdd ( ( LESS | EQUAL ) exprAdd )?
int exprComp()
{
    int startIdx = idxCrtAtom;
    if (exprAdd())
    {
        if (consume(LESS))
        {
            if (exprAdd())
            {
                return SUCCES;
            }
            else
                err("lipsa expresie dupa LESS");
        }
        else if (consume(EQUAL))
        {
            if (exprAdd())
            {
                return SUCCES;
            }
            else
                err("lipsa expresie dupa EQUAL");
        }
        return SUCCES;
    }
    idxCrtAtom = startIdx;
    return 0;
}

//exprAdd ::= exprMul ( ( ADD | SUB ) exprMul )*
int exprAdd()
{
    int startIdx = idxCrtAtom;
    if (exprMul())
    {
        for (;;)
        {
            if (consume(ADD))
            {
                if (exprMul())
                {
                    return SUCCES;
                }
                else
                    err("lipsa expresie dupa ADD");
            }
            else if (consume(SUB))
            {
                if (exprMul())
                {
                    return SUCCES;
                }
                else
                    err("lipsa expresie dupa SUB");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return 0;
}

//exprMul ::= exprPrefix ( ( MUL | DIV ) exprPrefix )*
int exprMul()
{
    int startIdx = idxCrtAtom;
    if (exprPrefix())
    {
        for (;;)
        {
            if (consume(MUL))
            {
                if (exprPrefix())
                {
                    return SUCCES;
                }
                else
                    err("lipsa expresie dupa MUL");
            }
            else if (consume(DIV))
            {
                if (exprPrefix())
                {
                    return SUCCES;
                }
                else
                    err("lipsa expresie dupa DIV");
            }

            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return 0;
}

//exprPrefix ::= ( SUB | NOT )? factor
int exprPrefix()
{
    int startIdx = idxCrtAtom;

    if (consume(SUB))
    {
        if (factor())
        {
            return SUCCES;
        }
        else
            err("Lipsa factor dupa SUB");
    }
    else if (consume(NOT))
    {
        if (factor())
        {
            return SUCCES;
        }
        else
            err("Lipsa factor dupa NOT");
    }
    else if (factor())
    {
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return 0;
}

//factor ::= INT
//         | REAL
//        | STR
//        | LPAR expr RPAR
//       | ID ( LPAR ( expr ( COMMA expr )* )? RPAR )?

int factor()
{
    if (consume(INT))
    {
        return SUCCES;
    }
    else if (consume(REAL))
    {
        return SUCCES;
    }
    else if (consume(STR))
    {
        return SUCCES;
    }
    else if (consume(LPAR))
    {
        if (expr())
        {
            if (consume(RPAR))
            {
                return SUCCES;
            }
            else
                err("lipsa paranteza dreapta");
        }
        else
            err("expresie gresita intre paranteze");
    }
    else
    {
        if (consume(ID))
        {
            if (consume(LPAR))
            {
                if (expr())
                {
                    while (SUCCES)
                    {
                        if (consume(COMMA))
                        {
                            if (!expr())
                                err("lipsa expresie dupa virgula");
                        }
                        else
                            break;
                    }
                    if (consume(RPAR))
                    {
                        return SUCCES;
                    }
                    else
                        err("lipsa paranteza dreapta");
                }
                else if (consume(RPAR))
                {
                    return SUCCES;
                }
                else
                    err("lipsa paranteza dreapta");
            }
            return SUCCES;
        }
        return 0;
    }
}