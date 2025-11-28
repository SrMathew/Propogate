#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState *_compilerState = NULL;
static Logger *_logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule()
{
	if (_logger != NULL)
	{
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState *compilerState)
{
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char *functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char *functionName)
{
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Variable *StringVariableSemanticAction(char *name)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (!name) logError(_logger, strcat(": name is NULL", __FUNCTION__));
    else logDebugging(_logger, strcat(": name='%s'", __FUNCTION__), name);
	Variable *newVariable = calloc(1, sizeof(Variable));
	newVariable->name = name;
	return newVariable;
}

Formula *VariableFormulaSemanticAction(Variable *variable)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula *formula = calloc(1, sizeof(Formula));
	formula->variable = variable;
	formula->formulaType = VAR_FORMULA;
	return formula;
}

Formula *BinaryFormulaSemanticAction(Formula *leftFormula, Formula *rightFormula, BinaryFormulaType formulaType)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula *formula = calloc(1, sizeof(Formula));
	formula->leftFormula = leftFormula;
	formula->rightFormula = rightFormula;
	formula->binaryFormulaType = formulaType;
	formula->formulaType = BINARY;
	return formula;
}

Formula *UnaryFormulaSemanticAction(Formula *formula, UnaryFormulaType formulaType)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula *newFormula = calloc(1, sizeof(Formula));
	newFormula->formula = formula;
	newFormula->unaryFormulaType = formulaType;
	newFormula->formulaType = UNARY;
	return newFormula;
}

Definition *VariableDefinitionSemanticAction(Variable *variable, bool value)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Definition *definition = calloc(1, sizeof(Definition));
	definition->variable = variable;
	definition->value = value;
	definition->definitionType = VAR_DEF;
	return definition;
}

Definition *FormulaDefinitionSemanticAction(Formula *formula, char *name)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (!name) logError(_logger, "StringVariableSemanticAction: name is NULL");
    else logDebugging(_logger, "StringVariableSemanticAction: name='%s'", name);
	Definition *definition = calloc(1, sizeof(Definition));
	definition->formula = formula;
	definition->name = name;
	definition->definitionType = FORM_DEF;
	return definition;
}

Expression *DefinitionExpressionSemanticAction(Definition *definition)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *expression = calloc(1, sizeof(Expression));
	expression->definition = definition;
	expression->expressionType = DEF_EXPR;
	return expression;
}

Expression *FormulaExpressionSemanticAction(Formula *formula)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *expression = calloc(1, sizeof(Expression));
	expression->formula = formula;
	expression->expressionType = FORM_EXPR;
	return expression;
}

ExpressionList *ExpressionListSemanticAction(Expression *expression, ExpressionList *expressionList)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionList *newExpressionList = calloc(1, sizeof(ExpressionList));
	newExpressionList->expression = expression;
	newExpressionList->next = expressionList;
	return newExpressionList;
}

Program *ExpressionListProgramSemanticAction(ExpressionList *expressionList)
{
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program *program = calloc(1, sizeof(Program));
	program->expressionList = expressionList;
	_compilerState->abstractSyntaxtTree = program;
	return program;
}