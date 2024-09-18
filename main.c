#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//using union to make stack able to store floats or characters
//it stores different data types in the same memory
//so either access float or characters
//when you will access anything of type data you've to write .c(character) or.f(floats)

typedef union
{
    float f;
    char c;
} Data;

typedef struct Node
{
    Data data ;
    struct Node *next;
} Node;

typedef struct
{
    Node *top;
} stack;


stack *initialize()
{
    stack *s=malloc(sizeof(stack));
    s->top=NULL;
    return s;
}

Node *newnode(Data value)
{
    Node *n=malloc(sizeof(Node));
    n->data=value;
    n->next=NULL;
    return n;
}

void push(stack *s,Data value)
{
    Node *n=newnode(value);
    if(s->top==NULL)
    {
        s->top=n;
    }
    else
    {
        n->next=s->top;
        s->top=n;
    }
}

Data pop(stack *s)
{
    Data x;
    if(s->top->next==NULL)
    {
        x=s->top->data;
        free(s->top);
        s->top=NULL;
    }
    else
    {
        x=s->top->data;
        Node *temp=s->top;
        s->top=s->top->next;
        free(temp);
    }
    return x;
}



int isEmpty(stack *s)
{
    if(s->top==NULL)
        return 1;
    return 0;
}

Data Top(stack *s)
{
    if(!isEmpty(s))
    {
        return s->top->data;
    }
}

int priority(char c)
{
    switch(c)
    {
    case '(':
        return 3;
    case '^':
        return 2;
    case '*':
    case '/':
    case '%':
        return 1;
    case '+':
    case '-':
        return 0;
    }
}

int isNumber(char *num)
{
    int i=0;
    int flag=1;//is number
    int decimal=0;
    int digits=0;
    int negatives=0;
    while(num[i]!=NULL)
    {
        if(num[i]=='-')
            negatives++;
        if(num[i]=='.')
            decimal++;
        if(isdigit(num[i]))
            digits=1;//flag
        if(!isdigit(num[i])&&num[i]!='.'&&num[i]!='-')
            flag=0;
        i++;
    }
    if(negatives>1||decimal>1)
        flag=0;
    if((negatives==1&&digits==0)||(decimal==1&&digits==0))
        flag=0;
    return flag;
}

int validate(char *infix)
{
    int i,n;
    n=strlen(infix);
    for(i=0; i<n; i++)
    {
        if(infix[i]>='a'&&infix[i]<='z'||infix[i]>='A'&&infix[i]<='Z')
            return 0;
    }
    return 1;
}

char* infixTopostfix (char* infix)
{
    Data x;
    char* post=malloc(strlen(infix)+1);
    stack *s=initialize();
    char delim[]=" \n";
    char *token=strtok(infix,delim);
    strcpy(post,"");
    while(token!=NULL)
    {
        if(isNumber(token))
        {
            strcat(post,token);
            strcat(post," ");    //because we want space after each number or operator
        }
        else if(token[0]==')')
        {
            while(!isEmpty(s) && Top(s).c!='(')
            {
                char ch=pop(s).c;
                char str[3];
                str[0]=ch;
                str[1]=' ';
                str[2]='\0';        //str should end with \0 so that strcat knows the end of the second string
                strcat(post, str);  //it concatenates two strings so str helped us to change from char to string
            }
            pop(s);
        }
        else if(isEmpty(s))
        {
            x.c=token[0]; //token[0] because strtok returns char * and we want the first char only
            push(s,x);
        }
        else
        {
            while(!isEmpty(s) && priority(Top(s).c)>=priority(token[0]) && Top(s).c!='(')
            {
                char ch=pop(s).c;
                char str[3];
                str[0]=ch;
                str[1]=' ';
                str[2]='\0';
                strcat(post,str);
            }
            x.c=token[0];//so we can push x (which is of type Data)
            push(s,x);
        }
        token=strtok(NULL,delim);
    }
    while(!isEmpty(s))
    {
        char ch=pop(s).c;
        char str[3];
        str[0]=ch;
        str[1]=' ';
        str[2]='\0';
        strcat(post,str);
    }
    return post;
}



float evaluate(char x, float op1, float op2)
{
    if(x=='+')
        return (op1+op2);
    if(x=='-')
        return (op1-op2);
    if(x=='*')
        return (op1*op2);
    if(x=='/')
    {
        if (op2==0)
        {
            printf("Error: cant divide by zero");
        }
        return (op1/op2);
    }
    if(x=='%')
        return fmod(op1,op2);
    if(x=='^')
        return pow(op1,op2);
    else
        printf("Error: Invalid operator\n");


}

float evaluatePostfix(char* postfix)
{
    stack *s=initialize();
    char *token = strtok(postfix, " ");
    float op1,op2,value;
    Data val;
    Data num;
    while(token!=NULL)
    {
        if(isNumber(token))
        {
            num.f=atof(token); // if token is number convert it to float
            push(s,num);
        }

        else
        {
            op2=pop(s).f;
            op1=pop(s).f;
            value=evaluate(token[0],op1,op2);
            val.f=value;
            push(s,val);
        }
        token=strtok(NULL," ");// get next token

    }
    value=pop(s).f;
    return value;

}


int main()
{
    stack*s=initialize();
    char expression[200];
    printf("Enter an Infix expression: ");
    fgets(expression,200,stdin);
    if(validate(expression)==1)
    {
        char *postfix=infixTopostfix(expression);
        printf("%s",postfix);
        float result=evaluatePostfix(postfix);
        printf("\nThe Value is: %.3f",result);
    }
    else
        printf("Invalid infix expression");

    return 0;
}
