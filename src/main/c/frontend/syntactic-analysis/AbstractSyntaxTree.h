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
typedef enum DefinitionType DefinitionType;
typedef enum ExpressionType ExpressionType;
typedef enum MathType MathType;
typedef enum ElementType ElementType;

typedef struct Variable Variable;
typedef struct Formula Formula;
typedef struct Definition Definition;
typedef struct Expression Expression;
typedef struct ExpressionList ExpressionList;
typedef struct Propogate Propogate;
typedef struct Text Text;
typedef struct Math Math;
typedef struct Element Element;
typedef struct Content Content;
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

enum DefinitionType
{
	VAR_DEF,
	FORM_DEF
};

enum ExpressionType
{
	DEF_EXPR,
	FORM_EXPR
};

enum MathType
{
	PROPOGATE,
	GARBAGE
};

enum ElementType
{
	MATH,
	ENVIRONMENT,
	TEXT_ONLY
};

struct Variable
{
	char *name;
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
		struct
		{
			Formula *formula;
			UnaryFormulaType unaryFormulaType;
		};
		struct
		{
			Variable *variable;
		};
	};
	FormulaType formulaType;
};

struct Definition
{
	union
	{
		struct
		{
			Variable *variable;
			bool value;
		};
		struct
		{
			Formula *formula;
			char *name;
		};
	};
	DefinitionType definitionType;
};

struct Expression
{
	union
	{
		Definition *definition;
		Formula *formula;
	};
	ExpressionType expressionType;
};

struct ExpressionList
{
	Expression *expression;
	ExpressionList *next;
};

struct Propogate
{
	ExpressionList *expressionList;
};

struct Text
{
	char *text;
};

struct Math
{
	union
	{
		Propogate *propogate;
		struct 
		{
			Text *text;
			Math *next;
		};
		MathType mathType;
	};
};

struct Element
{
	union
	{
		Math *math;
		struct
		{
			Text *environment;
			Content *content;
		};
		Text *text;
	};
	ElementType elementType;	
};

struct Content
{
	Element *element;
	Content *next;
};

struct Program
{
	Content *content;
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyVariable(Variable *variable);
void destroyFormula(Formula *formula);
void destroyDefinition(Definition *definition);
void destroyExpression(Expression *expression);
void destroyExpressionList(ExpressionList *expressionList);
void destroyPropogate(Propogate *propogate);
void destroyText(Text *text);
void destroyMath(Math *math);
void destroyElement(Element *element);
void destroyContent(Content *content);
void destroyProgram(Program *program);

#endif
