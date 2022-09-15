#include <iostream>
#include <cctype>
#include <cerrno>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <queue>


namespace Spec {
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
        AND, OR, CLASS, IF, ELSE, VAR, FUNC, FOR, WHILE,  
        RETURN, PRINT, SUPER, THIS, TRUE, FALSE, NIL,

        ENOF
    };


    struct Token 
    {
        int line;
        TokenType type;
        std::string literal;
        std::string lexeme;
    };

    namespace Expr 
    { 
        enum class ExpressionType 
        {
            NUMBER, STRING, BOOL, NIL, 
            UNARY, BINARY, GROUPING, OPERATOR
        };


        struct Expression
        {
            ExpressionType type;
            std::string content;
        };


        enum class BinaryOp 
        {
            // Arith
            MUL, DIV, PLUS, MINUS,                              

            // Logical
            EQUAL, NOT_EQ, 
            GREATER, GREATER_EQ, 
            LESS, LESS_EQ,   
        };

        enum class UnaryOp {NOT, MINUS};


        Expression Binary(Expression p_left, BinaryOp p_op, Expression p_right) noexcept
        {
            std::string op;
            switch (p_op)
            {
                case BinaryOp::MUL:         op = "*"; break;
                case BinaryOp::DIV:         op = "/"; break;
                case BinaryOp::PLUS:        op = "+"; break;
                case BinaryOp::MINUS:       op = "-"; break;
                case BinaryOp::EQUAL:       op = "="; break;
                case BinaryOp::NOT_EQ:      op = "!="; break;
                case BinaryOp::GREATER:     op = ">"; break;
                case BinaryOp::GREATER_EQ:  op =">="; break;
                case BinaryOp::LESS:        op = "<"; break;
                case BinaryOp::LESS_EQ:     op = "<="; break;
            }

            return {ExpressionType::BINARY, p_left.content + op + p_right.content};
        }


        Expression Unary(UnaryOp p_op, Expression p_expr)
        {
            std::string op;

            switch (p_op)
            {
                case UnaryOp::NOT: op = "!"; break;
                case UnaryOp::MINUS: op = "-"; break;
            }

            return {ExpressionType::UNARY, op + p_expr};
        }


        Expression Number(double num)
        {
            return {ExpressionType::NUMBER, std::to_string(num)};
        }

        
        Expression String(std::string str)
        {
            return {ExpressionType::STRING, str};
        }


        Expression Bool(bool boolean)
        {
            return {ExpressionType::BOOL, std::to_string(boolean)};
        }


        Expression Nil()
        {
            return {ExpressionType::NIL, "nil"};
        }
    }


    std::unordered_map<std::string_view, TokenType> s_reserved {
        {"and", TokenType::AND},
        {"or", TokenType::OR},
        {"class", TokenType::CLASS},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"var", TokenType::VAR},
        {"func", TokenType::FUNC},
        {"for", TokenType::FOR},
        {"while", TokenType::WHILE},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"print", TokenType::PRINT},
        {"nil", TokenType::NIL},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
    };


    TokenType reserved(std::string_view word) noexcept 
    { 
        try 
        {
            Spec::s_reserved;
            return s_reserved.at(word);
        } 
        catch(const std::out_of_range& e)
        {
            return TokenType::IDENTIFIER;
        }
    }
}


namespace ERRHAND
{
    enum class Phase { SCANNING };

    namespace Queue
    {
        struct Errors 
        {
            int line;
            int column;
            Phase phase;
            std::string reason;
        };

        std::queue<Errors> s_errors;

        void push(int p_line, int p_column, std::string p_reason, Phase p_phase) noexcept
        {
            s_errors.emplace(p_line, p_column, p_phase, p_reason);
        }

        void consume() noexcept
        {
            // Pop the queue and print the output
        }

        auto count() noexcept { return s_errors.size(); }
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
    using namespace Spec;

    std::vector<Token> scan(std::string p_source) noexcept
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
                
                case '0': case '1': case '2': case '3': case '4': 
                case '5': case '6': case '7': case '8': case '9':
                    { // Prevent automatic storage duration issue with local scope
                    std::string _literal {c};

                    while (std::isdigit(consume()) || c == '_') 
                    {
                        if (c != '_') _literal += c;
                        lexeme_buf += c;
                    }

                    // Is a decimal
                    if(c == '.')
                    {
                        lexeme_buf += c;
                        _literal += c;

                        while (std::isdigit(consume())) 
                        {
                            lexeme_buf += c;
                            _literal += c;
                        }
                    }

                    add_token(TokenType::NUMBER, _literal);
                    }

                    break;


                default:
                    if(std::isalpha(c))
                    {
                        while(std::isalnum(consume())) 
                            lexeme_buf += c;

                       add_token(Spec::reserved(lexeme_buf));
                    } else
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
    } else if (argc == 1)
    {
        std::cout << "> ";
        for(std::string line; std::getline(std::cin, line);)
        {
            Scanner::scan(line);
            std::cout << "> ";
        }
    }
    
    return 0;
}
