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
		free(variable->variable);
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
			destroyFormula(formula->leftFormula);
			destroyFormula(formula->rightFormula);
			break;
		case UNARY:
			destroyFormula(formula->formula);
			break;
		case VAR_FORMULA:
			destroyVariable(formula->variable);
			break;
		}
		free(formula);
	}
}

void destroyFormulaList(FormulaList *formulaList)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (formulaList != NULL)
	{
		if (formulaList->next != NULL)
		{
			destroyFormulaList(formulaList->next);
		}
		destroyFormula(formulaList->formula);
		free(formulaList);
	}
}

void destroyProgram(Program *program)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL)
	{
		destroyFormulaList(program->formulaList);
		free(program);
	}
}
