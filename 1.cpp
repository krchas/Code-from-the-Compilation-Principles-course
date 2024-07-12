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
        /*  TO DO  */
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        child = child->next;
        tmp1 = evaluate(child->nodelist)->result;
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmp + tmp1;
        /*  提供int cmpstring(char a[], char b[])函数可调用，具体用法看定义  */
        break;

    case AssignmentASTnode:
        /*  TO DO  */
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        insertIdNode(node->text, tmp, 1);
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmp;
        /* 提供IdTableList findIdentifier(char a[])函数可用，具体用法看定义  */
        break;

    case ShowASTNode:
        /*  TO DO  */
        child = node->children;
        tmp = evaluate(child->nodelist)->result;
        printf("%d\n", tmp);
        resultList = (ResultList)malloc(sizeof(Result));
        resultList->result = tmp;
        break;

    case IfASTNode:
        /*  TO DO  */
        child = node->children;
        if (tmp = evaluate(child->nodelist)->result)
        {
            child = child->next;
            evaluate(child->nodelist);
        }
        else
        {
            if (child->next->next != NULL)
                child = child->next->next;
            evaluate(child->nodelist)->result;
        }
        // resultList = (ResultList)malloc(sizeof(Result));
        // resultList->result = tmp; 
        break;

    case ElseASTNode:
        /*  TO DO  */
        child = node->children;
        evaluate(child->nodelist);
        break;

    case WhileASTNode:
        child = node->children;
        while (tmp = evaluate(child->nodelist)->result)
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