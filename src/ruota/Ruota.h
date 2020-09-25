#ifndef RUOTA_H
#define RUOTA_H

#include "RuotaTypes.h"

class Ruota;

class UnaryI;
class CastingI;
class BinaryI;
class Container;
class DefineI;
class Sequence;
class IfElseI;
class WhileI;
class ForI;
class VariableI;
class DeclareI;
class IndexI;
class InnerI;
class AddI;
class SubI;
class MulI;
class DivI;
class ModI;
class PowI;
class LessI;
class MoreI;
class ELessI;
class EMoreI;
class Equals;
class NEquals;
class AndI;
class OrI;
class SetI;
class ReturnI;
class ExternI;
class LengthI;
class SizeI;
class ClassI;
class NewI;
class CastToI;
class AllocI;
class UntilI;
class ScopeI;
class MapI;
class ReferI;

class Ruota
{
private:
	static const std::map<std::string, signed int> bOperators;
	static const std::map<std::string, signed int> uOperators;
	Scope main;

public:
	static Lexer lexer;

	static hashcode_t HASH_THIS;
	static hashcode_t HASH_INIT;
	static hashcode_t HASH_KEY;
	static hashcode_t HASH_VALUE;

	Ruota();
	Symbol parseCode(const std::string &);
};

class UnaryI : public Instruction
{
protected:
	Instruction *a;

public:
	UnaryI(I_TYPE, Instruction *);
	Instruction *getA();
	virtual ~UnaryI();
};

class CastingI : public Instruction
{
protected:
	hashcode_t key;

public:
	CastingI(I_TYPE, hashcode_t);
	const hashcode_t getKey();
};

class BinaryI : public UnaryI
{
protected:
	Instruction *b;

public:
	BinaryI(I_TYPE, Instruction *, Instruction *);
	Instruction *getB();
	virtual ~BinaryI();
};

class Container : public Instruction
{
protected:
	Symbol d;

public:
	Container(Symbol d);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class DefineI : public Instruction
{
protected:
	hashcode_t key;
	D_TYPE ftype;
	std::vector<std::pair<LEX_TOKEN_TYPE, hashcode_t>> params;
	std::shared_ptr<Instruction> body;

public:
	DefineI(hashcode_t, D_TYPE ftype, std::vector<std::pair<LEX_TOKEN_TYPE, hashcode_t>>, std::shared_ptr<Instruction>);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class Sequence : public Instruction
{
protected:
	std::vector<Instruction *> children;

public:
	Sequence(std::vector<Instruction *>);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	virtual ~Sequence();
};

class IfElseI : public Instruction
{
protected:
	Instruction *ifs;
	Instruction *body;
	Instruction *elses;

public:
	IfElseI(Instruction *, Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	virtual ~IfElseI();
};

class WhileI : public Instruction
{
protected:
	Instruction *whiles;
	Instruction *body;

public:
	WhileI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	virtual ~WhileI();
};

class ForI : public Instruction
{
protected:
	hashcode_t id;
	Instruction *fors;
	Instruction *body;

public:
	ForI(hashcode_t, Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	virtual ~ForI();
};

class VariableI : public CastingI
{
public:
	VariableI(hashcode_t);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class DeclareI : public CastingI
{
public:
	DeclareI(hashcode_t);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class IndexI : public BinaryI
{
public:
	IndexI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class InnerI : public BinaryI
{
public:
	InnerI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class CallI : public BinaryI
{
public:
	CallI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class AddI : public BinaryI
{
public:
	AddI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class SubI : public BinaryI
{
public:
	SubI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class MulI : public BinaryI
{
public:
	MulI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class DivI : public BinaryI
{
public:
	DivI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class ModI : public BinaryI
{
public:
	ModI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class PowI : public BinaryI
{
public:
	PowI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class LessI : public BinaryI
{
public:
	LessI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class MoreI : public BinaryI
{
public:
	MoreI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class ELessI : public BinaryI
{
public:
	ELessI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class EMoreI : public BinaryI
{
public:
	EMoreI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class Equals : public BinaryI
{
public:
	Equals(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class NEquals : public BinaryI
{
public:
	NEquals(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class AndI : public BinaryI
{
public:
	AndI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class OrI : public BinaryI
{
public:
	OrI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class SetI : public BinaryI
{
public:
	SetI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class ReturnI : public UnaryI
{
public:
	ReturnI(Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class ExternI : public UnaryI
{
protected:
	std::string id;
	boost::function<Symbol(std::vector<Symbol>)> f;

public:
	ExternI(const std::string &, Instruction *a);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class LengthI : public UnaryI
{
public:
	LengthI(Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class SizeI : public UnaryI
{
public:
	SizeI(Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class ClassI : public Instruction
{
protected:
	hashcode_t key;
	OBJECT_TYPE type;
	std::shared_ptr<Instruction> body;
	Instruction * extends;

public:
	ClassI(hashcode_t, OBJECT_TYPE, std::shared_ptr<Instruction>, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	~ClassI();
};

class NewI : public BinaryI
{
public:
	NewI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class CastToI : public UnaryI
{
protected:
	D_TYPE convert;

public:
	CastToI(Instruction *, D_TYPE);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class AllocI : public UnaryI
{
public:
	AllocI(Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class UntilI : public BinaryI
{
public:
	UntilI(Instruction *, Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

class ScopeI : public Instruction
{
protected:
	std::vector<Instruction *> children;

public:
	ScopeI(std::vector<Instruction *>);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	virtual ~ScopeI();
};

class MapI : public Instruction
{
protected:
	std::map<hashcode_t, Instruction *> children;

public:
	MapI(std::map<hashcode_t, Instruction *>);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
	virtual ~MapI();
};

class ReferI : public UnaryI
{
public:
	ReferI(Instruction *);
	Symbol evaluate(Scope &) const override;
	const std::string toString(bool) const override;
};

namespace rdir
{
	extern std::vector<boost::filesystem::path> loaded;

	static inline boost::filesystem::path findFile(boost::filesystem::path currentDir, const std::string &filename)
	{
		auto currentDirCheck = currentDir / filename;
		if (boost::filesystem::exists(currentDirCheck))
			return currentDirCheck;
		auto libDirCheck = boost::dll::program_location().parent_path() / "lib" / filename;
		if (boost::filesystem::exists(libDirCheck))
			return libDirCheck;
		throw std::runtime_error("File `" + filename + "` cannot be found");
	}
} // namespace rdir

namespace rlib
{
	extern std::map<std::string, boost::function<Symbol(std::vector<Symbol>)>> loaded;

	inline void loadFunction(boost::filesystem::path currentDir, const std::string &rawlibname, const std::string &fname)
	{
		std::string libname = rawlibname;

#ifdef __unix__
		libname += ".so";
#else
		libname += ".dll";
#endif

		std::string search = rawlibname + "$" + fname;
		if (loaded.find(search) != loaded.end())
			return;

		loaded[search] = boost::dll::import<Symbol(std::vector<Symbol>)>(rdir::findFile(currentDir, libname), fname);
	}
} // namespace rlib

inline std::string colorASCII(TextColor color)
{
	return "\033[" + std::to_string(color) + "m";
}

#endif