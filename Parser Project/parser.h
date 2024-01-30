#ifndef PARSER_H
#define PARSER_H

void expr();
void term();
void factor();
void cond();
void stmt();
void opp();
void error();
void program();

extern char lexeme[100];
extern char* tokenToString();
extern int errorflag;
extern char nextToken;
#endif
