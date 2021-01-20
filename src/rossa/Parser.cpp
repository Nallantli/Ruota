#include "../../bin/include/Rossa.h"

/*-------------------------------------------------------------------------------------------------------*/
/*class Instruction                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

Instruction::Instruction(const type_t &type, const token_t &token)
	: token{ token }
	, type{ type }
{}

Instruction::type_t Instruction::getType() const
{
	return type;
}

Instruction::~Instruction()
{}

/*-------------------------------------------------------------------------------------------------------*/
/*class UnaryI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

UnaryI::UnaryI(const type_t &type, const i_ptr_t &a, const token_t &token)
	: Instruction(type, token)
	, a{ a }
{}

const i_ptr_t UnaryI::getA() const
{
	return a;
}

/*-------------------------------------------------------------------------------------------------------*/
/*class CastingI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

CastingI::CastingI(const type_t &type, const hash_ull &key, const token_t &token)
	: Instruction(type, token)
	, key{ key }
{}

const hash_ull CastingI::getKey() const
{
	return key;
}

/*-------------------------------------------------------------------------------------------------------*/
/*class BinaryI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

BinaryI::BinaryI(const type_t &type, const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: UnaryI(type, a, token)
	, b{ b }
{}

const i_ptr_t BinaryI::getB() const
{
	return b;
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ContainerI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

ContainerI::ContainerI(const sym_t &d, const token_t &token)
	: Instruction(CONTAINER, token)
	, d{ d }
{}

const sym_t ContainerI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	return d;
}

const std::string ContainerI::compile() const
{
	return C_UNARY("ContainerI", d.toCodeString());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class DefineI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

DefineI::DefineI(const hash_ull &key, const sig_t &ftype, const std::vector<std::pair<LexerTokenType, hash_ull>> &params, const i_ptr_t &body, const std::vector<hash_ull> &captures, const token_t &token)
	: Instruction(DEFINE, token)
	, key{ key }
	, ftype{ ftype }
	, params{ params }
	, body{ body }
	, captures{ captures }
{}

const sym_t DefineI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	std::map<hash_ull, const sym_t> capturedVars;
	for (const hash_ull &e : captures) {
		capturedVars[e].set(&scope->getVariable(e, &token, stack_trace), &token, false, stack_trace);
	}
	func_ptr_t f = std::make_shared<const Function>(key, scope, params, body, capturedVars);
	if (key > 0) {
		return scope->createVariable(key, sym_t::FunctionSIG(ftype, f), &token);
	}
	return sym_t::FunctionSIG(ftype, f);
}

const std::string DefineI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const std::pair<LexerTokenType, hash_ull> &e : params) {
		if (i++ > 0)
			ca += ", ";
		ca += "{static_cast<LexerTokenType>(" + std::to_string(e.first) + "), " + std::to_string(e.second) + "}";
	}
	ca += "}";
	std::string cc = "{";
	i = 0;
	for (const hash_ull &e : captures) {
		if (i++ > 0)
			cc += ", ";
		cc += "static_cast<hash_ull>(" + std::to_string(e) + ")";
	}
	cc += "}";
	return C_QUINARY("DefineI", std::to_string(key), ftype.toCodeString(), ca, body->compile(), cc);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class VargDefineI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

VargDefineI::VargDefineI(const hash_ull &key, const i_ptr_t &body, const std::vector<hash_ull> &captures, const token_t &token)
	: Instruction(VARG_DEFINE, token)
	, key{ key }
	, body{ body }
	, captures{ captures }
{}

const sym_t VargDefineI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	std::map<hash_ull, const sym_t> capturedVars;
	for (const hash_ull &e : captures) {
		capturedVars[e].set(&scope->getVariable(e, &token, stack_trace), &token, false, stack_trace);
	}
	func_ptr_t f = std::make_shared<const Function>(key, scope, body, capturedVars);
	if (key > 0) {
		return scope->createVariable(key, sym_t::FunctionVARG(static_cast<func_ptr_t>(f)), &token);
	}
	return sym_t::FunctionVARG(static_cast<func_ptr_t>(f));
}

const std::string VargDefineI::compile() const
{
	std::string cc = "{";
	int i = 0;
	for (const hash_ull &e : captures) {
		if (i++ > 0)
			cc += ", ";
		cc += "static_cast<hash_ull>(" + std::to_string(e) + ")";
	}
	cc += "}";
	return C_TRINARY("VargDefineI", std::to_string(key), body->compile(), cc);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class SequenceI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

SequenceI::SequenceI(const i_vec_t &children, const token_t &token)
	: Instruction(SEQUENCE, token)
	, children{ children }
{}

const sym_t SequenceI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	sym_vec_t evals;
	for (const i_ptr_t &e : children) {
		if (e->getType() == UNTIL_I) {
			sym_vec_t v = e->evaluate(scope, stack_trace).getVector(&token, stack_trace);
			evals.insert(evals.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
		} else {
			evals.push_back(e->evaluate(scope, stack_trace));
		}
	}
	return sym_t::Array(evals);
}

const std::string SequenceI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const i_ptr_t &e : children) {
		if (i++ > 0)
			ca += ", ";
		ca += e->compile();
	}
	ca += "}";
	return C_UNARY("SequenceI", ca);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class IFElseI                                                                                          */
/*-------------------------------------------------------------------------------------------------------*/

IfElseI::IfElseI(const i_ptr_t &ifs, const i_ptr_t &body, const i_ptr_t &elses, const token_t &token)
	: Instruction(IFELSE, token)
	, ifs{ ifs }
	, body{ body }
	, elses{ elses }
{}

const sym_t IfElseI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	scope_ptr_t newScope = std::make_shared<Scope>(scope, 0);
	if (ifs->evaluate(newScope, stack_trace).getBool(&token, stack_trace)) {
		const sym_t r = body->evaluate(newScope, stack_trace);
		newScope->clear();
		return r;
	} else if (elses) {
		const sym_t r = elses->evaluate(newScope, stack_trace);
		newScope->clear();
		return r;
	}
	newScope->clear();
	return sym_t();
}

const std::string IfElseI::compile() const
{
	if (elses)
		return C_TRINARY("IfElseI", ifs->compile(), body->compile(), elses->compile());
	return C_TRINARY("IfElseI", ifs->compile(), body->compile(), "nullptr");
}

/*-------------------------------------------------------------------------------------------------------*/
/*class WhileI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

WhileI::WhileI(const i_ptr_t &whiles, const i_vec_t &body, const token_t &token)
	: Instruction(WHILE, token)
	, whiles{ whiles }
	, body{ body }
{}

const sym_t WhileI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	while (whiles->evaluate(scope, stack_trace).getBool(&token, stack_trace)) {
		scope_ptr_t newScope = std::make_shared<Scope>(scope, 0);
		for (const i_ptr_t &i : body) {
			const sym_t temp = i->evaluate(newScope, stack_trace);
			bool cflag = false;
			switch (temp.getSymbolType()) {
				case sym_t::type_t::ID_REFER:
				case sym_t::type_t::ID_RETURN:
					newScope->clear();
					return temp;
				case sym_t::type_t::ID_BREAK:
					newScope->clear();
					return sym_t();
				case sym_t::type_t::ID_CONTINUE:
					cflag = true;
					break;
				default:
					break;
			}
			if (cflag)
				continue;
		}
	}
	return sym_t();
}

const std::string WhileI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const i_ptr_t &e : body) {
		if (i++ > 0)
			ca += ", ";
		ca += e->compile();
	}
	ca += "}";
	return C_BINARY("WhileI", whiles->compile(), ca);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ForI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

ForI::ForI(const hash_ull &id, const i_ptr_t &fors, const i_vec_t &body, const token_t &token)
	: Instruction(FOR, token)
	, id{ id }
	, fors{ fors }
	, body{ body }
{}

const sym_t ForI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_vec_t evalFor = fors->evaluate(scope, stack_trace).getVector(&token, stack_trace);
	for (const sym_t &e : evalFor) {
		scope_ptr_t newScope = std::make_shared<Scope>(scope, 0);
		newScope->createVariable(id, e, &token);
		bool cflag = false;
		for (const i_ptr_t &i : body) {
			const sym_t temp = i->evaluate(newScope, stack_trace);
			switch (temp.getSymbolType()) {
				case sym_t::type_t::ID_REFER:
				case sym_t::type_t::ID_RETURN:
					newScope->clear();
					return temp;
				case sym_t::type_t::ID_BREAK:
					newScope->clear();
					return sym_t();
				case sym_t::type_t::ID_CONTINUE:
					cflag = true;
					break;
				default:
					break;
			}
			if (cflag)
				continue;
		}
		newScope->clear();
	}
	return sym_t();
}

const std::string ForI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const i_ptr_t &e : body) {
		if (i++ > 0)
			ca += ", ";
		ca += e->compile();
	}
	ca += "}";
	return C_TRINARY("ForI", std::to_string(id), fors->compile(), ca);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class VariableI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

VariableI::VariableI(const hash_ull &key, const token_t &token)
	: CastingI(VARIABLE, key, token)
{}

const sym_t VariableI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	return scope->getVariable(key, &token, stack_trace);
}

const std::string VariableI::compile() const
{
	return C_UNARY("VariableI", std::to_string(key));
}

/*-------------------------------------------------------------------------------------------------------*/
/*class GetThisI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

GetThisI::GetThisI(const token_t &token)
	: CastingI(GET_THIS_I, key, token)
{}

const sym_t GetThisI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	return scope->getThis(&token, stack_trace);
}

const std::string GetThisI::compile() const
{
	return C_NONE("GetThisI");
}

/*-------------------------------------------------------------------------------------------------------*/
/*class DeclareI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

DeclareI::DeclareI(const hash_ull &key, const type_sll &vtype, const i_ptr_t &a, const bool &isConst, const token_t &token)
	: CastingI(DECLARE, key, token)
	, vtype{ vtype }
	, a{ a }
	, isConst{ isConst }
{}

const sym_t DeclareI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t v = scope->createVariable(key, &token);
	const sym_t evalA = a->evaluate(scope, stack_trace);
	v.set(&evalA, &token, isConst, stack_trace);
	return v;
}

const std::string DeclareI::compile() const
{
	return C_QUATERNARY("DeclareI", std::to_string(key), std::to_string(vtype), a->compile(), isConst ? "true" : "false");
}

/*-------------------------------------------------------------------------------------------------------*/
/*class IndexI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

IndexI::IndexI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(INDEX, a, b, token)
{}

const sym_t IndexI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::index(scope, evalA, evalB, &token, stack_trace);
}

const std::string IndexI::compile() const
{
	return C_BINARY("IndexI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class InnerI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

InnerI::InnerI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(INNER, a, b, token)
{}

const sym_t InnerI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	switch (evalA.getValueType()) {
		case Value::type_t::DICTIONARY:
			if (b->getType() == VARIABLE)
				return evalA.indexDict(ROSSA_DEHASH(reinterpret_cast<const VariableI *>(b.get())->getKey()));
			throw RTError(_CANNOT_ENTER_DICTIONARY_, token, stack_trace);
		case Value::type_t::OBJECT:
		{
			const scope_ptr_t o = evalA.getObject(&token, stack_trace);
			if (o->getType() != Scope::type_t::STATIC_O && o->getType() != Scope::type_t::INSTANCE_O)
				throw RTError(_CANNOT_INDEX_OBJECT_, token, stack_trace);
			return b->evaluate(o, stack_trace);
		}
		default:
			throw RTError(_CANNOT_INDEX_VALUE_, token, stack_trace);
	}
}

const std::string InnerI::compile() const
{
	return C_BINARY("InnerI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class CallI                                                                                            */
/*-------------------------------------------------------------------------------------------------------*/

CallI::CallI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(INDEX, a, b, token)
{}

const sym_t CallI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	return ops::call(scope, a, b->evaluate(scope, stack_trace).getVector(&token, stack_trace), &token, stack_trace);
}

const std::string CallI::compile() const
{
	return C_BINARY("CallI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class AddI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

AddI::AddI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(ADD, a, b, token)
{}

const sym_t AddI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::add(scope, evalA, evalB, &token, stack_trace);
}

const std::string AddI::compile() const
{
	return C_BINARY("AddI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class SubI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

SubI::SubI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(SUB, a, b, token)
{}

const sym_t SubI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::sub(scope, evalA, evalB, &token, stack_trace);
}

const std::string SubI::compile() const
{
	return C_BINARY("SubI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class MulI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

MulI::MulI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(MUL, a, b, token)
{}

const sym_t MulI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::mul(scope, evalA, evalB, &token, stack_trace);
}

const std::string MulI::compile() const
{
	return C_BINARY("MulI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class DivI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

DivI::DivI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(DIV, a, b, token)
{}

const sym_t DivI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::div(scope, evalA, evalB, &token, stack_trace);
}

const std::string DivI::compile() const
{
	return C_BINARY("DivI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ModI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

ModI::ModI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(MOD, a, b, token)
{}

const sym_t ModI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::mod(scope, evalA, evalB, &token, stack_trace);
}

const std::string ModI::compile() const
{
	return C_BINARY("ModI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class PowI                                                                                             */
/*-------------------------------------------------------------------------------------------------------*/

PowI::PowI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(POW_I, a, b, token)
{}

const sym_t PowI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::pow(scope, evalA, evalB, &token, stack_trace);
}

const std::string PowI::compile() const
{
	return C_BINARY("PowI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class LessI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

LessI::LessI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(LESS, a, b, token)
{}

const sym_t LessI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::less(scope, evalA, evalB, &token, stack_trace);
}

const std::string LessI::compile() const
{
	return C_BINARY("LessI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class MoreI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

MoreI::MoreI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(MORE, a, b, token)
{}

const sym_t MoreI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::more(scope, evalA, evalB, &token, stack_trace);
}

const std::string MoreI::compile() const
{
	return C_BINARY("MoreI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ELessI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

ELessI::ELessI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(ELESS, a, b, token)
{}

const sym_t ELessI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::eless(scope, evalA, evalB, &token, stack_trace);
}

const std::string ELessI::compile() const
{
	return C_BINARY("ELessI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class EMoreI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

EMoreI::EMoreI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(EMORE, a, b, token)
{}

const sym_t EMoreI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::emore(scope, evalA, evalB, &token, stack_trace);
}

const std::string EMoreI::compile() const
{
	return C_BINARY("EMoreI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class EqualsI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

EqualsI::EqualsI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(EQUALS, a, b, token)
{}

const sym_t EqualsI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	if (evalA.getValueType() == Value::type_t::OBJECT && !evalA.getObject(&token, stack_trace)->hasValue(Rossa::HASH_EQUALS))
		return scope->getVariable(Rossa::HASH_EQUALS, &token, stack_trace).call({ evalA, evalB }, &token, stack_trace);

	return sym_t::Boolean(evalA.equals(&evalB, &token, stack_trace));
}

const std::string EqualsI::compile() const
{
	return C_BINARY("EqualsI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class NEqualsI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

NEqualsI::NEqualsI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(NEQUALS, a, b, token)
{}

const sym_t NEqualsI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	if (evalA.getValueType() == Value::type_t::OBJECT && !evalA.getObject(&token, stack_trace)->hasValue(Rossa::HASH_NEQUALS))
		return scope->getVariable(Rossa::HASH_NEQUALS, &token, stack_trace).call({ evalA, evalB }, &token, stack_trace);

	return sym_t::Boolean(evalA.nequals(&evalB, &token, stack_trace));
}

const std::string NEqualsI::compile() const
{
	return C_BINARY("NEqualsI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class AndI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

AndI::AndI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(AND, a, b, token)
{}

const sym_t AndI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	if (!a->evaluate(scope, stack_trace).getBool(&token, stack_trace))
		return sym_t::Boolean(false);
	if (b->evaluate(scope, stack_trace).getBool(&token, stack_trace))
		return sym_t::Boolean(true);
	return sym_t::Boolean(false);
}

const std::string AndI::compile() const
{
	return C_BINARY("AndI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class OrI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

OrI::OrI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(OR, a, b, token)
{}

const sym_t OrI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	if (a->evaluate(scope, stack_trace).getBool(&token, stack_trace))
		return sym_t::Boolean(true);
	if (b->evaluate(scope, stack_trace).getBool(&token, stack_trace))
		return sym_t::Boolean(true);
	return sym_t::Boolean(false);
}

const std::string OrI::compile() const
{
	return C_BINARY("OrI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class BOrI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

BOrI::BOrI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(B_OR, a, b, token)
{}

const sym_t BOrI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::bor(scope, evalA, evalB, &token, stack_trace);
}

const std::string BOrI::compile() const
{
	return C_BINARY("BOrI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class BXOrI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

BXOrI::BXOrI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(B_XOR, a, b, token)
{}

const sym_t BXOrI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::bxor(scope, evalA, evalB, &token, stack_trace);
}

const std::string BXOrI::compile() const
{
	return C_BINARY("BXOrI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class BAndI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

BAndI::BAndI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(B_AND, a, b, token)
{}

const sym_t BAndI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::band(scope, evalA, evalB, &token, stack_trace);
}

const std::string BAndI::compile() const
{
	return C_BINARY("BAndI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class BShiftLeftI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

BShiftLeftI::BShiftLeftI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(B_SH_L, a, b, token)
{}

const sym_t BShiftLeftI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::bshl(scope, evalA, evalB, &token, stack_trace);
}

const std::string BShiftLeftI::compile() const
{
	return C_BINARY("BShiftLeftI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class BShiftRightI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

BShiftRightI::BShiftRightI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(B_SH_R, a, b, token)
{}

const sym_t BShiftRightI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	return ops::bshr(scope, evalA, evalB, &token, stack_trace);
}

const std::string BShiftRightI::compile() const
{
	return C_BINARY("BShiftRightI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class SetI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

SetI::SetI(const i_ptr_t &a, const i_ptr_t &b, const bool &isConst, const token_t &token)
	: BinaryI(SET, a, b, token)
	, isConst{ isConst }
{}

const sym_t SetI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	if (evalA.getValueType() == Value::type_t::OBJECT && !evalA.getObject(&token, stack_trace)->hasValue(Rossa::HASH_SET)) {
		try {
			return scope->getVariable(Rossa::HASH_SET, &token, stack_trace).call({ evalA, evalB }, &token, stack_trace);
		} catch (const RTError &e) {
			evalA.set(&evalB, &token, isConst, stack_trace);
		}
	} else {
		evalA.set(&evalB, &token, isConst, stack_trace);
	}
	return evalA;
}

const std::string SetI::compile() const
{
	return C_TRINARY("SetI", a->compile(), b->compile(), isConst ? "true" : "false");
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ReturnI                                                                                          */
/*-------------------------------------------------------------------------------------------------------*/

ReturnI::ReturnI(const i_ptr_t &a, const token_t &token)
	: UnaryI(RETURN, a, token)
{}

const sym_t ReturnI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	sym_t evalA = a->evaluate(scope, stack_trace);
	evalA.setSymbolType(sym_t::type_t::ID_RETURN);
	return evalA;
}

const std::string ReturnI::compile() const
{
	return C_UNARY("ReturnI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ExternI                                                                                          */
/*-------------------------------------------------------------------------------------------------------*/

ExternI::ExternI(const std::string &libname, const std::string &fname, const i_ptr_t &a, const token_t &token)
	: UnaryI(EXTERN, a, token)
	, libname{ libname }
	, fname{ fname }
{
	this->f = lib::loadFunction(libname, fname, &token);
}

const sym_t ExternI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	return f(a->evaluate(scope, stack_trace).getVector(&token, stack_trace), &token, Rossa::MAIN_HASH, stack_trace);
}

const std::string ExternI::compile() const
{
	return C_TRINARY("ExternI", ("\"" + libname + "\""), ("\"" + fname + "\""), a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class LengthI                                                                                          */
/*-------------------------------------------------------------------------------------------------------*/

LengthI::LengthI(const i_ptr_t &a, const token_t &token)
	: UnaryI(LENGTH, a, token)
{}

const sym_t LengthI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	switch (evalA.getValueType()) {
		case Value::type_t::STRING:
		{
			std::string str = evalA.getString(&token, stack_trace);
			int c, i, ix, q;
			for (q = 0, i = 0, ix = str.size(); i < ix; i++, q++) {
				c = static_cast<unsigned char>(str[i]);
				if (c >= 0 && c <= 127)
					i += 0;
				else if ((c & 0xE0) == 0xC0)
					i += 1;
				else if ((c & 0xF0) == 0xE0)
					i += 2;
				else if ((c & 0xF8) == 0xF0)
					i += 3;
				else
					return sym_t::Number(number_t::Long(evalA.getString(&token, stack_trace).size()));
			}
			return sym_t::Number(number_t::Long(q));
		}
		case Value::type_t::DICTIONARY:
			return sym_t::Number(number_t::Long(evalA.dictionarySize(&token, stack_trace)));
		case Value::type_t::ARRAY:
			return sym_t::Number(number_t::Long(evalA.vectorSize()));
		case Value::type_t::OBJECT:
		{
			const scope_ptr_t o = evalA.getObject(&token, stack_trace);
			if (o->hasValue(Rossa::HASH_LENGTH))
				return o->getVariable(Rossa::HASH_LENGTH, &token, stack_trace).call({ }, &token, stack_trace);
		}
		default:
			throw RTError(_FAILURE_LENGTH_, token, stack_trace);
	}
}

const std::string LengthI::compile() const
{
	return C_UNARY("LengthI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ClassI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

ClassI::ClassI(const hash_ull &key, const Scope::type_t &type, const i_ptr_t &body, const i_ptr_t &extends, const token_t &token)
	: Instruction(CLASS_I, token)
	, key{ key }
	, type{ type }
	, body{ body }
	, extends{ extends }
{}

const sym_t ClassI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	i_ptr_t nbody = body;
	scope_ptr_t o;
	scope_ptr_t ex = nullptr;
	std::vector<type_sll> extensions;
	if (extends) {
		const sym_t e = extends->evaluate(scope, stack_trace);
		if (e.getValueType() == Value::type_t::TYPE_NAME)
			extensions.push_back(e.getTypeName(&token, stack_trace));
		else {
			ex = e.getObject(&token, stack_trace);
			if (ex->getType() == Scope::type_t::STATIC_O)
				throw RTError(_FAILURE_EXTEND_, token, stack_trace);
			const i_ptr_t eb = ex->getBody();
			i_vec_t temp;
			temp.push_back(body);
			temp.push_back(eb);
			nbody = std::make_shared<ScopeI>(temp, token);
		}
	}
	o = std::make_shared<Scope>(scope, type, nbody, key, ex, extensions);
	if (type == Scope::type_t::STATIC_O)
		body->evaluate(o, stack_trace);
	return scope->createVariable(key, sym_t::Object(o), &token);
}

const std::string ClassI::compile() const
{
	if (extends)
		return C_QUATERNARY("ClassI", std::to_string(key), ("static_cast<ObjectType>(" + std::to_string(type) + ")"), body->compile(), extends->compile());
	return C_QUATERNARY("ClassI", std::to_string(key), ("static_cast<ObjectType>(" + std::to_string(type) + ")"), body->compile(), "nullptr");
}

/*-------------------------------------------------------------------------------------------------------*/
/*class NewI                                                                                             */
/*-------------------------------------------------------------------------------------------------------*/

NewI::NewI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(NEW_I, a, b, token)
{}

const sym_t NewI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t &evalA = a->evaluate(scope, stack_trace);
	const sym_vec_t evalB = b->evaluate(scope, stack_trace).getVector(&token, stack_trace);

	const scope_ptr_t &base = evalA.getObject(&token, stack_trace);
	return base->instantiate(evalB, &token, stack_trace);
}

const std::string NewI::compile() const
{
	return C_BINARY("NewI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class CastToI                                                                                          */
/*-------------------------------------------------------------------------------------------------------*/

CastToI::CastToI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(CAST_TO_I, a, b, token)
{}

const sym_t CastToI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const type_sll convert = b->evaluate(scope, stack_trace).getTypeName(&token, stack_trace);

	switch (evalA.getValueType()) {
		case Value::type_t::NUMBER:
			switch (convert) {
				case Value::type_t::NUMBER:
					return evalA;
				case Value::type_t::STRING:
					return sym_t::String(evalA.toString(&token, stack_trace));
				case Value::type_t::BOOLEAN_D:
					return sym_t::Boolean(evalA.getNumber(&token, stack_trace).getLong() != 0);
				default:
					break;
			}
			break;
		case Value::type_t::STRING:
			switch (convert) {
				case Value::type_t::NUMBER:
					try {
						const std::string s = evalA.getString(&token, stack_trace);
						if (s.length() > 2 && s[0] == '0' && isalpha(s[1])) {
							switch (s[1]) {
								case 'b':
								case 'B':
									return sym_t::Number(number_t::Long(std::stoll(s.substr(2), nullptr, 2)));
								default:
									return sym_t::Number(number_t::Long(std::stoll(s, nullptr, 0)));
							}
						}
						return sym_t::Number(number_t::Double(std::stold(s)));
					} catch (const std::invalid_argument &e) {
						throw RTError(format::format(_FAILURE_STR_TO_NUM_, { evalA.getString(&token, stack_trace) }), token, stack_trace);
					}
				case Value::type_t::STRING:
					return evalA;
				case Value::type_t::BOOLEAN_D:
					return sym_t::Boolean(evalA.getString(&token, stack_trace) == KEYWORD_TRUE);
				case Value::type_t::ARRAY:
				{
					std::string str = evalA.getString(&token, stack_trace);
					sym_vec_t nv;
					int last = 0;
					int c, i, ix, q, s;
					for (q = 0, i = 0, ix = str.size(); i < ix; i++, q++) {
						c = static_cast<unsigned char>(str[i]);
						if (c >= 0 && c <= 127) {
							i += 0;
							s = 1;
						} else if ((c & 0xE0) == 0xC0) {
							i += 1;
							s = 2;
						} else if ((c & 0xF0) == 0xE0) {
							i += 2;
							s = 3;
						} else if ((c & 0xF8) == 0xF0) {
							i += 3;
							s = 4;
						} else {
							nv.clear();
							for (size_t i = 0; i < str.size(); i++)
								nv.push_back(sym_t::String(std::string(1, str[i])));
							return sym_t::Array(nv);
						}
						nv.push_back(sym_t::String(str.substr(last, s)));
						last = i + 1;
					}
					return sym_t::Array(nv);
				}
				case Value::type_t::TYPE_NAME:
				{
					const std::string s = evalA.getString(&token, stack_trace);
					if (s == KEYWORD_NUMBER)
						return sym_t::TypeName(Value::type_t::NUMBER);
					if (s == KEYWORD_STRING)
						return sym_t::TypeName(Value::type_t::STRING);
					if (s == KEYWORD_BOOLEAN)
						return sym_t::TypeName(Value::type_t::BOOLEAN_D);
					if (s == KEYWORD_ARRAY)
						return sym_t::TypeName(Value::type_t::ARRAY);
					if (s == KEYWORD_DICTIONARY)
						return sym_t::TypeName(Value::type_t::DICTIONARY);
					if (s == KEYWORD_FUNCTION)
						return sym_t::TypeName(Value::type_t::FUNCTION);
					if (s == KEYWORD_OBJECT)
						return sym_t::TypeName(Value::type_t::OBJECT);
					if (s == KEYWORD_TYPE)
						return sym_t::TypeName(Value::type_t::TYPE_NAME);
					if (s == KEYWORD_NIL_NAME)
						return sym_t::TypeName(Value::type_t::NIL);
					if (s == KEYWORD_POINTER)
						return sym_t::TypeName(Value::type_t::POINTER);
					return sym_t::TypeName(ROSSA_HASH(evalA.getString(&token, stack_trace)));
				}
				default:
					break;
			}
			break;
		case Value::type_t::BOOLEAN_D:
			switch (convert) {
				case Value::type_t::NUMBER:
					return sym_t::Number(number_t::Long(evalA.getBool(&token, stack_trace) ? 1 : 0));
				case Value::type_t::STRING:
					return sym_t::String(evalA.getBool(&token, stack_trace) ? "true" : "false");
				case Value::type_t::BOOLEAN_D:
					return evalA;
				default:
					break;
			}
			break;
		case Value::type_t::ARRAY:
			switch (convert) {
				case Value::type_t::STRING:
					return sym_t::String(evalA.toString(&token, stack_trace));
				case Value::type_t::ARRAY:
					return evalA;
				case Value::type_t::DICTIONARY:
				{
					const sym_vec_t v = evalA.getVector(&token, stack_trace);
					sym_map_t nd;
					for (size_t i = 0; i < v.size(); i++)
						nd[std::to_string(i)] = v[i];
					return sym_t::Dictionary(nd);
				}
				default:
					break;
			}
			break;
		case Value::type_t::DICTIONARY:
			switch (convert) {
				case Value::type_t::STRING:
					return sym_t::String(evalA.toString(&token, stack_trace));
				case Value::type_t::ARRAY:
				{
					const sym_map_t dict = evalA.getDictionary(&token, stack_trace);
					sym_vec_t nv;
					for (const std::pair<const std::string, sym_t> &e : dict) {
						sym_vec_t l = { sym_t::String(e.first), e.second };
						nv.push_back(sym_t::Array(l));
					}
					return sym_t::Array(nv);
				}
				case Value::type_t::DICTIONARY:
					return evalA;
				default:
					break;
			}
			break;
		case Value::type_t::OBJECT:
		{
			if (convert == evalA.getAugValueType())
				return evalA;
			const hash_ull fname = ROSSA_HASH("->" + getTypeString(convert));
			const scope_ptr_t o = evalA.getObject(&token, stack_trace);
			if (o->hasValue(fname))
				return o->getVariable(fname, &token, stack_trace).call({ }, &token, stack_trace);
			break;
		}
		case Value::type_t::TYPE_NAME:
			switch (convert) {
				case Value::type_t::STRING:
					return sym_t::String(getTypeString(evalA.getAugValueType()));
				case Value::type_t::TYPE_NAME:
					return evalA;
				default:
					break;
			}
			break;
		default:
			break;
	}

	const hash_ull fname = ROSSA_HASH("->" + getTypeString(convert));
	return scope->getVariable(fname, &token, stack_trace).call({ evalA }, &token, stack_trace);
}

const std::string CastToI::compile() const
{
	return C_BINARY("CastToI", a->compile(), b->compile());
}


/*-------------------------------------------------------------------------------------------------------*/
/*class AllocI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

AllocI::AllocI(const i_ptr_t &a, const token_t &token)
	: UnaryI(ALLOC_I, a, token)
{}

const sym_t AllocI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const long_int_t evalA = a->evaluate(scope, stack_trace).getNumber(&token, stack_trace).getLong();
	if (evalA < 0)
		throw RTError(_FAILURE_ALLOC_, token, stack_trace);
	return sym_t::allocate(evalA);
}

const std::string AllocI::compile() const
{
	return C_UNARY("AllocI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class UntilI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

UntilI::UntilI(const i_ptr_t &a, const i_ptr_t &b, const i_ptr_t &step, const bool &inclusive, const token_t &token)
	: BinaryI(UNTIL_I, a, b, token)
	, step(step)
	, inclusive{ inclusive }
{}

const sym_t UntilI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);

	if (step == nullptr)
		return ops::untilnostep(scope, inclusive, evalA, evalB, &token, stack_trace);
	else
		return ops::untilstep(scope, inclusive, evalA, evalB, step->evaluate(scope, stack_trace), &token, stack_trace);
}

const std::string UntilI::compile() const
{
	if (step)
		return C_QUATERNARY("UntilI", a->compile(), b->compile(), step->compile(), inclusive ? "true" : "false");
	return C_QUATERNARY("UntilI", a->compile(), b->compile(), "nullptr", inclusive ? "true" : "false");
}


/*-------------------------------------------------------------------------------------------------------*/
/*class ScopeI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

ScopeI::ScopeI(const i_vec_t &children, const token_t &token)
	: Instruction(SCOPE_I, token)
	, children{ children }
{}

const sym_t ScopeI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	for (const i_ptr_t &e : children) {
		const sym_t eval = e->evaluate(scope, stack_trace);
		if (eval.getSymbolType() != sym_t::type_t::ID_CASUAL)
			return eval;
	}
	return sym_t();
}

const std::string ScopeI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const i_ptr_t &e : children) {
		if (i++ > 0)
			ca += ", ";
		ca += e->compile();
	}
	ca += "}";
	return C_UNARY("ScopeI", ca);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class MapI                                                                                             */
/*-------------------------------------------------------------------------------------------------------*/

MapI::MapI(const std::map<std::string, i_ptr_t> &children, const token_t &token)
	: Instruction(MAP_I, token)
	, children{ children }
{}

const sym_t MapI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	sym_map_t evals;
	for (const std::pair<std::string, i_ptr_t> &e : children) {
		const sym_t eval = e.second->evaluate(scope, stack_trace);
		if (eval.getValueType() == Value::type_t::NIL)
			continue;
		evals[e.first] = eval;
	}
	return sym_t::Dictionary(evals);
}

const std::string MapI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const std::pair<std::string, i_ptr_t> &e : children) {
		if (i++ > 0)
			ca += ", ";
		ca += "{\"" + e.first + "\", " + e.second->compile() + "}";
	}
	ca += "}";
	return C_UNARY("MapI", ca);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ReferI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

ReferI::ReferI(const i_ptr_t &a, const token_t &token)
	: UnaryI(REFER_I, a, token)
{}

const sym_t ReferI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	sym_t evalA = a->evaluate(scope, stack_trace);
	evalA.setSymbolType(sym_t::type_t::ID_REFER);
	return evalA;
}

const std::string ReferI::compile() const
{
	return C_UNARY("ReferI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class SwitchI                                                                                          */
/*-------------------------------------------------------------------------------------------------------*/

SwitchI::SwitchI(const i_ptr_t &switchs, const std::map<sym_t, size_t> &cases_solved, const std::map<i_ptr_t, size_t> &cases_unsolved, const i_vec_t &cases, const i_ptr_t &elses, const token_t &token)
	: Instruction(SWITCH_I, token)
	, switchs{ switchs }
	, cases_solved{ cases_solved }
	, cases_unsolved{ cases_unsolved }
	, cases{ cases }
	, elses{ elses }
{}

const sym_t SwitchI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	scope_ptr_t newScope = std::make_shared<Scope>(scope, 0);
	const sym_t eval = switchs->evaluate(newScope, stack_trace);
	size_t index = 0;
	if (cases_solved.find(eval) != cases_solved.end()) {
		index = cases_solved.at(eval);
	} else if (!cases_unsolved.empty()) {
		for (const std::pair<const i_ptr_t, const size_t> &e : cases_unsolved) {
			const sym_t evalE = e.first->evaluate(newScope, stack_trace);
			if (evalE.equals(&eval, &token, stack_trace)) {
				index = e.second;
			}
		}
	}

	if (index > 0) {
		const sym_t r = cases[index - 1]->evaluate(newScope, stack_trace);
		newScope->clear();
		return r;
	} else if (elses) {
		const sym_t r = elses->evaluate(newScope, stack_trace);
		newScope->clear();
		return r;
	}

	newScope->clear();
	return sym_t();
}

const std::string SwitchI::compile() const
{
	std::string cs = "{";
	size_t i = 0;
	for (const std::pair<const sym_t, size_t> &e : cases_solved) {
		if (i++ > 0)
			cs += ", ";
		cs += "{\"" + e.first.toCodeString() + "\", " + std::to_string(e.second) + "}";
	}
	cs += "}";

	std::string cu = "{";
	i = 0;
	for (const std::pair<const i_ptr_t, const size_t> &e : cases_unsolved) {
		if (i++ > 0)
			cu += ", ";
		cu += "{\"" + e.first->compile() + "\", " + std::to_string(e.second) + "}";
	}
	cu += "}";

	std::string ca = "{";
	i = 0;
	for (const i_ptr_t &e : cases) {
		if (i++ > 0)
			ca += ", ";
		ca += e->compile();
	}
	ca += "}";

	if (elses)
		return C_QUINARY("SwitchI", switchs->compile(), cs, cu, ca, elses->compile());
	return C_QUINARY("SwitchI", switchs->compile(), cs, cu, ca, "nullptr");
}

/*-------------------------------------------------------------------------------------------------------*/
/*class TryCatchI                                                                                        */
/*-------------------------------------------------------------------------------------------------------*/

TryCatchI::TryCatchI(const i_ptr_t &a, const i_ptr_t &b, const hash_ull &key, const token_t &token)
	: BinaryI(TRY_CATCH_I, a, b, token)
	, key{ key }
{}

const sym_t TryCatchI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	try {
		scope_ptr_t newScope = std::make_shared<Scope>(scope, 0);
		const sym_t r = a->evaluate(newScope, stack_trace);
		newScope->clear();
		return r;
	} catch (const RTError &e) {
		scope_ptr_t newScope = std::make_shared<Scope>(scope, 0);
		newScope->createVariable(key, sym_t::String(std::string(e.what())), &token);
		const sym_t r = b->evaluate(newScope, stack_trace);
		newScope->clear();
		return r;
	}
}

const std::string TryCatchI::compile() const
{
	return C_TRINARY("TryCatchI", a->compile(), b->compile(), std::to_string(key));
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ThrowI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

ThrowI::ThrowI(const i_ptr_t &a, const token_t &token)
	: UnaryI(THROW_I, a, token)
{}

const sym_t ThrowI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	throw RTError(evalA.getString(&token, stack_trace), token, stack_trace);
	return sym_t();
}

const std::string ThrowI::compile() const
{
	return C_UNARY("ThrowI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class PureEqualsI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

PureEqualsI::PureEqualsI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(PURE_EQUALS, a, b, token)
{}

const sym_t PureEqualsI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);
	return sym_t::Boolean(evalA.pureEquals(&evalB, &token, stack_trace));
}

const std::string PureEqualsI::compile() const
{
	return C_BINARY("PureEqualsI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class PureNEqualsI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

PureNEqualsI::PureNEqualsI(const i_ptr_t &a, const i_ptr_t &b, const token_t &token)
	: BinaryI(PURE_NEQUALS, a, b, token)
{}

const sym_t PureNEqualsI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	const sym_t evalB = b->evaluate(scope, stack_trace);
	return sym_t::Boolean(evalA.pureNEquals(&evalB, &token, stack_trace));
}

const std::string PureNEqualsI::compile() const
{
	return C_BINARY("PureNEqualsI", a->compile(), b->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class CharNI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

CharNI::CharNI(const i_ptr_t &a, const token_t &token)
	: UnaryI(CHARN_I, a, token)
{}

const sym_t CharNI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const std::string evalA = a->evaluate(scope, stack_trace).getString(&token, stack_trace);
	sym_vec_t nv;
	for (const unsigned char &c : evalA)
		nv.push_back(sym_t::Number(number_t::Long(c)));
	return sym_t::Array(nv);
}

const std::string CharNI::compile() const
{
	return C_UNARY("CharNI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class CharSI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

CharSI::CharSI(const i_ptr_t &a, const token_t &token)
	: UnaryI(CHARS_I, a, token)
{}

const sym_t CharSI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	switch (evalA.getValueType()) {
		case Value::type_t::NUMBER:
			return sym_t::String(std::string(1, static_cast<char>(evalA.getNumber(&token, stack_trace).getLong())));
		case Value::type_t::ARRAY:
		{
			std::string ret = "";
			const sym_vec_t v = evalA.getVector(&token, stack_trace);
			for (const sym_t &e : v)
				ret.push_back(static_cast<char>(e.getNumber(&token, stack_trace).getLong()));
			return sym_t::String(ret);
		}
		default:
			throw RTError(_FAILURE_TO_STR_, token, stack_trace);
	}
}

const std::string CharSI::compile() const
{
	return C_UNARY("CharSI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class DeclareVarsI                                                                                      */
/*-------------------------------------------------------------------------------------------------------*/

DeclareVarsI::DeclareVarsI(const std::vector<hash_ull> &keys, const token_t &token)
	: Instruction(DECLARE_VARS_I, token)
	, keys{ keys }
{}

const sym_t DeclareVarsI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	sym_vec_t newvs;
	for (const hash_ull &k : keys)
		newvs.push_back(scope->createVariable(k, &token));
	return sym_t::Array(newvs);
}

const std::string DeclareVarsI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const hash_ull &e : keys) {
		if (i++ > 0)
			ca += ", ";
		ca += std::to_string(e);
	}
	ca += "}";
	return C_UNARY("DeclareVarsI", ca);
}

/*-------------------------------------------------------------------------------------------------------*/
/*class ParseI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

ParseI::ParseI(const i_ptr_t &a, const token_t &token)
	: UnaryI(ALLOC_I, a, token)
{}

const sym_t ParseI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const std::string evalA = a->evaluate(scope, stack_trace).getString(&token, stack_trace);

	const std::vector<token_t> tokens = Rossa::lexString(evalA, std::filesystem::current_path() / KEYWORD_NIL);
	NodeParser np(tokens, std::filesystem::current_path() / KEYWORD_NIL);
	return np.parse()->fold()->genParser()->evaluate(scope, stack_trace);
}

const std::string ParseI::compile() const
{
	return C_UNARY("ParseI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class TypeI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

TypeI::TypeI(const i_ptr_t &a, const token_t &token)
	: UnaryI(TYPE_I, a, token)
{}

const sym_t TypeI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	const sym_t evalA = a->evaluate(scope, stack_trace);
	return sym_t::TypeName(evalA.getAugValueType());
}

const std::string TypeI::compile() const
{
	return C_UNARY("TypeI", a->compile());
}

/*-------------------------------------------------------------------------------------------------------*/
/*class CallOpI                                                                                           */
/*-------------------------------------------------------------------------------------------------------*/

CallOpI::CallOpI(const size_t &id, const i_vec_t &children, const token_t &token)
	: Instruction(CALL_OP_I, token)
	, id{ id }
	, children{ children }
{}

const sym_t CallOpI::evaluate(const scope_ptr_t &scope, trace_t &stack_trace) const
{
	switch (id) {
		case 0:
			return ops::index(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 1:
			return ops::untilnostep(NULL,
				false,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 2:
			return ops::untilstep(NULL,
				false,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				children[2]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 3:
			return ops::untilnostep(NULL,
				true,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 4:
			return ops::untilstep(NULL,
				true,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				children[2]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 5:
			return ops::add(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 6:
			return ops::sub(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 7:
			return ops::mul(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 8:
			return ops::div(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 9:
			return ops::mod(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 10:
			return ops::pow(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 11:
			return ops::less(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 12:
			return ops::more(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 13:
			return ops::eless(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 14:
			return ops::emore(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 15:
			return ops::bor(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 16:
			return ops::bxor(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 17:
			return ops::band(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 18:
			return ops::bshl(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 19:
			return ops::bshr(NULL,
				children[0]->evaluate(scope, stack_trace),
				children[1]->evaluate(scope, stack_trace),
				&token, stack_trace);
		case 20:
			return ops::call(scope,
				children[0],
				children[1]->evaluate(scope, stack_trace).getVector(&token, stack_trace),
				&token, stack_trace);
		default:
			return sym_t();
	}
}

const std::string CallOpI::compile() const
{
	std::string ca = "{";
	size_t i = 0;
	for (const i_ptr_t &e : children) {
		if (i++ > 0)
			ca += ", ";
		ca += e->compile();
	}
	ca += "}";
	return C_BINARY("CallOpI", std::to_string(id), ca);
}