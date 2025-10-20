%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}

%}

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.error detailed
%locations

// TODO define here datatypes for interpreter
%union {
	/** Terminals. */

	//char variable;	//p, q, r
	//TODO char* veremos...
	char* str;
	//int integer;
	//bool boolean;
	TokenLabel token;

	/** Non-terminals. */

	Formula * formula;
	Variable * variable;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
 /*
%destructor { destroyConstant($$); } <constant>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyFactor($$); } <factor>
*/
%destructor { destroyFormula($$); } <formula>
%destructor { destroyVariable($$); } <variable>
//TODO build our own destructor for our datatypes

// TODO define data type for each symbol in the entry
/** Terminals. */
/*
%token <integer> INTEGER
%token <token> ADD
%token <token> CLOSE_BRACE
%token <token> CLOSE_COMMENT
%token <token> CLOSE_PARENTHESIS
%token <token> DIV
%token <token> MUL
%token <token> OPEN_BRACE
%token <token> OPEN_COMMENT
%token <token> OPEN_PARENTHESIS
%token <token> SUB
*/
%token <str> VAR_SYMBOL
%token <str> FORM_SYMBOL
%token <token> CLOSE_PARENTHESIS
%token <token> AND
%token <token> OR
%token <token> IMPLY
%token <token> NEG
%token <token> OPEN_PARENTHESIS

%token <token> EQUALS

%token <token> IGNORED
%token <token> UNKNOWN

/** Non-terminals. */
/*
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program
*/
%type <variable> variable
%type <formula> formula
%type <program> program

// TODO define precedence and associativity
/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
 /*
%left ADD SUB
%left MUL DIV
*/
%left AND OR IMPLY
%left NEG

// TODO DEFINE Grammar here
%%
// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
/*
program: expression											{ $$ = ExpressionProgramSemanticAction($1); }
	;

expression: expression[left] ADD expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]				{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor												{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS		{ $$ = ExpressionFactorSemanticAction($2); }
	| constant												{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER											{ $$ = IntegerConstantSemanticAction($1); }
	;
*/
program: formula																{ $$ = FormulaProgramSemanticAction($1); }
	;

formula: OPEN_PARENTHESIS formula[left] AND formula[right] CLOSE_PARENTHESIS	{ $$ = BinaryFormulaSemanticAction($left, $right, AND_TYPE); }
	| OPEN_PARENTHESIS formula[left] OR formula[right] CLOSE_PARENTHESIS 		{ $$ = BinaryFormulaSemanticAction($left, $right, OR_TYPE); }
	| OPEN_PARENTHESIS formula[left] IMPLY formula[right] CLOSE_PARENTHESIS		{ $$ = BinaryFormulaSemanticAction($left, $right, IMPLY_TYPE); }
	| NEG OPEN_PARENTHESIS formula CLOSE_PARENTHESIS							{ $$ = UnaryFormulaSemanticAction($3, NEG_TYPE); }
	| OPEN_PARENTHESIS formula CLOSE_PARENTHESIS								{ $$ = UnaryFormulaSemanticAction($2, NOTHING); }
	| formula																	{ $$ = UnaryFormulaSemanticAction($1, NOTHING); }
	| variable																	{ $$ = VariableFormulaSemanticAction($1); }
	//form_symbol?
	;

variable: VAR_SYMBOL																{ $$ = StringVariableSemanticAction($1); }
	;
%%
