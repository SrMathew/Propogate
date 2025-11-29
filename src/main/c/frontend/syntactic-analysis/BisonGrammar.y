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

%union {
	/** Terminals. */

	char* string;
	bool value;
	TokenLabel token;

	/** Non-terminals. */

	Variable * variable;
	Formula * formula;
	Definition * definition;
	Expression * expression;
	ExpressionList * expressionList;
	Propogate * propogate;
	Text * text;
	Math * math;
	Element * element;
	Content * content;
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
 
%destructor { destroyVariable($$); } <variable>
%destructor { destroyFormula($$); } <formula>
%destructor { destroyDefinition($$); } <definition>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyExpressionList($$); } <expressionList>
%destructor { destroyPropogate($$); } <propogate>
%destructor { destroyText($$); } <text>
%destructor { destroyMath($$); } <math>
%destructor { destroyElement($$); } <element>
%destructor { destroyContent($$); } <content>

/** Terminals. */
%token <string> VAR_NAME
%token <string> FORM_NAME

%token <value> VALUE

%token <token> AND
%token <token> OR
%token <token> IMPLY
%token <token> NEG

%token <token> EQUALS

%token <token> SEPARATOR

%token <token> PROPOGATE_COMMAND

%token <token> OPEN_PARENTHESIS
%token <token> OPEN_BRACKET
%token <token> OPEN_BRACE
%token <token> CLOSE_PARENTHESIS
%token <token> CLOSE_BRACKET
%token <token> CLOSE_BRACE

%token <token> BEGIN_ENVIRONMENT
%token <token> END_ENVIRONMENT

%token <string> MM_ENVIRONMENT
%token <token> MM_OPEN_PARENTHESIS
%token <token> MM_OPEN_BRACKET
%token <token> MM_OPEN_DOLLAR
%token <token> MM_OPEN_DOUBLEDOLLAR
%token <token> MM_CLOSE_PARENTHESIS
%token <token> MM_CLOSE_BRACKET
%token <token> MM_CLOSE_DOLLAR
%token <token> MM_CLOSE_DOUBLEDOLLAR

%token <token> DOLLAR

%token <string> TEXT

%token <token> IGNORED
%token <token> UNKNOWN

/** Non-terminals. */
%type <variable> variable
%type <formula> formula
%type <definition> definition
%type <expression> expression
%type <expressionList> expressionList
%type <propogate> propogate
%type <text> text
%type <math> math
%type <element> element
%type <content> content
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left AND
%left OR IMPLY
%left NEG
%%
// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: 
	  content 																	{ $$ = ContentProgramSemanticAction($1); }
	;

content: 
	  content element  															{ $$ = ElementContentSemanticAction($2, $1); }
	| %empty																	{ $$ = NULL; }
	;

// TODO: element puede ser basura o estar en mathmode
element: 
	  /* MM_OPEN_PARENTHESIS math MM_CLOSE_PARENTHESIS								{ $$ = MathBracketElementSemanticAction($2); }
	| MM_OPEN_BRACKET math MM_CLOSE_BRACKET	 			 						{ $$ = MathBracketElementSemanticAction($2); }
	| MM_OPEN_DOLLAR math MM_CLOSE_DOLLAR		 		 						{ $$ = MathBracketElementSemanticAction($2); }
	| MM_OPEN_DOUBLEDOLLAR math MM_CLOSE_DOUBLEDOLLAR							{ $$ = MathBracketElementSemanticAction($2); }
	| BEGIN_ENVIRONMENT OPEN_BRACE MM_ENVIRONMENT CLOSE_BRACE math END_ENVIRONMENT OPEN_BRACE MM_ENVIRONMENT CLOSE_BRACE
																				{ $$ = MathEnvironmentElementSemanticAction($5, $3, $8); } */
	DOLLAR math DOLLAR														{ $$ = MathBracketElementSemanticAction($2); }
	| BEGIN_ENVIRONMENT OPEN_BRACE text CLOSE_BRACE content END_ENVIRONMENT OPEN_BRACE text CLOSE_BRACE
																				{ $$ = ContentElementSemanticAction($5, $3, $8); }
	| text																		{ $$ = TextElementSemanticAction($1); }
	;

math: 
	  math PROPOGATE_COMMAND OPEN_BRACE propogate CLOSE_BRACE					{ $$ = PropogateMathSemanticAction($4, $1); }
	| math text																	{ $$ = TextMathSemanticAction($2, $1); }
	| %empty																	{ $$ = NULL;}
	;

text: 
	  TEXT 																		{ $$ = StringTextSemanticAction($1); }										
	;

propogate: 
	  expressionList															{ $$ = ExpressionListPropogateSemanticAction($1); }
	;

expressionList: 
	  expressionList SEPARATOR expression										{ $$ = ExpressionListSemanticAction($3, $1); }
	| expression																{ $$ = ExpressionListSemanticAction($1, NULL); }
	| %empty																	{ $$ = NULL; }
	;

expression: 
	  definition																{ $$ = DefinitionExpressionSemanticAction($1); }
	| formula																	{ $$ = FormulaExpressionSemanticAction($1); }
	;

definition: 
	  variable EQUALS VALUE														{ $$ = VariableDefinitionSemanticAction($1, $3);}
	| FORM_NAME EQUALS formula													{ $$ = FormulaDefinitionSemanticAction($3, $1);}
	;

/*
NEG OPEN_PARENTHESIS formula[left] OR formula[right] AND OPEN_PARENTHESIS NEG OPEN_PARENTHESIS formula[left] AND formula[right] { $$ = BinaryFormulaSemanticAction($left, $right, XNOR); }
OPEN_PARENTHESIS formula[left] OR formula[right] AND OPEN_PARENTHESIS NEG OPEN_PARENTHESIS formula[left] AND formula[right] { $$ = BinaryFormulaSemanticAction($left, $right, XOR)}
NEG OPEN_PARENTHESIS formula[left] OR formula[right] CLOSE_PARENTHESIS 		{ $$ = BinaryFormulaSemanticAction($left, $right, NOR); }
NEG OPEN_PARENTHESIS formula[left] AND formula[right] CLOSE_PARENTHESIS	{ $$ = BinaryFormulaSemanticAction($left, $right, NAND); }
*/

formula: 
	  OPEN_PARENTHESIS formula[left] AND formula[right] CLOSE_PARENTHESIS		{ $$ = BinaryFormulaSemanticAction($left, $right, AND_TYPE); }
	| OPEN_PARENTHESIS formula[left] OR formula[right] CLOSE_PARENTHESIS		{ $$ = BinaryFormulaSemanticAction($left, $right, OR_TYPE); }
	| OPEN_PARENTHESIS formula[left] IMPLY formula[right] CLOSE_PARENTHESIS		{ $$ = BinaryFormulaSemanticAction($left, $right, IMPLY_TYPE); }
	| NEG OPEN_PARENTHESIS formula CLOSE_PARENTHESIS							{ $$ = UnaryFormulaSemanticAction($3, NEG_TYPE); }
	| OPEN_PARENTHESIS formula CLOSE_PARENTHESIS								{ $$ = UnaryFormulaSemanticAction($2, NOTHING); }
	| variable																	{ $$ = VariableFormulaSemanticAction($1); }
	;

variable: VAR_NAME																{ $$ = StringVariableSemanticAction($1); }
	;
%%
