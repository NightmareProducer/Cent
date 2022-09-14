#include <iostream>
#include <cctype>
#include <cerrno>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <queue>


enum class TokenType 
{
    // SINGLE CHARACTER TOKENS
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // ONE OR TWO CHARACTER TOKENS
    BANG, BANG_EQ,
    EQUAL, EQUAL_EQ,
    GREATER, GREATER_EQ,
    LESS, LESS_EQ,
    COMMENT,

    // LITERALS
    IDENTIFIER, STRING, NUMBER, CHAR,

    // KEYWORDS
    AND, CLASS, ELSE, FALSE, FUN, FOR, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    ENOF
};


struct Token 
{
    int line;
    TokenType type;
    std::string literal;
    std::string lexeme;
};


namespace ERRHAND
{
    enum class Phase { SCANNING };

    namespace Queue
    {
        namespace 
        {
            struct Errors 
            {
                int line;
                int column;
                Phase phase;
                std::string reason;
            };

            std::queue<Errors> s_errors;
        }

        void push(int p_line, int p_column, std::string p_reason, Phase p_phase)
        {
            s_errors.emplace(p_line, p_column, p_phase, p_reason);
        }

        void consume()
        {
            // Pop the queue and print the output
        }

        auto count() { return s_errors.size(); }
    }

    // Immediately close the program.
    void quit(std::string_view reason)
    {
        std::cerr << reason << std::endl;
        throw errno;
    }
}


namespace Tools {
    std::string file2string(std::string p_fp)
    {
        std::ifstream f(p_fp, std::ios::in | std::ios::binary);
        if (f.is_open()) 
        {
            std::string content;

            // Get file length
            f.seekg(0, std::ios::end);
            content.resize(f.tellg());
            f.seekg(0, std::ios::beg);

            f.read(&content[0], content.size());
            return content;
        }
        throw errno;
    }
}


namespace Scanner 
{
    std::vector<Token> scan(std::string p_source) 
    {
        int s_linenum {0};
        int s_column {0};
        std::vector<Token> tokens;

        for(std::size_t i = 0; i < p_source.length(); ++i) 
        {
            char c = p_source[i];

            std::string lexeme_buf {c};

            const auto add_token = [&](TokenType p_type, std::string p_literal = "")
            {
                tokens.emplace_back(s_linenum, p_type, p_literal, lexeme_buf);
            };

            const auto at_end = [&i, &p_source]() 
            {
                return i >= p_source.length();
            };

            const auto consume = [&c, &i, &p_source, &at_end]() 
            { 
                return (at_end()) ? (char) EOF : (c = p_source[++i], c);
            };

            const auto peek = [&i, &p_source, &at_end]()
            {
                return (at_end()) ? (char) EOF : p_source[i + 1];
            };

            const auto next_match = [&i, &consume, &at_end](char p_expected) 
            {
                return (at_end()) ? false : consume() == p_expected;
            };

            ++s_column;

            switch (c)
            {
                case '(':
                    add_token(TokenType::LEFT_PAREN);
                    break;
                case ')':
                    add_token(TokenType::RIGHT_PAREN);
                    break;
                case '{':
                    add_token(TokenType::LEFT_BRACE);
                    break;
                case '}':
                    add_token(TokenType::RIGHT_BRACE);
                    break;
                case '*':
                    add_token(TokenType::STAR);
                    break;
                case '+':
                    add_token(TokenType::PLUS);
                    break;
                case '-':
                    add_token(TokenType::MINUS);
                    break;
                case '.':
                    add_token(TokenType::DOT);
                    break;
                case ',':
                    add_token(TokenType::COMMA);
                    break;
                case ';':
                    add_token(TokenType::SEMICOLON);
                    break;


                case '/':
                    if (next_match('/')) 
                    {
                        add_token(TokenType::COMMENT);
                        while(consume() != '\n');
                    }
                    else 
                        add_token(TokenType::SLASH);

                    break;
                case '=':
                    add_token(next_match('=') ? TokenType::EQUAL_EQ : TokenType::EQUAL);
                    break;
                case '<':
                    add_token(next_match('=') ? TokenType::LESS_EQ : TokenType::LESS);
                    break;
                case '>':
                    add_token(next_match('=') ? TokenType::GREATER_EQ : TokenType::GREATER);
                    break;
                case '!':
                    add_token(next_match('=') ? TokenType::BANG_EQ : TokenType::BANG);
                    break;
                

                case '"':
                    { // Prevent automatic storage duration issue with local scope
                    std::string _literal;

                    // Move forward the cursor index till we found a closing string literal
                    while(consume() != '"' && c != EOF) 
                        _literal += c;

                    // No closing string literal found
                    if(c == EOF) {
                        ERRHAND::Queue::push(s_linenum, s_column, "Unterminated string", ERRHAND::Phase::SCANNING);
                        break;
                    }

                    // Example _literal = "\"hello world\""
                    lexeme_buf += _literal + c; 

                    add_token(TokenType::STRING, _literal);
                    break;
                    }

                // Ignore white spaces
                case ' ':
                case '\r':
                case 't':
                    break;
                case '\n':
                    ++s_linenum;
                    s_column = 0;
                    break;
                
                case 0: case 1: case 2: case 3: case 4: 
                case 5: case 6: case 7: case 8: case 9:
                    while (std::isdigit(consume()))
                        lexeme_buf += c;

                    // Is a decimal
                    if(c == '.')
                    {
                        lexeme_buf += c;

                        while (std::isdigit(consume()))
                            lexeme_buf += c;
                    }

                    add_token(TokenType::NUMBER, lexeme_buf);

                    break;


                default:
                    ERRHAND::Queue::push(s_linenum, s_column, "Unrecognize Character", ERRHAND::Phase::SCANNING);
            }
        }

        return tokens;
    }
}


int main(int argc, char *argv[]) 
{
    if (argc == 2)
    {
        Scanner::scan(Tools::file2string(argv[1]));
    }
    return 0;
}
