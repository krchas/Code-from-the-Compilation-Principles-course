#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convert.h"

 TokenList list;
 TokenList IR_list;
 FILE* sFile;
 char TokenText[256];
Token token;
 void TokenReader();

 int count_T = 0, count_L = 1;
 

int isLetter(char ch)
{
	if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
	{
		return 1;
	}
	return 0;
}

int isDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return 1;
	}
	return 0;
}

int cmpstring(char a[], char b[])
{
	int la = strlen(a);
	int lb = strlen(b);
	if (la != lb)
		return 0;
	for (size_t i = 0; i <= la; i++)
	{
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}

dfastat initToken(char ch)
{
	if (strlen(TokenText) > 0)//如果当前进入DFA的字符串不为空
	{
		
		TokenList tokenNode = (TokenList)malloc(sizeof(Token));
		tokenNode->type = token.type;
		strcpy(tokenNode->text, TokenText);
		tokenNode->next = NULL;
		tokenNode->parent = NULL;
		
		if (list == NULL)
		{
			list = tokenNode;
		}
		else
		{
			TokenList p = list;
			while (p->next!=NULL)
			{
				p = p->next;
			}
			p->next = tokenNode;
			tokenNode->parent = p;
		}
		memset(TokenText, '\0',256);
		memset(token.text, '\0', 256);
	}
	dfastat newstat = InitialStat;
	if (isLetter(ch))
	{
		newstat = LetterStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Identifier;
	}
	else if(isDigit(ch))
	{
		newstat = DigitStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Identifier;
	}
	else if('>'==ch)
	{
		newstat = GTStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if('<'==ch)
	{
		newstat = LTStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if('='==ch)
	{
		newstat =AssignmentStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if(':'==ch)
	{
		newstat = SemiColonStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}else if('+'==ch)
	{
		newstat = PlusStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Identifier;
	}else if('('==ch)
	{
		newstat = LeftParenStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if(')'==ch)
	{
		newstat = RightParenStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if('{'==ch)
	{
		newstat = LeftBraceStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if('}'==ch)
	{
		newstat = RightBraceStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
	else if(','==ch)
	{
		newstat = DotStat;
		TokenText[strlen(TokenText) ] = ch;
		//token.type = Identifier;
	}
    else if(';'==ch)
	{
		newstat = SemiColonStat;
		TokenText[strlen(TokenText) ] = ch;
	}
    else if('|'==ch)
	{
		newstat = OrStat;
		TokenText[strlen(TokenText) ] = ch;
	}
    else if('&'==ch)
    {
		newstat = AndStat;
		TokenText[strlen(TokenText) ] = ch;
	}
	return newstat;
}

void TokenReader()
{
	memset(TokenText, '\0', 256);
	char c;
	dfastat stat = InitialStat;
    //从源文件中开始读取内容，并得到相关Token
	while (!feof(sFile))
	{
		c = fgetc(sFile);
		
		switch (stat)
		{

		case InitialStat:
			stat = initToken(c);
			break;
		case LetterStat:
			if (isLetter(c)||isDigit(c))
			{
				stat = LetterStat;
				TokenText[strlen(TokenText) ] = c;
			}
			else
			{   
				if (cmpstring(TokenText, "main"))
				{
					token.type = Main;
				}	
				else if (cmpstring(TokenText, "if"))
				{
					token.type = If;
				}
				else if (cmpstring(TokenText, "else"))
				{
					token.type = Else;
				}
				else if (cmpstring(TokenText, "int"))
				{
					token.type = Int;
				}
				else if (cmpstring(TokenText, "show"))
				{
					token.type = Show;
				}
                else if (cmpstring(TokenText, "while"))
				{
					token.type = While;
				}
				else
				{
					token.type = Identifier;
				}
				stat = initToken(c);
			}
			break;
		case DigitStat:	
			if (isDigit(c))
			{
				stat = DigitStat;
				TokenText[strlen(TokenText) ] = c;
			}
			else
			{
				token.type = IntLiteral;
				stat = initToken(c);
			}
			break;
		case GTStat://大于
			if (cmpstring(TokenText, ">"))
			{
				token.type = GT;
			}	
			stat = initToken(c);
			break;	
		case LTStat://小于
			
			token.type = LT;
					
			stat = initToken(c);
			break;			         
		case AssignmentStat:
            if(c == '=')
            {
                stat = AssignmentStat;
                TokenText[strlen(TokenText) ] = c;
            }
            else 
            {
                if (cmpstring(TokenText, "=="))
                {
                      token.type = Equal;
                }  
                else if (cmpstring(TokenText, "="))
                {
                      token.type = Assignment;
                }   
                else 
                {
                  printf("error!more than two continuous \'=\'\n");
                }     
                stat = initToken(c);
            }
			break;		
			//等于
        case OrStat:
            if(c == '|')
            {
                stat = OrStat;
                TokenText[strlen(TokenText) ] = c;
            }
            else 
            {
                if(cmpstring(TokenText, "||"))
                    token.type = Or;
                //else if(cmpstring(TokenText, "="))
                //       token.type = Bor;
                else 
                {
                    printf("error!more than two continuous \'|\'\n");
                }     
                stat = initToken(c);
            }
            break;
        case AndStat:
            if(c == '&')
            {
                stat = OrStat;
                TokenText[strlen(TokenText) ] = c;
            }
            else 
            {
                if(cmpstring(TokenText, "&&"))
                    token.type = And;
                //else if(cmpstring(TokenText, "="))
                //       token.type = Band;
                else 
                {
                    printf("error!more than two continuous \'&\'\n");
                }     
                stat = initToken(c);
            }  
            break;    
		case ColonStat: 
			token.type = Colon;
				 //冒号
			stat = initToken(c);
			break;	
		case PlusStat: 
	        
			token.type = Plus;
				
			stat = initToken(c);
			break;		     //加号

		case LeftParenStat:
			
	 		token.type = LeftParen;
			stat = initToken(c);
			break;		 //左圆括号

		case RightParenStat:
			token.type = RightParen;
			stat = initToken(c);
			break;		//右括号

		case LeftBraceStat:  
			
			token.type = LeftBrace;//左花括号
			stat = initToken(c);
			break;		

		case RightBraceStat:
			 
			token.type = RightBrace;
			stat = initToken(c);
			break;		//右花括号

		case DotStat:        //逗号:
			token.type = Dot;
			stat = initToken(c);
			break;

		case SemiColonStat:    //分号
			token.type = SemiColon;
			stat = initToken(c);
			break;

		default:
			stat = initToken(c);
			break;
		}
					
	}	
}

IdTableList IdHead = NULL;
TokenList pList;
IRList IRList_Head = NULL;
FILE* IRFile;

ASTNodeList assignmentStatment();
ASTNodeList primary();
ASTNodeList intDeclare();
ASTNodeList intNodeList();
ASTNodeList boolStatement();
ASTNodeList addtiveStatment();
ASTNodeList relStatement();
ASTNodeList showStatement();
ASTNodeList ifStatement();
ASTNodeList whileStatement();
ASTNodeList Body();

void varDeclarationBody(ASTNodeList node, TokenList tokentemp);
void ifBody(ASTNodeList node, TokenList tokentemp);
void showBody(ASTNodeList node, TokenList tokentemp);
void assignmentBody(ASTNodeList node, TokenList tokentemp);

int continueSign = 1;
int breakSign = 1;

char ASTNodeTypeString[30][256] = 
{
    "Program",
    "IntDeclaration",
    "ExpressionStmt",
    "Assignment",
    "Primary",
    "Addtive",
    "Identifier",
    "IntLiteral",
    "Body",
    "If",
    "Else",
    "Boole",
    "While",
    "GT",
    "LT",
    "Equal",
    "Show",
};

void addASTNode(ASTNodeList node, ASTNodeList child) 
{
    if (node != NULL && child != NULL) 
    {
        ASTChildrenList childlist = (ASTChildrenList)malloc(sizeof(ASTChildren));
        childlist->nodelist = child;
        childlist->next = NULL;
        if (node->children == NULL) 
        {
            node->children = childlist;
        } 
        else 
        {
            ASTChildrenList p = node->children;
            while (p->next != NULL) 
            {
                 p = p->next;
            }
            p->next = childlist;
        }
        child->parent = node;
    }
    return;
  
}

IdTableList findIdentifier(char a[]) 
{
    IdTableList p = IdHead;
    while (p != NULL) 
    {
         if (cmpstring(a, p->text)) 
         {
               return p;
         }
         p = p->next;
    }
    return p;
}

void insertIdNode(char a[], int value, int k) 
{
    IdTableList idNode = (IdTableList)malloc(sizeof(IdTable));
    strcpy(idNode->text, a);
    idNode->value = value;
    idNode->flag = k;
    idNode->next = IdHead;
    IdHead = idNode;
    return;
}

int hundred(int n)    //return 10^n
{
    int re = 1;
    if (n == 0) 
    {
         return 1;
    }
    while (n > 0) 
    {
        re *= 10;
        n--;
    }
    return re;
}

int literalToNumber(char a[])    //a = char 123 ,return int 123
{
    int lenth = strlen(a) - 1;
    int i = lenth;
    int b = 0;
    while (i >= 0) 
    {
        b += (int)(a[i] - '0') * hundred(lenth - i);
        i--;
    }
    return b;
}

// 加法表达式
ASTNodeList addtiveStatment() 
{
    ASTNodeList child1 = primary();
    ASTNodeList node = child1;
    TokenList tokentmp;

    while (1) 
    {
        tokentmp = pList;
        if (tokentmp != NULL && (tokentmp->type == Plus)) 
        {
            pList = pList->next;
            ASTNodeList child2 = primary();
            if (child2 != NULL) 
            {
                node = (ASTNodeList)malloc(sizeof(ASTNode));
                strcpy(node->text, tokentmp->text);
                node->nodetype = Addtive;
                node->children = NULL;
                node->parent = NULL;
                addASTNode(node, child1);
                addASTNode(node, child2);
                child1 = node;
            } 
            else 
            {
                printf("invalid Addtive expression,expecting the right part\n");
                node = NULL;
            }
        } 
        else 
        {
            break;
        }
    }
    return node;
}

// 大于小于等于表达式
ASTNodeList relStatement() 
{
    ASTNodeList child1 = addtiveStatment();
    ASTNodeList node = child1;
    TokenList tokentmp = pList;

    while (1) 
    {
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == GT) 
        {
            pList = pList->next;
            ASTNodeList child2 = addtiveStatment();
            if (child2 != NULL) 
            {
                node = (ASTNodeList)malloc((sizeof(ASTNode)));
                node->nodetype = GTASTNode;
                node->children = NULL;
                node->parent = NULL;
                strcpy(node->text, tokentmp->text);
                addASTNode(node, child1);
                addASTNode(node, child2);
                child1 = node;
            } 
            else 
            {
                printf("error:Lack of Rel after EQStatement\n");
                exit(0);
            }
        } 
        else if (tokentmp != NULL && tokentmp->type == LT) 
        {
            pList = pList->next;
            ASTNodeList child2 = addtiveStatment();
            if (child2 != NULL) 
            {
                node = (ASTNodeList)malloc((sizeof(ASTNode)));
                node->nodetype = LTASTNode;
                node->children = NULL;
                node->parent = NULL;
                strcpy(node->text, tokentmp->text);
                addASTNode(node, child1);
                addASTNode(node, child2);
                child1 = node;
            } 
            else 
            {
                printf("error:Lack of Rel after EQStatement\n");
                exit(0);
            }
        }
        else if (tokentmp != NULL && tokentmp->type == Equal) 
        {
            pList = pList->next;
            ASTNodeList child2 = addtiveStatment();
            if (child2 != NULL) 
            {
                node = (ASTNodeList)malloc((sizeof(ASTNode)));
                node->nodetype = EqualASTNode;
                node->children = NULL;
                node->parent = NULL;
                strcpy(node->text, tokentmp->text);
                addASTNode(node, child1);
                addASTNode(node, child2);
                child1 = node;
            } 
            else 
            {
                printf("error:Lack of Rel after EQStatement\n");
                exit(0);
            }
        } 
        else 
        {
          break;
        }
    }
    return node;
}

// 布尔表达式
ASTNodeList boolStatement() 
{
    TokenList tokentmp = pList;
    ASTNodeList node = (ASTNodeList)malloc(sizeof(ASTNode));
    node->nodetype = BooleASTNode;
    node->children = NULL;
    node->parent = NULL;
    strcpy(node->text, "Boole");

    ASTNodeList child1 = relStatement();
    addASTNode(node, child1);
    return node;
}

// 赋值语句
ASTNodeList assignmentStatment() 
{
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;

    if (tokentmp != NULL && tokentmp->type == Identifier) 
    {
        pList = pList->next;
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        node->nodetype = AssignmentASTnode;
        strcpy(node->text, tokentmp->text);
        node->children = NULL;
        node->parent = NULL;
        tokentmp = pList;
        TokenList tmp = pList;
        if (tokentmp != NULL && tokentmp->type == Assignment) 
        {
            pList = pList->next;
            ASTNodeList child = addtiveStatment();
            if (child == NULL) 
            {
                printf("invalide assignment statement in assignmentStatment,expecting ""semicolon\n");  
                node = NULL;
            } 
            else 
            {
                addASTNode(node, child);
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == SemiColon) 
                {
                    pList = pList->next;
                } 
                else 
                {
                    printf("invalid statement in assignmentStatment,expecting semicolon\n");                    
                }
            }
        } 
        else 
        {
            pList = tmp;
            node = NULL;
        }
    }
    return node;
}

ASTNodeList whileStatement()
{
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;
  
    if (tokentmp != NULL && tokentmp->type == While) 
    {
        pList = pList->next;
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        node->nodetype = WhileASTNode;
        strcpy(node->text, tokentmp->text);
        node->children = NULL;
        node->parent = NULL;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftParen) 
        {
            pList = pList->next;
            tokentmp = pList;
            ASTNodeList child = boolStatement();
            if (child == NULL) 
            {
                node = NULL;
            } 
            else 
            {
                addASTNode(node, child);
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == RightParen) 
                {
                    pList = pList->next;
                } 
                else 
                {
                    printf("invalid statement in whileStatment,expecting RightParen\n");                 
                }
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == LeftBrace) 
                {
                    pList = pList->next;
                } 
                else 
                {
                  printf("invalid statement in whileStatment,expecting LeftBrace\n");                      
                }
                ASTNodeList bodytmp=Body();
                addASTNode(node,bodytmp);
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == RightBrace) 
                {
                    pList = pList->next;
                } 
                else 
                {
                    printf("invalid statement in whileStatment,expecting RightBrace\n");              
                }
            } 
        }
        else 
        {
            printf("invalid statement in whileStatment,expecting LeftParen\n");
          
        } 
    }
    else 
    {
        node = NULL;
    }
    return node;
    //;
}

ASTNodeList ifStatement()
{
    ASTNodeList node = NULL,node2 = NULL;
    TokenList tokentmp = pList;
  
    if (tokentmp != NULL && tokentmp->type == If) 
    {
        pList = pList->next;
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        node->nodetype = IfASTNode;
        strcpy(node->text, tokentmp->text);
        node->children = NULL;
        node->parent = NULL;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftParen) 
        {
            pList = pList->next;
            tokentmp = pList;
            ASTNodeList child=boolStatement();
            if (child == NULL) 
            {
                node = NULL;
            } 
            else 
            {
                addASTNode(node, child);
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == RightParen) 
                {
                     pList = pList->next;
                } 
                else 
                {
                     printf("invalid statement in assignmentStatment,expecting RightParen\n");              
                }
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == LeftBrace) 
                {
                    pList = pList->next;
                } 
                else 
                {
                  printf("invalid statement in assignmentStatment,expecting LeftBrace\n");                    
                }
                ASTNodeList bodytmp=Body();
                addASTNode(node,bodytmp);
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == RightBrace) 
                {
                    pList = pList->next;
                } 
                else 
                {
                  printf("invalid statement in assignmentStatment,expecting RightBrace\n");  
                }
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == Else) 
                {
                    pList = pList->next;
                    node2 = (ASTNodeList)malloc(sizeof(ASTNode));
                    node2->nodetype = ElseASTNode;
                    strcpy(node2->text, tokentmp->text);
                    node2->children = NULL;
                    node2->parent = NULL;
                    tokentmp = pList;
                    if (tokentmp != NULL && tokentmp->type == LeftBrace) 
                    {
                        pList = pList->next;
                    } 
                    else 
                    {
                        printf("invalid statement in assignmentStatment,expecting LeftBrace\n");                      
                    }
                    bodytmp=Body();
                    addASTNode(node2,bodytmp);
                    tokentmp = pList;
                    if (tokentmp != NULL && tokentmp->type == RightBrace) 
                    {
                        pList = pList->next;
                    } 
                    else 
                    {
                        printf("invalid statement in assignmentStatment,expecting RightBrace\n");                      
                    }
                }
            }
        } 
        else 
        {
            printf("invalid statement in assignmentStatment,expecting LeftParen\n");          
        } 
        addASTNode(node,node2);  
    }
    else 
    {
      node = NULL;
    }
    return node; 
}

ASTNodeList showStatement()
{
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;
  
    if (tokentmp != NULL && tokentmp->type == Show) 
    {
        pList = pList->next;
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        node->nodetype = ShowASTNode;
        strcpy(node->text, tokentmp->text);
        node->children = NULL;
        node->parent = NULL;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftParen) 
        {
            pList = pList->next;
            tokentmp = pList;
            if (tokentmp != NULL && tokentmp->type == Identifier) 
            {
                ASTNodeList child1 = primary();
                ASTNodeList nodetmp=child1;
                while (1) 
                {
                    tokentmp = pList;
                    if (tokentmp != NULL && (tokentmp->type == Plus)) 
                    {
                        pList = pList->next;
                        ASTNodeList child2 = primary();
                        if (child2 != NULL) 
                        {
                            nodetmp = (ASTNodeList)malloc(sizeof(ASTNode));
                            strcpy(nodetmp->text, tokentmp->text);
                            nodetmp->nodetype = Addtive;
                            nodetmp->children = NULL;
                            nodetmp->parent = NULL;
                            addASTNode(nodetmp, child1);
                            addASTNode(nodetmp, child2);
                            child1 = nodetmp;
                        } 
                        else 
                        {
                            printf("invalid Addtive expression,expecting the right part\n");
                            node = NULL;
                        }
                    } 
                    else 
                    {
                        break;
                    }
                }
                if (child1 == NULL) 
                {
                    node = NULL;
                } 
                else 
                {
                    addASTNode(node, child1);
                    tokentmp = pList;
                    if (tokentmp != NULL && tokentmp->type == RightParen) 
                    {
                        pList = pList->next;
                    }
                    else 
                    {
                        printf("invalid statement in assignmentStatment,expecting RightParen\n");                
                    }
                    tokentmp = pList;
                    if (tokentmp != NULL && tokentmp->type == SemiColon) 
                    {
                        pList = pList->next;
                    }
                    else 
                    {
                        printf("invalid statement in assignmentStatment,expecting semicolon\n");                           
                    }
                }
            } 
        } 
        else 
        {
            printf("invalid statement in assignmentStatment,expecting LeftParen\n");         
        } 
    }
    else 
    {
        node = NULL;
    }
    return node;
}
//基础表达式
ASTNodeList primary() //解析整型字面量、标识符和括号的表达式，然后构建相应的抽象语法树节点
{
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;

    if (tokentmp != NULL) 
    {
        if (tokentmp->type == IntLiteral)
        {
            pList = pList->next;
            node = (ASTNodeList)malloc(sizeof(ASTNode));
            node->nodetype = IntLiteralASTnode;
            strcpy(node->text, tokentmp->text);
            node->children = NULL;
            node->parent = NULL;
        }
        else if (tokentmp->type == Identifier) 
        {
            pList = pList->next;
            node = (ASTNodeList)malloc(sizeof(ASTNode));
            node->nodetype = IdentifierASTnode;
            strcpy(node->text, tokentmp->text);
            node->children = NULL;
            node->parent = NULL;
        } 
        else if (tokentmp->type == LeftParen) 
        {
            pList = pList->next;
            node = relStatement();
            if (node != NULL) 
            {
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == RightParen) 
                {
                    pList = pList->next;
                }
                else 
                {
                    printf("expecting right parenthesis in expression\n");
                    node = NULL;
                }
            } 
            else 
            {
                printf("expecting an additive expression inside parenthesis in "
                    "expression\n");   
            node = NULL;
            }
        }
    }
    return node;
}

ASTNodeList intDeclare() 
{
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;

    if (tokentmp != NULL && tokentmp->type == Int) 
    {
        tokentmp = pList;
        pList = pList->next;
        if (pList != NULL && pList->type == Identifier) 
        {
            tokentmp = pList;
            pList = pList->next;
            node = (ASTNodeList)malloc(sizeof(ASTNode));
            strcpy(node->text, tokentmp->text);
            node->nodetype = IntDeclaration;
            node->children = NULL;
            node->parent = NULL;
            tokentmp = pList;
            if (tokentmp != NULL && tokentmp->type == Assignment) 
            {
                pList = pList->next;
                ASTNodeList child = addtiveStatment();
                if (child == NULL) 
                {
                    printf("invalide variable initialization,expecting an expression\n");
                    node = NULL;
                } 
                else 
                {
                    addASTNode(node, child);
                }
            }
        }  
        else 
        {
            printf("variable name expected!\n");
            node = NULL;
        }
    }
    return node;
}

// 遍历语法树，并打印语法树
void dumpAST(ASTNodeList p, int deepth) 
{
    if (p == NULL) 
    {
        return;
    }
    char a[100];
    memset(a, '\0', 100);
    for (int i = 0; i < deepth; i++) 
    {
        a[i] = '\t';
    }
    printf("%s%s\t%s\n", a, ASTNodeTypeString[p->nodetype], p->text);
    ASTChildrenList children = p->children;
    while (children != NULL) 
    {
        dumpAST(children->nodelist, deepth + 1);
        children = children->next;
    }
    return;
}

ASTNodeList intNodeList() 
{
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;

    if (pList != NULL && pList->type == Identifier) 
    {
        tokentmp = pList;
        pList = pList->next;
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = IntDeclaration;
        node->children = NULL;
        node->parent = NULL;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == Assignment) 
        {
            pList = pList->next;
            ASTNodeList child = addtiveStatment();
            if (child == NULL)
            {
                printf("invalide variable initialization,expecting an expression\n");
                node = NULL;
            } 
            else 
            {
            addASTNode(node, child);
            }
        }
    }
    else 
    {
        printf("variable name expected!\n");
        node = NULL;
    }
    return node;
}

ResultList evaluate(ASTNodeList node) 
{
    ResultList resultList = NULL;
    ASTChildrenList child = NULL;
    ResultList childList1 = NULL;
    ResultList childList2 = NULL;
    IdTableList tmpIdNode = NULL;
    ASTNodeList parent = NULL;
    int tmp, tmp1, tmpcontinue, tmpbreak;

    switch (node->nodetype)
    {
    case Program:
        child = node->children;
        while (child != NULL) 
        {
            evaluate(child->nodelist);
            child = child->next;
        }
        break;

    case BodyASTNode:
        child = node->children;
        while (child != NULL) 
        {
            evaluate(child->nodelist);
            child = child->next;
        }
        break;

    case IntDeclaration:
        child = node->children;
        if (child != NULL && continueSign == 1 && breakSign == 1) 
        {
            childList1 = evaluate(child->nodelist);
            insertIdNode(node->text, childList1->result, 1);
        } 
        else 
        {
            insertIdNode(node->text, 0, 0);
        }
        break;

    case IntLiteralASTnode:
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = literalToNumber(node->text);
        break;
 
    case IdentifierASTnode:
        tmpIdNode = findIdentifier(node->text);
        if (tmpIdNode == NULL) 
        {
            printf("%s not identified\n", node->text);
            exit(0);
        }
        if (tmpIdNode->flag == 0) 
        {
            printf("%s not assignment\n", node->text);
            exit(0);
        }
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmpIdNode->value;
        break;

    case LTASTNode:
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        child = child->next;
        tmp1 = evaluate(child->nodelist)->result;
        resultList = (ResultList)malloc(sizeof(Result));
        if (tmp < tmp1) 
        {
            resultList->result = 1;       
        }
        else 
        {
            resultList->result = 0;
        }
        break;

    case GTASTNode:
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        child = child->next;
        tmp1 = evaluate(child->nodelist)->result;
        resultList = (ResultList)malloc(sizeof(Result));
        if (tmp > tmp1) 
        {
            resultList->result = 1;
        }
        else 
        {
            resultList->result = 0;
        }
        break;
    case EqualASTNode:
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        child = child->next;
        tmp1 = evaluate(child->nodelist)->result;
        resultList = (ResultList)malloc(sizeof(Result));
        if (tmp == tmp1) 
        {
            resultList->result = 1;
        } 
        else
        {
            resultList->result = 0;
        }
        break;
    case Addtive:
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        child = child->next;
        tmp1 = evaluate(child->nodelist)->result;
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmp+tmp1;
        break;

    case AssignmentASTnode:
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        insertIdNode(node->text,tmp,1);
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmp;    
        break;

    case ShowASTNode:
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        printf("%d\n",tmp);
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmp;  
        break;

    case IfASTNode:
        child = node->children; 
        if(tmp = evaluate(child->nodelist)->result)
        {
            child = child->next;
            evaluate(child->nodelist);
        }
        else
        {
            if(child->next->next!=NULL)
            child = child->next->next;
            evaluate(child->nodelist)->result;
        }
        break;

    case ElseASTNode:
        child = node->children;
        evaluate(child->nodelist); 
        break;

    case WhileASTNode:
        child = node->children;
        while(tmp = evaluate(child->nodelist)->result)
        {
            evaluate(child->next->nodelist);
        }
        break;

    case BooleASTNode:
        child = node->children;
        resultList = evaluate(child->nodelist);
        break;

    default:
        break;
    }
    return resultList;
}

void varDeclarationBody(ASTNodeList node, TokenList tokentmp) 
{
    ASTNodeList child = intDeclare();
    if (child != NULL) 
    {
        addASTNode(node, child);
    } 
    else 
    {
        printf("error:IntDeclare\n");
        exit(0);
    }

    while (1) 
    {
        if (pList != NULL && pList->type == Dot) 
        {
            pList = pList->next;
            ASTNodeList child = intNodeList();
            if (child != NULL) 
            {
                addASTNode(node, child);
            }
            else
            {
                printf("error:IntDeclare\n");
                exit(0);
            }
        } 
        else if (pList != NULL && pList->type == SemiColon) 
        {
            pList = pList->next;
            break;
        } 
        else 
        {
            printf("invalide statement in varDeclaration,expecting semicolon\n");
        }
    }
}

void assignmentBody(ASTNodeList node, TokenList tokentmp) 
{
    ASTNodeList child = assignmentStatment();
    if (child != NULL)
    {
        addASTNode(node, child);
    } 
    else 
    {
        printf("error : assignmentStatment\n");
        exit(0);
    }
}

void showBody(ASTNodeList node, TokenList tokentmp) 
{
	ASTNodeList child = showStatement();
    if(child !=NULL)
    {
        addASTNode(node, child);
    }
    else 
    {
        printf("error : showStatment\n");
        exit(0);
    }
}

void whileBody(ASTNodeList node, TokenList tokentmp)
{
    ASTNodeList child = whileStatement();
    if(child !=NULL)
    {
        addASTNode(node, child);
    } 
    else 
    {
        printf("error : wileStatment\n");
        exit(0);
    }
}

void ifBody(ASTNodeList node, TokenList tokentmp) 
{
    ASTNodeList child = ifStatement();
    if(child !=NULL)
    {
        addASTNode(node, child);
    } 
    else 
    {
        printf("error : ifStatment\n");
        exit(0);
    }
}

ASTNodeList Body() 
{
    ASTNodeList node = NULL;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    node->nodetype = BodyASTNode;
    strcpy(node->text, "Body");
    node->children = NULL;
    node->parent = NULL;
    TokenList tokentmp = pList;
    ASTNodeList tempBody;

    if (tokentmp != NULL && tokentmp->type == Int) 
    {
        varDeclarationBody(node, tokentmp);
        tempBody = Body();
        addASTNode(node, tempBody);
    } 
    else if (tokentmp != NULL && tokentmp->type == Identifier) 
    {
        assignmentBody(node, tokentmp);
        tempBody = Body();
        addASTNode(node, tempBody);
    } 
    else if (tokentmp != NULL && tokentmp->type == If) 
    {
        ifBody(node, tokentmp);
        tempBody = Body();
        addASTNode(node, tempBody);
    } 
    else if (tokentmp != NULL && tokentmp->type == While) 
    {
        whileBody(node, tokentmp);
        tempBody = Body();
        addASTNode(node, tempBody);
    }
    else if (pList != NULL && pList->type == Show)
    {
        showBody(node, tokentmp);
        tempBody = Body();
        addASTNode(node, tempBody);
    } 
    else if (pList != NULL && pList->type == RightBrace)
    {
        return NULL;
    } 
    else 
    {
        printf("error:unknown statement in main body");
        exit(0);
    }
    return node;
}

ASTNodeList Programming() 
{
    TokenList tokentmp = pList;
    ASTNodeList node = NULL;

    if (tokentmp != NULL && tokentmp->type == Main) 
    {
        pList = pList->next;
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        node->nodetype = Program;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftParen) 
        {
            pList = pList->next;
            tokentmp = pList;
            if (tokentmp != NULL && tokentmp->type == RightParen) 
            {
                pList = pList->next;
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == LeftBrace) 
                {
                    pList = pList->next;
                    tokentmp = pList;
                    ASTNodeList child = Body();
                    if (child != NULL) 
                    {
                        addASTNode(node, child);
                        if (pList != NULL && pList->type == RightBrace) 
                        {
                            pList = pList->next;
                        }
                        else 
                        {
                            printf("error:Lack of RightBrace\n");
                            exit(0);
                        }
                    }
                }
                else 
                {
                    printf("error:Lack of LeftBrace\n");
                    exit(0);
                }
            } 
            else 
            {
                printf("error:Lack of RightParen\n");
                exit(0);
            }
        } 
        else 
        {
            printf("error:Lack of LeftParen\n");
            exit(0);
        }
    }
    return node;
}


IRList IR(ASTNodeList node)
{
    IRList irList = NULL;
    ASTChildrenList child = NULL;
    ASTChildrenList child1 = NULL;
    IRList childList1 = NULL;
    IRList childList2 = NULL;
    IdTableList tmpIdNode = NULL;
    char L1tmp[100];
    char L2tmp[100];
    char IRtmp[256];

    int L1 = 0;
    int L2 = 0;
    switch (node->nodetype)
    {
    case Program:
        child = node->children;
        while (child != NULL)
        {
            IR(child->nodelist);
            child = child->next;
        }
        break;

    case BodyASTNode:
        child = node->children;
        while (child != NULL)
        {
            IR(child->nodelist);
            child = child->next;
        }
        break;

    case AssignmentASTnode:

        tmpIdNode = findIdentifier(node->text);
        if (tmpIdNode == NULL)
        {
            printf("%s not identified\n", node->text);
            exit(0);
        }
        child = node->children;
        childList1 = IR(child->nodelist);
        if (childList1 != NULL)
        {
            printf("\t%s = %s\n", node->text, childList1->a);
            fprintf(IRFile, "\t%s = %s\n", node->text, childList1->a);
            tmpIdNode->flag = 1;
        }
        break;

    case IntDeclaration:
        child = node->children;
        if (child != NULL)
        {
            childList1 = IR(child->nodelist);
            insertIdNode(node->text, 0, 1);
            printf("\t%s = %s\n", node->text, childList1->a);
            fprintf(IRFile, "\t%s = %s\n", node->text, childList1->a);
        }
        else
        {
            insertIdNode(node->text, 0, 0);
        }
        break;

    case IdentifierASTnode:
        tmpIdNode = findIdentifier(node->text);
        if (tmpIdNode == NULL)
        {
            printf("%s not identified\n", node->text);
            exit(0);
        }
        if (tmpIdNode->flag == 0)
        {
            printf("%s not assignment\n", node->text);
            exit(0);
        }
        irList = (IRList)malloc(sizeof(IRnode));
        strcpy(irList->a, node->text);
        break;

    case IntLiteralASTnode:
        irList = (IRList)malloc(sizeof(IRnode));
        strcpy(irList->a, node->text);
        break;

    case Addtive:
        IRtmp[0] = 'T';
        sprintf(IRtmp + 1 , "%d", count_T);
        count_T++;
        irList = (IRList)malloc(sizeof(IRnode));
        strcpy(irList->a, IRtmp);
        
        child = node->children;
        childList1 = IR(child->nodelist);
        childList2 = IR(child->next->nodelist);
        printf("\t%s = %s + %s\n", IRtmp, childList1->a, childList2->a);
        break;

    case ShowASTNode:
        child = node->children;
        irList = NULL;
        strcpy(IRtmp, IR(child->nodelist)->a);
        printf("\t%s = %s + 0\n", IRtmp, IRtmp);
        break;

    case IfASTNode:
        child = node->children;
        childList1 = IR(child->nodelist);
        strcpy(IRtmp, childList1->a);

        printf("\tIFNZ %s GOTO L%d\n", IRtmp, count_L);
        L1tmp[0] = 'L';
        sprintf(L1tmp + 1, "%d", count_L);
        count_L++;
        child1 = child->next->next;
        IR(child1->nodelist);
        printf("\tGOTO L%d\n", count_L);
        L2tmp[0] = 'L';
        sprintf(L2tmp + 1, "%d", count_L);
        count_L++;
        printf("%s:\n", L1tmp);
        IR(child->next->nodelist);
        printf("%s:\n", L2tmp);
        break;
    case ElseASTNode:
        child = node->children;
        IR(child->nodelist);
        break;

    case LTASTNode:
        IRtmp[0] = 'T';
        sprintf(IRtmp + 1, "%d", count_T);
        count_T++;
        irList = (IRList)malloc(sizeof(IRnode));
        strcpy(irList->a, IRtmp);
        child = node->children;
        childList1 = IR(child->nodelist);
        childList2 = IR(child->next->nodelist);
        printf("\t%s = %s < %s\n", IRtmp, childList1->a, childList2->a);
        break;

    case GTASTNode:
        IRtmp[0] = 'T';
        sprintf(IRtmp + 1, "%d", count_T);
        count_T++;
        irList = (IRList)malloc(sizeof(IRnode));
        strcpy(irList->a, IRtmp);
        child = node->children;
        childList1 = IR(child->nodelist);
        childList2 = IR(child->next->nodelist);
        printf("\t%s = %s > %s\n", IRtmp, childList1->a, childList2->a);
        break;
    case BooleASTNode:
        child = node->children;
        irList = IR(child->nodelist);
        break;
    default:
        break;
    }
    return irList;
}

int main()
{
    sFile = fopen("test4.txt", "r");
    IRFile = fopen("./IR.txt", "w");

    if (sFile == NULL) {
        printf("打开文件失败!\n");
        return 0;
    }
    list = NULL;
    TokenReader();
    printf(
        "\n\n-语法分析\n\n"
        "--打印语法树\n");
    pList = list;
    ASTNodeList AST = Programming();

    if (AST != NULL) {
        dumpAST(AST, 0);
        printf("\n--显示show函数中的变量值\n");
        evaluate(AST);
        printf("\n--打印中间代码\n\n");
        IR(AST);
    }
    fclose(sFile);
    fclose(IRFile);
    return 0;
}
