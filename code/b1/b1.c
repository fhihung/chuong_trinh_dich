#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100

// Enum để đại diện cho các trạng thái của DFA
typedef enum
{
    START,
    IN_IDENTIFIER,
    IN_NUMBER,
    IN_OPERATOR,
    IN_DELIMITER,
    DONE
} State;

// Các hằng số định danh các loại token
#define KEYWORD "Keyword"
#define IDENTIFIER "Identifier"
#define OPERATOR "Operator"
#define LITERAL "Literal"
#define DELIMITER "Delimiter"

// Các hàm chức năng
int is_operator(char c)
{
    const char operators[] = "+-*/%=&|><!^~";
    return strchr(operators, c) != NULL;
}

int is_delimiter(char c)
{
    const char delimiters[] = "()[]{};,:";
    return strchr(delimiters, c) != NULL;
}

int is_keyword(const char *word)
{
    const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; ++i)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// Hàm chính để phân tích một đoạn mã và trả về các token
void tokenize(const char *code)
{
    State state = START;
    char lexeme[MAX_TOKEN_LENGTH];
    int lexeme_index = 0;

    for (int i = 0; code[i] != '\0'; ++i)
    {
        char current_char = code[i];

        switch (state)
        {
        case START:
            if (isalpha(current_char) || current_char == '_')
            {
                state = IN_IDENTIFIER;
                lexeme[lexeme_index++] = current_char;
            }
            else if (isdigit(current_char))
            {
                state = IN_NUMBER;
                lexeme[lexeme_index++] = current_char;
            }
            else if (is_operator(current_char))
            {
                state = IN_OPERATOR;
                lexeme[lexeme_index++] = current_char;
            }
            else if (is_delimiter(current_char))
            {
                state = IN_DELIMITER;
                lexeme[lexeme_index++] = current_char;
            }
            break;
        case IN_IDENTIFIER:
            if (isalnum(current_char) || current_char == '_')
            {
                lexeme[lexeme_index++] = current_char;
            }
            else
            {
                lexeme[lexeme_index] = '\0';
                if (is_keyword(lexeme))
                {
                    printf("Keyword: %s\n", lexeme);
                }
                else
                {
                    printf("Identifier: %s\n", lexeme);
                }
                lexeme_index = 0;
                state = START;
                --i; // Re-process current character
            }
            break;
        case IN_NUMBER:
            if (isdigit(current_char))
            {
                lexeme[lexeme_index++] = current_char;
            }
            else
            {
                lexeme[lexeme_index] = '\0';
                printf("Number: %s\n", lexeme);
                lexeme_index = 0;
                state = START;
                --i; // Re-process current character
            }
            break;
        case IN_OPERATOR:
            if (is_operator(current_char))
            {
                lexeme[lexeme_index++] = current_char;
            }
            else
            {
                lexeme[lexeme_index] = '\0';
                printf("Operator: %s\n", lexeme);
                lexeme_index = 0;
                state = START;
                --i; // Re-process current character
            }
            break;
        case IN_DELIMITER:
            if (is_delimiter(current_char))
            {
                lexeme[lexeme_index++] = current_char;
            }
            else
            {
                lexeme[lexeme_index] = '\0';
                printf("Delimiter: %s\n", lexeme);
                lexeme_index = 0;
                state = START;
                --i; // Re-process current character
            }
            break;
        case DONE:
            // Should never reach here
            break;
        }
    }

    // Handling the last token
    if (state == IN_IDENTIFIER)
    {
        lexeme[lexeme_index] = '\0';
        if (is_keyword(lexeme))
        {
            printf("Keyword: %s\n", lexeme);
        }
        else
        {
            printf("Identifier: %s\n", lexeme);
        }
    }
    else if (state == IN_NUMBER)
    {
        lexeme[lexeme_index] = '\0';
        printf("Number: %s\n", lexeme);
    }
    else if (state == IN_OPERATOR)
    {
        lexeme[lexeme_index] = '\0';
        printf("Operator: %s\n", lexeme);
    }
    else if (state == IN_DELIMITER)
    {
        lexeme[lexeme_index] = '\0';
        printf("Delimiter: %s\n", lexeme);
    }
}

int main()
{
    char code[] = "int main() { return 3; }";
    tokenize(code);
    return 0;
}