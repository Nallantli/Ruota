#include "Ruota.h"

using namespace ruota;

Lexer::Lexer()
{}

const char Lexer::nextChar()
{
	auto c = INPUT[INPUT_INDEX++];
	if (c == '\n') {
		LINE_INDEX++;
		TOKEN_DIST = 0;
	} else {
		TOKEN_DIST++;
	}
	return c;
}

const char Lexer::peekChar(const size_t &i) const
{
	if (INPUT_INDEX + i < INPUT.size())
		return INPUT[INPUT_INDEX + i];
	return 0;
}

const int Lexer::getToken()
{
	static int last;
	while (isspace(last = nextChar()))
		;

	if (isalpha(last) || last == '_') {
		ID_STRING = last;
		while (isalnum(peekChar(0)) || peekChar(0) == '_') {
			last = nextChar();
			ID_STRING += last;
		}

		if (ID_STRING == "then")
			return TOK_THEN;
		else if (ID_STRING == "else")
			return TOK_ELSE;
		else if (ID_STRING == "do")
			return TOK_DO;
		else if (ID_STRING == "in")
			return TOK_IN;
		else if (ID_STRING == "var")
			return TOK_VAR;
		else if (ID_STRING == "if")
			return TOK_IF;
		else if (ID_STRING == "while")
			return TOK_WHILE;
		else if (ID_STRING == "var")
			return TOK_VAR;
		else if (ID_STRING == "elif")
			return TOK_ELSEIF;
		else if (ID_STRING == "for")
			return TOK_FOR;
		else if (ID_STRING == "true")
			return TOK_TRUE;
		else if (ID_STRING == "false")
			return TOK_FALSE;
		else if (ID_STRING == "return")
			return TOK_RETURN;
		else if (ID_STRING == "nil")
			return TOK_NIL;
		else if (ID_STRING == "Number")
			return TOK_NUMBER;
		else if (ID_STRING == "String")
			return TOK_STRING;
		else if (ID_STRING == "Array")
			return TOK_ARRAY;
		else if (ID_STRING == "Boolean")
			return TOK_BOOLEAN;
		else if (ID_STRING == "Dictionary")
			return TOK_DICTIONARY;
		else if (ID_STRING == "Object")
			return TOK_OBJECT;
		else if (ID_STRING == "Function")
			return TOK_FUNCTION;
		else if (ID_STRING == "Type")
			return TOK_TYPE_NAME;
		else if (ID_STRING == "extern")
			return TOK_EXTERN;
		else if (ID_STRING == "extern_call")
			return TOK_EXTERN_CALL;
		else if (ID_STRING == "size")
			return TOK_SIZE;
		else if (ID_STRING == "length")
			return TOK_LENGTH;
		else if (ID_STRING == "struct")
			return TOK_STRUCT;
		else if (ID_STRING == "static")
			return TOK_STATIC;
		else if (ID_STRING == "class")
			return TOK_CLASS;
		else if (ID_STRING == "new")
			return TOK_NEW;
		else if (ID_STRING == "load")
			return TOK_LOAD;
		else if (ID_STRING == "alloc")
			return TOK_ALLOC;
		else if (ID_STRING == "ref")
			return TOK_REF;
		else if (ID_STRING == "break")
			return TOK_BREAK;
		else if (ID_STRING == "refer")
			return TOK_REFER;
		else if (ID_STRING == "Nil")
			return TOK_NIL_NAME;
		else if (ID_STRING == "Pointer")
			return TOK_POINTER;
		else if (ID_STRING == "virtual")
			return TOK_VIRTUAL;
		else if (ID_STRING == "switch")
			return TOK_SWITCH;
		else if (ID_STRING == "try")
			return TOK_TRY;
		else if (ID_STRING == "catch")
			return TOK_CATCH;
		else if (ID_STRING == "throw")
			return TOK_THROW;
		else if (ID_STRING == "charn")
			return TOK_CHARN;
		else if (ID_STRING == "chars")
			return TOK_CHARS;
		else if (ID_STRING == "case")
			return TOK_CASE;
		else if (ID_STRING == "parse")
			return TOK_PARSE;
		else if (ID_STRING == "inf") {
			NUM_VALUE = CNumber::Double(INFINITY);
			return TOK_NUM;
		} 		else if (ID_STRING == "nan") {
			NUM_VALUE = CNumber::Double(NAN);
			return TOK_NUM;
		} 		else if (Ruota::bOperators.find(ID_STRING) != Ruota::bOperators.end() || Ruota::uOperators.find(ID_STRING) != Ruota::uOperators.end())
			return TOK_OPR;

		return TOK_IDF;
	} else if (isdigit(last) || (last == '.' && isdigit(peekChar(0)))) {
		if (last == '0' && isalpha(peekChar(0))) {
			char base = nextChar();
			std::string numStr = "";
			while (isalnum(peekChar(0))) {
				last = nextChar();
				numStr += last;
			}
			switch (base)
			{
			case 'b':
			case 'B':
				NUM_VALUE = CNumber::Long(std::stoll(numStr, nullptr, 2));
				break;
			default:
				NUM_VALUE = CNumber::Long(std::stoll("0" + std::string(1, base) + numStr, nullptr, 0));
				break;
			}
			ID_STRING = "0" + std::string(1, base) + numStr;
			return TOK_NUM;
		}

		std::string numStr = std::string(1, last);
		bool flag = false;
		while (isdigit(peekChar(0)) || (peekChar(0) == '.' && isdigit(peekChar(1)))) {
			if (flag && peekChar(0) == '.')
				break;
			if (!flag)
				flag = last == '.';
			last = nextChar();
			numStr += last;
		}

		ID_STRING = numStr;
		if (flag)
			NUM_VALUE = CNumber::Double(std::stold(numStr));
		else
			NUM_VALUE = CNumber::Long(std::stoll(numStr, nullptr, 10));
		return TOK_NUM;
	} else if (last == '#') {
		std::string commentStr = "";
		do {
			last = nextChar();
			commentStr += last;
		} while (last != EOF && last != '\n' && last != '\r');

		ID_STRING = commentStr;
		return '#';
	} else if (last == EOF || last == 0)
		return TOK_EOF;
	else if (Ruota::bOperators.find(std::string(1, last)) != Ruota::bOperators.end()) {
		std::string opStr = std::string(1, last);
		while (Ruota::bOperators.find(opStr + peekChar(0)) != Ruota::bOperators.end()) {
			last = nextChar();
			opStr += last;
		}

		ID_STRING = opStr;
		if (ID_STRING == "->")
			return TOK_CAST;
		if (ID_STRING == "=>")
			return TOK_DEF;
		if (ID_STRING == "::")
			return TOK_DEF_TYPE;
		if (ID_STRING == ".")
			return TOK_INNER;
		if (ID_STRING == "..")
			return TOK_UNTILF;
		if (ID_STRING == ".+")
			return TOK_UNTILT;
		if (ID_STRING == ":")
			return ':';
		return TOK_OPR;
	} else if (Ruota::uOperators.find(std::string(1, last)) != Ruota::uOperators.end()) {
		std::string opStr = std::string(1, last);
		while (Ruota::uOperators.find(opStr + peekChar(0)) != Ruota::uOperators.end()) {
			last = nextChar();
			opStr += last;
		}

		ID_STRING = opStr;
		return TOK_OPR;
	} else if (last == '`') {
		std::string value = "";
		while (true) {
			last = nextChar();
			if (last == '`') {
				ID_STRING = value;
				return TOK_IDF;
			}
			value += last;
		}
	} else if (last == '"') {
		std::string value = "";
		while (true) {
			last = nextChar();
			if (last == '"') {
				ID_STRING = value;
				return TOK_STR_LIT;
			} else if (last == '\\') {
				switch (last = nextChar()) {
					case '"':
						last = '"';
						break;
					case '\'':
						last = '\'';
						break;
					case 'n':
						last = '\n';
						break;
					case '?':
						last = '\?';
						break;
					case 'a':
						last = '\a';
						break;
					case 'b':
						last = '\b';
						break;
					case 'f':
						last = '\f';
						break;
					case 'r':
						last = '\r';
						break;
					case 't':
						last = '\t';
						break;
					case 'v':
						last = '\v';
						break;
					case '0':
						last = '\0';
						break;
					case 'x':
					{
						std::string code = std::string({ nextChar(), nextChar() });
						char hex = std::stoul(code, nullptr, 16);
						last = hex;
						break;
					}
					case 'u':
					{
						std::string code = std::string({ nextChar(), nextChar(), nextChar(), nextChar() });
						char hex = std::stoul(code, nullptr, 16);
						last = hex;
						break;
					}
				}
			}
			value += last;
		}
	} else if (last == '\'') {
		std::string value = "";
		while (true) {
			last = nextChar();
			if (last == '\'') {
				if (value.size() < 1)
					value = std::string(1, 0);
				ID_STRING = value;
				NUM_VALUE = CNumber::Long(static_cast<unsigned char>(value[0]));
				return TOK_NUM;
			} else if (last == '\\') {
				switch (last = nextChar()) {
					case '"':
						last = '"';
						break;
					case '\'':
						last = '\'';
						break;
					case 'n':
						last = '\n';
						break;
					case '?':
						last = '\?';
						break;
					case 'a':
						last = '\a';
						break;
					case 'b':
						last = '\b';
						break;
					case 'f':
						last = '\f';
						break;
					case 'r':
						last = '\r';
						break;
					case 't':
						last = '\t';
						break;
					case 'v':
						last = '\v';
						break;
					case '0':
						last = '\0';
						break;
					case 'x':
					{
						std::string code = std::string({ nextChar(), nextChar() });
						char hex = std::stoul(code, nullptr, 16);
						last = hex;
						break;
					}
					case 'u':
					{
						std::string code = std::string({ nextChar(), nextChar(), nextChar(), nextChar() });
						char hex = std::stoul(code, nullptr, 16);
						last = hex;
						break;
					}
				}
			}
			value += last;
		}
	}

	int ret = last;
	ID_STRING = last;
	//last = nextChar();
	return ret;
}

std::vector<Token> Lexer::lexString(const std::string &INPUT, const boost::filesystem::path &filename)
{
	std::vector<std::string> LINES;
	std::stringstream ss(INPUT);
	std::string item;

	while (std::getline(ss, item, '\n')) {
		LINES.push_back(item);
	}

	std::vector<Token> tokens;
	this->INPUT = INPUT;
	this->INPUT_INDEX = 0;
	this->LINE_INDEX = 0;
	this->TOKEN_DIST = 0;

	while (true) {
		int token = this->getToken();
		if (token == TOK_EOF)
			break;
		if (token == '#')
			continue;
		Token t = { filename, LINES[LINE_INDEX], LINE_INDEX, this->TOKEN_DIST, this->ID_STRING, this->NUM_VALUE, token };

		if (t.type == TOK_DEF) {
			std::vector<Token> temp;
			while (tokens.back().type != '(') {
				temp.push_back(tokens.back());
				tokens.pop_back();
			}
			temp.push_back(tokens.back());
			tokens.pop_back();
			if (!tokens.empty() && (tokens.back().type == TOK_IDF || tokens.back().type == '~' || tokens.back().type == TOK_SIZE || tokens.back().type == TOK_CHARN || tokens.back().type == TOK_CHARS || tokens.back().type == TOK_LENGTH || tokens.back().type == TOK_ALLOC || tokens.back().type == TOK_PARSE)) {
				temp.push_back(tokens.back());
				tokens.pop_back();
				if (tokens.back().type == TOK_DEF_TYPE) {
					temp.push_back(tokens.back());
					tokens.pop_back();
					temp.push_back(tokens.back());
					tokens.pop_back();
				}
				tokens.push_back(t);
			} else {
				tokens.push_back({ filename, LINES[LINE_INDEX], LINE_INDEX, this->TOKEN_DIST, this->ID_STRING, this->NUM_VALUE, TOK_LAMBDA });
			}
			while (!temp.empty()) {
				tokens.push_back(temp.back());
				temp.pop_back();
			}
		} else {
			tokens.push_back(t);
		}
	}

	for (auto &t : tokens) {
		while (!t.line.empty() && isspace(t.line[0])) {
			t.line = t.line.substr(1);
			t.distance--;
		}
	}

	return tokens;
}