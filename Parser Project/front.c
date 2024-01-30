/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "front.h"
#include "parser.h"


/* Global Variable */
char nextToken;
int errorflag;

/* Local Variables */
static int charClass;
char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;
#define MAX_LEXEME_LENGTH 99
/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char*argv[])
{
    printf("DCooke Analyzer :: R11710212 \n");

    /* Open the input data file and process its contents */

    //if ((in_fp = fopen("exit0_0.dc", "r")) == NULL) {
      //  error(0); // file not found
        if(argc < 2){
            return(2);
        }else if((in_fp = fopen(argv[1],"r")) == NULL){
            printf("ERROR - cannot open file \n");
            return(3);
        }else {
            getChar();
            do {
                lex();
                stmt();
                
            }while (nextToken != EOF);
            
        }
        if (errorflag ==0){
            printf("\n Syntax Validated \n");
            return(0);
            
        }
        else{
            return(1);
        }    
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    addChar();
    switch (ch) {
        case '(':
            nextToken = LEFT_PAREN;
            break;
        case ')':
            nextToken = RIGHT_PAREN;
            break;
        case ';':
            nextToken = SEMICOLON;
            break;
  // check for ADD INC
        case '+':
            if ((nextChar = getc(in_fp)) == '+') {
                addChar();
                nextToken = INC_OP;
            } else {
                ungetc(nextChar, in_fp);
                nextToken = ADD_OP;
            }
            break;
  //check for DEC INC
        case '-':
            if ((nextChar = getc(in_fp)) == '-') {
                addChar();
                nextToken = DEC_OP;
            } else {
                ungetc(nextChar, in_fp);
                nextToken = SUB_OP;
            }
            break;
        case '*':
            nextToken = MULT_OP;
            break;
        case '/':
            nextToken = DIV_OP;
            break;
      //Check for EQUAL OP
        case '=':
            if ((nextChar = getc(in_fp)) == '=') {
                addChar();
                nextToken = EQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);
                nextToken = ASSIGN_OP;
            }
            break;
     //CHECK FOR LESS OP
        case '<':
            if ((nextChar = getc(in_fp)) == '=') {
                addChar();
                nextToken = LEQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);
                nextToken = LESSER_OP;
            }
            break;
            //CHECK FOR GREAT OP
        case '!':
            if ((nextChar = getc(in_fp)) == '=') {
                addChar();
                nextToken = NEQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);
                nextToken = UNKNOWN;
            }
            break;
      //CHECK FOR GREAT OP
        case '>':
            if ((nextChar = getc(in_fp)) == '=') {
                addChar();
                nextToken = GEQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);
                nextToken = GREATER_OP;
            }
            break;
        case '{':
            nextToken = LEFT_CBRACE;
            break;
        case '}':
            nextToken = RIGHT_CBRACE;
            break;

        default:
            nextToken = UNKNOWN;
            break;


    }
    return nextToken;
}/****************************************************/
/* i made this space and IDK what to do with it lol this is a comment so basically it's allowed. */

/*****************************************************/
/* a function that maps numeric token to string*/
char* tokenToString(int token) {
    switch (token) {
        case INT_LIT: return "INT_LIT";
        case IDENT: return "IDENT";
        case ASSIGN_OP: return "ASSIGN_OP";
        case ADD_OP: return "ADD_OP";
        case SUB_OP: return "SUB_OP";
        case MULT_OP: return "MULT_OP";
        case DIV_OP: return "DIV_OP";
        case LEFT_PAREN: return "LEFT_PAREN";
        case RIGHT_PAREN: return "RIGHT_PAREN";
        case LEFT_CBRACE: return "LEFT_CBRACE";
        case RIGHT_CBRACE: return "RIGHT_CBRACE";
        case EQUAL_OP: return "EQUAL_OP";
        case LESSER_OP: return "LESSER_OP";
        case GREATER_OP: return "GREATER_OP";
        case NEQUAL_OP: return "NEQUAL_OP";
        case LEQUAL_OP: return "LEQUAL_OP";
        case GEQUAL_OP: return "GEQUAL_OP";
        case SEMICOLON: return "SEMICOLON";
        case INC_OP: return "INC_OP";
        case DEC_OP: return "DEC_OP";
        case KEY_WHILE: return "KEY_WHILE";
        case KEY_WRITE: return "KEY_WRITE";
        case KEY_DO: return "KEY_DO";
        case KEY_READ: return "KEY_READ";
        case EOF: return "EOF";
        default: return "UNKNOWN";
    }
}


/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = CHARACTERS ;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }

            //check for keywords
            if(strcmp(lexeme, "read")==0){
                nextToken = KEY_READ;
            } else if(strcmp(lexeme, "write")== 0){
                nextToken = KEY_WRITE;
            } else if(strcmp(lexeme,"while")== 0){
                nextToken = KEY_WHILE;
            } else if (strcmp(lexeme, "do")== 0){
                nextToken = KEY_DO;
            } else {
                nextToken = IDENT;
            }
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case CHARACTERS:
            lookup(nextChar);
            if (nextToken == CHARACTERS){
                nextToken = CHARACTERS;
                addChar();
                getChar();
            } else {
                getChar();
            }
            break;
            

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    //how do I measure the space between the two kini like its not on the same line mehn I am tired.
    if (nextToken != EOF){
        printf("%s  \t %s\n", lexeme, tokenToString(nextToken));
        
    }
    

    return nextToken;
} /* End of function lex */

