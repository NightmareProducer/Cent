#include "Scanner.h"
#include "Token.h"
#include "Error.h"

#include <cctype>
#include <fstream>


namespace Cent::Tool
{
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
} // namespace Cent


namespace Cent::Scanner
{
    using namespace Cent::Type;
    using namespace Cent::Constant;

    TokenList scan(std::string p_source) noexcept
    {
        int s_linenum {0};
        int s_column {0};
        TokenList tokens;

        for(std::size_t index = 0; index < p_source.length(); ++index) 
        {
            char c = p_source[index];

            std::string lexeme_buf {c};

            const auto add_token = [&](TokenType p_type, std::string p_literal = "")
            {
                tokens.push_back(std::move(Token(p_type, lexeme_buf, p_literal, s_linenum, s_column)));
            };

            const auto at_end = [&index, &p_source]() 
            {
                return index >= p_source.length();
            };

            const auto consume = [&c, &index, &p_source, &at_end]() 
            { 
                return (at_end()) ? (char) EOF : (c = p_source[++index], c);
            };

            const auto peek = [&index, &p_source, &at_end]()
            {
                return (at_end()) ? (char) EOF : p_source[index + 1];
            };

            const auto next_match = [&index, &consume, &at_end](char p_expected) 
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
                        Cent::Error::enqueue(s_linenum, s_column, "Unterminated string", Constant::Phase::SCANNING);
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
                    auto _token_type = TokenType::INT;

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
                        _token_type = TokenType::FLOAT;

                        while (std::isdigit(consume())) 
                        {
                            lexeme_buf += c;
                            _literal += c;
                        }
                    }

                    add_token(_token_type, _literal);
                    }

                    break;


                default:
                    if(std::isalpha(c))
                    {
                        while(std::isalnum(consume())) 
                            lexeme_buf += c;

                    add_token(Tool::reserved(lexeme_buf));
                    } else
                        Error::enqueue(s_linenum, s_column, "Unrecognize Character", Constant::Phase::SCANNING);
            }
        }

        return tokens;
    }

} // namespace Scanner
