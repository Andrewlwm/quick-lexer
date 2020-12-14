#include "ansin.h"
#include "alex.h"
#include <stdio.h>
#include <stdlib.h>

int idxCrtAtom = FAILURE;

void err_msg(char *s)
{
    fprintf(stderr, "Eroare in linia %d: %s\n", atomi[idxCrtAtom].linie, s); //afiseaza locatia atomului curent si mesajul de eroare
    exit(FAILURE);
}

int consume(int cod)
{
    if (atomi[idxCrtAtom].cod == cod)
    {
        idxCrtAtom++;
        return SUCCES;
    }
    return FAILURE;
}

//   ? FAILURE sau o data
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
    } //else err_msg("Lipseste Finish");

    idxCrtAtom = startIdx;
    return FAILURE;
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
                        err_msg("Lipseste ; la finalul declaratiei de variabila");
                }
                else
                    err_msg("Lipseste tipul de baza al  variabilei");
            }
            else
                err_msg("Lipseste : dupa numele variabilei");
        }
        else
            err_msg("Lipseste numele variabilei");
    }
    idxCrtAtom = startIdx;
    return FAILURE;
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
    return FAILURE;
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
                                        err_msg("Lipseste end in blocul functiei");
                                }
                                else
                                    err_msg("Lipseste blocul functiei");
                            }
                            else
                                err_msg("Lipseste tipul de baza in functie");
                        }
                        else
                            err_msg("Lipseste : in definirea functiei");
                    }
                    else
                        err_msg("Lipseste paranteza dreapta ) in definirea functiei");
                }
                else
                    err_msg("Lipsesc argumentele functiei");
            }
            else
                err_msg("Lipseste paranteza stanga ( in definirea functiei");
        }
        else
            err_msg("Lipseste ID-ul in definirea functiei");
    }
    idxCrtAtom = startIdx;
    return FAILURE;
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
    return FAILURE;
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
                    err_msg("Lipsa paramaetru dupa virgula");
                    return FAILURE;
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
                err_msg("Lipseste tipul de baza din parametrul functiei");
        }
        else
            err_msg("Lipseste : din parametrul functiei");
    }
    idxCrtAtom = startIdx;
    return FAILURE;
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
            err_msg("Lipseste ; de la finalul expresiei");
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
                                    err_msg("lipseste end la finalul blocului de dupa ELSE");
                            }
                            else
                                err_msg("Lipseste blocul de dupa ELSE");
                        }
                        else if (consume(END))
                        {
                            return SUCCES;
                        }
                        else
                            err_msg("Lipseste end la finalul if-ului");
                    }
                    else
                        err_msg("Lipseste block-ul din if");
                }
                else
                    err_msg("Lipseste paranteza dreapta )");
            }
            else
                err_msg("Lipseste expresia dintre parantezele if-ului");
        }
        else
            err_msg("Lipseste paranteza stanga (");
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
                err_msg("Lipseste ; dupa expresia de dupa RETURN");
        }
        else
            err_msg("LIpseste expresia de dupa RETURN");
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
                            err_msg("Lipseste end la finalul block-ului din while");
                    }
                    else
                        err_msg("Lipseste block-ul din interiorul while-ului");
                }
                else
                    err_msg("Lipseste paranteza dreapta ) la while");
            }
            else
                err_msg("Expresie invalida intre parantezele while-ului");
        }
        else
            err_msg("Lipseste paranteza stanga ( la while");
    }

    idxCrtAtom = startIdx;
    return FAILURE;
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
    return FAILURE;
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
                    err_msg("Lipseste expresia dupa AND");
            }
            else if (consume(OR))
            {
                if (exprAssign())
                {
                    //return SUCCES;
                }
                else
                    err_msg("Lipseste expresian dupa OR");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
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
    return FAILURE;
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
                err_msg("lipsa expresie dupa LESS");
        }
        else if (consume(EQUAL))
        {
            if (exprAdd())
            {
                return SUCCES;
            }
            else
                err_msg("lipsa expresie dupa EQUAL");
        }
        return SUCCES;
    }
    idxCrtAtom = startIdx;
    return FAILURE;
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
                    err_msg("lipsa expresie dupa ADD");
            }
            else if (consume(SUB))
            {
                if (exprMul())
                {
                    return SUCCES;
                }
                else
                    err_msg("lipsa expresie dupa SUB");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
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
                    err_msg("lipsa expresie dupa MUL");
            }
            else if (consume(DIV))
            {
                if (exprPrefix())
                {
                    return SUCCES;
                }
                else
                    err_msg("lipsa expresie dupa DIV");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
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
            err_msg("Lipsa factor dupa SUB");
    }
    else if (consume(NOT))
    {
        if (factor())
        {
            return SUCCES;
        }
        else
            err_msg("Lipsa factor dupa NOT");
    }
    else if (factor())
    {
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
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
                err_msg("lipsa paranteza dreapta");
        }
        else
            err_msg("expresie gresita intre paranteze");
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
                                err_msg("lipsa expresie dupa virgula");
                        }
                        else
                            break;
                    }
                    if (consume(RPAR))
                    {
                        return SUCCES;
                    }
                    else
                        err_msg("lipsa paranteza dreapta");
                }
                else if (consume(RPAR))
                {
                    return SUCCES;
                }
                else
                    err_msg("lipsa paranteza dreapta");
            }
            return SUCCES;
        }
        return FAILURE;
    }
}