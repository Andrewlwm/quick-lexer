#include "ansin.h"
#include "alex.h"
#include <stdio.h>
#include <stdlib.h>

int idxCrtAtom = 0;

void err_msg(char *s)
{
    fprintf(stderr, "Error on line %d: %s\n", atomi[idxCrtAtom].linie, s); //afiseaza locatia atomului curent si mesajul de eroare
    exit(!FAILURE);
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

int program()
{
    int startIdx = idxCrtAtom;

    while (1)
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
        return SUCCES;
    //else err_msg("Finish not consumed.");

    idxCrtAtom = startIdx;
    return FAILURE;
}

int defVar()
{
    int startIdx = idxCrtAtom;

    if (!consume(VAR))
    {
        idxCrtAtom = startIdx;
        return FAILURE;
    }

    if (!consume(ID))
        err_msg("Expected `id` in variable declaration.");
    if (!consume(COLON))
        err_msg("Expected `:` in variable declaration.");
    if (!baseType())
        err_msg("Expected base type in variable declaration.");
    if (!consume(SEMICOLON))
        err_msg("Expected `;` after variable declaration.");

    return SUCCES;
}

int baseType()
{
    int startIdx = idxCrtAtom;

    if (consume(TYPE_INT))
        return SUCCES;
    else if (consume(TYPE_REAL))
        return SUCCES;
    else if (consume(TYPE_STR))
        return SUCCES;

    idxCrtAtom = startIdx;
    return FAILURE;
}

int defFunc()
{
    int startIdx = idxCrtAtom;

    if (consume(FUNCTION))
    {
        if (!consume(ID))
            err_msg("Expected `id` in function definition.");
        if (!consume(LPAR))
            err_msg("Expected `(` in function definition.");
        if (funcParams())
        {
        }
        if (!consume(RPAR))
            err_msg("Expected `)` in function definition.");
        if (!consume(COLON))
            err_msg("Expected `:` in function definition.");
        if (!baseType())
            err_msg("Expected return type in function definition.");

        while (1)
        {
            if (defVar())
            {
            }
            else
                break;
        }

        if (!block())
            err_msg("Function body not defined.");
        if (!consume(END))
            err_msg("Expected `end` block in function body.");

        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int block()
{
    int startIdx = idxCrtAtom;

    if (instr())
    {
        while (1)
        {
            if (!instr())
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int funcParams()
{
    int startIdx = idxCrtAtom;

    if (funcParam())
    {
        while (1)
        {
            if (consume(COMMA))
            {
                if (!funcParam())
                {
                    err_msg("Expected parameter after `,`.");
                    idxCrtAtom = startIdx;
                    return FAILURE;
                }
            }
            else
                break;
        }
    }

    return SUCCES;
}

int funcParam()
{
    int startIdx = idxCrtAtom;

    if (consume(ID))
    {
        if (!consume(COLON))
            err_msg("Expected `:` in function parameter declaration.");
        if (!baseType())
            err_msg("Expected base type in function parameter declaration.");

        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int instr()
{
    int startIdx = idxCrtAtom;

    if (expr())
    {
        if (!consume(SEMICOLON))
            err_msg("Expected `;` after expression.");

        return SUCCES;
    }
    else if (consume(IF))
    {
        if (!consume(LPAR))
            err_msg("Expected `(` in `if` statement.");
        if (!expr())
            err_msg("Expected expression inside `if` parenthesis.");
        if (!consume(RPAR))
            err_msg("Expected `)` in `if` statement.");
        if (!block())
            err_msg("`if` body not defined.");
        if (consume(ELSE))
        {
            if (!block())
                err_msg("`else` body not defined");
            if (!consume(END))
                err_msg("Expected `end` block in `else` body.");

            return SUCCES;
        }
        else if (consume(END))
            err_msg("Expected `end` block in `if` body.");

        return SUCCES;
    }
    else if (consume(RETURN))
    {
        if (!expr())
            err_msg("Expected expression after `return`.");
        if (!consume(SEMICOLON))
            err_msg("Expected `;` after `return`.");

        return SUCCES;
    }
    else if (consume(WHILE))
    {
        if (!consume(LPAR))
            err_msg("Expected `(` in `while` condition.");
        if (!expr())
            err_msg("Invalid expression in `while` condition.");
        if (!consume(RPAR))
            err_msg("Expected `)` in `while` condition.");
        if (!block())
            err_msg("`while` body not defined.");
        if (!consume(END))
            err_msg("Expected `end` block in `while` body.");

        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int expr()
{
    int startIdx = idxCrtAtom;

    if (exprLogic())
        return SUCCES;

    idxCrtAtom = startIdx;
    return FAILURE;
}

int exprLogic()
{
    int startIdx = idxCrtAtom;

    if (exprAssign())
    {
        while (1)
        {
            if (consume(AND))
            {
                if (!exprAssign())
                    err_msg("Missing expression after `and`.");
            }
            else if (consume(OR))
            {
                if (!exprAssign())
                    err_msg("Missing expression after `or`.");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int exprAssign()
{
    int startIdx = idxCrtAtom;

    if (consume(ID))
        if (!consume(ASSIGN))
            idxCrtAtom = startIdx;

    if (exprComp())
        return SUCCES;

    idxCrtAtom = startIdx;
    return FAILURE;
}

int exprComp()
{
    int startIdx = idxCrtAtom;

    if (!exprAdd())
    {
        idxCrtAtom = startIdx;
        return FAILURE;
    }

    if (consume(LESS))
    {
        if (!exprAdd())
            err_msg("Expected expression after `less`.");
    }
    if (consume(EQUAL))
        if (!exprAdd())
            err_msg("Expected expression after `equal`.");

    return SUCCES;
}

int exprAdd()
{
    int startIdx = idxCrtAtom;

    if (!exprMul())
    {
        idxCrtAtom = startIdx;
        return FAILURE;
    }

    while (1)
    {
        if (consume(ADD))
        {
            if (!exprMul())
                err_msg("Expected expression after `add`.");
        }
        else if (consume(SUB))
        {
            if (!exprMul())
                err_msg("Expected expression after `sub`.");
        }
        else
            break;
    }

    return SUCCES;
}

int exprMul()
{
    int startIdx = idxCrtAtom;

    if (exprPrefix())
    {
        while (1)
        {
            if (consume(MUL))
            {
                if (!exprPrefix())
                    err_msg("Expected expression after `mul`.");
            }
            else if (consume(DIV))
            {
                if (!exprPrefix())
                    err_msg("Expected expression after `div`.");
            }
            else
                break;
        }
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int exprPrefix()
{
    int startIdx = idxCrtAtom;

    if (consume(SUB))
    {
        if (!factor())
            err_msg("Expected factor after `sub`.");

        return SUCCES;
    }
    else if (consume(NOT))
    {
        if (!factor())
            err_msg("Expected factor after `not`.");

        return SUCCES;
    }
    else if (factor())
    {
        return SUCCES;
    }

    idxCrtAtom = startIdx;
    return FAILURE;
}

int factor()
{
    if (consume(INT))
        return SUCCES;
    else if (consume(REAL))
        return SUCCES;
    else if (consume(STR))
        return SUCCES;
    else if (consume(LPAR))
    {
        if (!expr())
            err_msg("Invalid expression in parenthesis.");
        if (!consume(RPAR))
            err_msg("Expected `)` after expression.");

        return SUCCES;
    }
    else
    {
        if (!consume(ID))
            return FAILURE;

        if (consume(LPAR))
        {
            if (expr())
            {
                while (1)
                {
                    if (consume(COMMA))
                    {
                        if (!expr())
                            err_msg("Expected expression after `,`.");
                    }
                    else
                        break;
                }
                if (!consume(RPAR))
                    err_msg("Expected `)` after expression.");
            }
            else if (!consume(RPAR))
                err_msg("Expected `)` after expression.");
        }

        return SUCCES;
    }
}