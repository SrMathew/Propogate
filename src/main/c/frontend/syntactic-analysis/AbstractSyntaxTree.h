#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();

/**
 * This type definitions allows self-referencing types (e.g., an expression
 * that is made of another expressions, such as talking about you in 3rd
 * person, but without the madness).
 */

typedef enum BinaryFormulaType BinaryFormulaType;
typedef enum UnaryFormulaType UnaryFormulaType;
typedef enum FormulaType FormulaType;

typedef struct Variable Variable;
typedef struct Formula Formula;

typedef struct Program Program;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum BinaryFormulaType
{
	AND_TYPE,
	OR_TYPE,
	IMPLY_TYPE
};

enum UnaryFormulaType
{
	NEG_TYPE,
	NOTHING
};

enum FormulaType
{
	BINARY,
	UNARY,
	VAR_FORMULA
};

struct Variable
{
	char * variable;
};

struct Formula
{
	union
	{
		struct
		{
			Formula *leftFormula;
			Formula *rightFormula;
			BinaryFormulaType binaryFormulaType;
		};
		struct {
			Formula * formula;
			UnaryFormulaType unaryFormulaType;
		};
		struct {
			Variable * variable;
		};
	};
	FormulaType formulaType;
};

struct Program
{
	Formula *formula;
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyFormula(Formula *formula);
void destroyVariable(Variable *variable);

void destroyProgram(Program *program);

#endif
