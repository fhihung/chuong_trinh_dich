#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

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

// Hàm chính để phân tích một đoạn mã và trả về các token
void tokenize(const char *code)
{
    State state = START;
    char lexeme[MAX_TOKEN_LENGTH];
    int lexeme_index = 0;

    // Biểu thức chính quy để nhận dạng các loại token
    regex_t regex_identifier, regex_number, regex_operator, regex_delimiter;
    regcomp(&regex_identifier, "[a-zA-Z_][a-zA-Z0-9_]*", REG_EXTENDED);
    regcomp(&regex_number, "[0-9]+", REG_EXTENDED);
    regcomp(&regex_operator, "[+\\-*/%=&|><!^~]", REG_EXTENDED);
    regcomp(&regex_delimiter, "[(){}\\[\\];,:]", REG_EXTENDED); // Sửa biểu thức này

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
            else if (regexec(&regex_operator, &current_char, 1, NULL, 0) == 0)
            {
                state = IN_OPERATOR;
                lexeme[lexeme_index++] = current_char;
            }
            else if (regexec(&regex_delimiter, &current_char, 1, NULL, 0) == 0) // Sửa dòng này
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
                printf("Identifier: %s\n", lexeme);
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
            if (regexec(&regex_operator, &current_char, 1, NULL, 0) == 0)
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
            if (regexec(&regex_delimiter, &current_char, 1, NULL, 0) == 0)
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

    // Giải phóng bộ nhớ cho biểu thức chính quy
    regfree(&regex_identifier);
    regfree(&regex_number);
    regfree(&regex_operator);
    regfree(&regex_delimiter);
}

int main()
{
    char code[] = "int main() { return 2; }";
    tokenize(code);
    return 0;
}
