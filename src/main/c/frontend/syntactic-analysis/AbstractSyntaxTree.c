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
		if (expressionList->expression != NULL)
		{
			destroyExpression(expressionList->expression);
		}
		free(expressionList);
	}
}

void destroyPropogate(Propogate *propogate)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (propogate != NULL)
	{
		if (propogate->expressionList != NULL)
		{
			destroyExpressionList(propogate->expressionList);
		}
		free(propogate);
	}
}

void destroyText(Text *text)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (text != NULL)
	{
		free(text->text);
		free(text);
	}
}

void destroyMath(Math *math)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (math != NULL)
	{
		if (math->next != NULL)
		{
			destroyMath(math->next);
		}
		switch (math->mathType)
		{
		case PROPOGATE:
			if (math->propogate != NULL)
			{
				destroyPropogate(math->propogate);
			}
			break;
		case TEXT_ONLY:
			if (math->text != NULL)
			{
				destroyText(math->text);
			}
			break;
		}
		free(math);
	}
}

void destroyElement(Element *element)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (element != NULL)
	{
		switch (element->elementType)
		{
		case MATH:
			if (element->math != NULL)
			{
				destroyMath(element->math);
			}
			break;
		case ENVIRONMENT:
			if (element->environment != NULL)
			{
				destroyText(element->environment);
			}
			if (element->content != NULL)
			{
				destroyContent(element->content);
			}
			break;
		case TEXT_ONLY:
			if (element->text != NULL)
			{
				destroyText(element->text);
			}
		}
		free(element);
	}
}

void destroyContent(Content *content)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (content != NULL)
	{
		if (content->next != NULL)
		{
			destroyContent(content->next);
		}
		if (content->element != NULL)
		{
			destroyElement(content->element);
		}
		free(content);
	}
}

void destroyProgram(Program *program)
{
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL)
	{
		if (program->content != NULL)
		{
			destroyContent(program->content);
		}
		free(program);
	}
}
