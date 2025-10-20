#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule()
{
	if (_logger != NULL)
	{
		logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreeModule()
{
	_logger = createLogger("AbstractSyntaxTree");
	return _shutdownAbstractSyntaxTreeModule;
}

/* PUBLIC FUNCTIONS */

void destroyVariable(Variable *variable)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (variable != NULL)
	{
		free(variable->name);
		free(variable);
	}
}

void destroyFormula(Formula *formula)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (formula != NULL)
	{
		switch (formula->formulaType)
		{
		case BINARY:
			if (formula->leftFormula != NULL)
			{
				destroyFormula(formula->leftFormula);
			}
			if (formula->rightFormula != NULL)
			{
				destroyFormula(formula->rightFormula);
			}
			break;
		case UNARY:
			if (formula->formula != NULL)
			{
				destroyFormula(formula->formula);
			}
			break;
		case VAR_FORMULA:
			if (formula->variable != NULL)
			{
				destroyVariable(formula->variable);
			}
			break;
		}
		free(formula);
	}
}

void destroyDefinition(Definition *definition)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (definition != NULL)
	{
		switch (definition->definitionType)
		{
		case VAR_DEF:
			if (definition->variable != NULL)
			{
				destroyVariable(definition->variable);
			}
			break;
		case FORM_DEF:
			if (definition->formula != NULL)
			{
				free(definition->name);
				destroyFormula(definition->formula);
				//free(definition->name);
			}
			break;
		}
		free(definition);
	}
}

void destroyExpression(Expression *expression)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL)
	{
		switch (expression->expressionType)
		{
		case DEF_EXPR:
			if (expression->definition != NULL)
			{
				destroyDefinition(expression->definition);
			}
			break;
		case FORM_EXPR:
			if (expression->formula != NULL)
			{
				destroyFormula(expression->formula);
			}
			break;
		}
		free(expression);
	}
}

void destroyExpressionList(ExpressionList *expressionList)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expressionList != NULL)
	{
		if (expressionList->next != NULL)
		{
			destroyExpressionList(expressionList->next);
		}
		destroyExpression(expressionList->expression);
		free(expressionList);
	}
}

void destroyProgram(Program *program)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL)
	{
		destroyExpressionList(program->expressionList);
		free(program);
	}
}
