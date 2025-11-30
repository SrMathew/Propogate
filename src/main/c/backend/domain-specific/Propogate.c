#include "Propogate.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

/**
 * Symbol Table Node to store variable states.
 */
typedef struct VariableState
{
	char *name;
	bool value;
	struct VariableState *next;
} VariableState;

static VariableState *_symbolTable = NULL;

/** Shutdown module's internal state. */
void _shutdownPropogateModule()
{
	if (_logger != NULL)
	{
		logDebugging(_logger, "Destroying module: Propogate...");
		destroyLogger(_logger);
		_logger = NULL;
	}

	// Clean symbol table
	VariableState *current = _symbolTable;
	while (current != NULL)
	{
		VariableState *next = current->next;
		if (current->name != NULL)
		{
			free(current->name);
		}
		free(current);
		current = next;
	}
	_symbolTable = NULL;
}

ModuleDestructor initializePropogateModule()
{
	_logger = createLogger("Propogate");
	_symbolTable = NULL;
	return _shutdownPropogateModule;
}

/** PRIVATE FUNCTIONS */
static void _setVariableValue(const char *name, bool value);
static EvaluationResult _getVariableValue(const char *name);
static EvaluationResult _invalidEvaluation();
static EvaluationResult _operateBinary(BinaryFormulaType type, EvaluationResult left, EvaluationResult right);
static EvaluationResult _operateUnary(UnaryFormulaType type, EvaluationResult op);
static void _processContent(Content *content, EvaluationResult *globalResult);
static void _processPropogateBlock(Propogate *propogateNode, EvaluationResult *globalResult);
static void _processElement(Element *element, EvaluationResult *globalResult);

/** SYMBOL TABLE */
static void _setVariableValue(const char *name, bool value)
{
	VariableState *current = _symbolTable;

	// Update existing
	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			current->value = value;
			logDebugging(_logger, "Updated variable '%s' to %s", name, value ? "true" : "false");
			return;
		}
		current = current->next;
	}

	// New entry
	VariableState *newState = (VariableState *)calloc(1, sizeof(VariableState));
	if (newState == NULL)
	{
		logError(_logger, "Out of memory defining variable %s", name);
		return;
	}

	newState->name = concatenate(1, name);
	newState->value = value;
	newState->next = _symbolTable;
	_symbolTable = newState;

	logDebugging(_logger, "Defined variable '%s' = %s", name, value ? "true" : "false");
}

static EvaluationResult _getVariableValue(const char *name)
{
	VariableState *current = _symbolTable;
	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return (EvaluationResult){.succeeded = true, .value = current->value};
		}
		current = current->next;
	}
	logDebugging(_logger, "Variable '%s' used without explicit definition. Defaulting to FALSE.", name);
	return (EvaluationResult){.succeeded = true, .value = false};
}

static EvaluationResult _invalidEvaluation()
{
	return (EvaluationResult){.succeeded = false, .value = false};
}

/** LOGIC OPERATIONS */
static EvaluationResult _operateBinary(BinaryFormulaType type, EvaluationResult left, EvaluationResult right)
{
	if (!left.succeeded || !right.succeeded)
		return _invalidEvaluation();

	bool resultVal = false;
	switch (type)
	{
	case AND_TYPE:
		resultVal = left.value && right.value;
		break;
	case OR_TYPE:
		resultVal = left.value || right.value;
		break;
	case IMPLY_TYPE:
		// A -> B  ===  !A || B
		resultVal = (!left.value) || right.value;
		break;
	default:
		logError(_logger, "Unknown binary operator type.");
		return _invalidEvaluation();
	}
	return (EvaluationResult){.succeeded = true, .value = resultVal};
}

static EvaluationResult _operateUnary(UnaryFormulaType type, EvaluationResult op)
{
	if (!op.succeeded)
		return _invalidEvaluation();

	if (type == NEG_TYPE)
	{
		return (EvaluationResult){.succeeded = true, .value = !op.value};
	}
	return op;
}

/** FUNCTION TO WALK THROUGH THE TREE */
static void _processPropogateBlock(Propogate *propogateNode, EvaluationResult *globalResult)
{
	if (propogateNode == NULL || propogateNode->expressionList == NULL)
		return;

	ExpressionList *currentExpr = propogateNode->expressionList;

	while (currentExpr != NULL)
	{
		Expression *expr = currentExpr->expression;
		if (expr != NULL)
		{
			switch (expr->expressionType)
			{
			case DEF_EXPR:
				if (expr->definition != NULL && expr->definition->definitionType == VAR_DEF)
				{
					_setVariableValue(expr->definition->variable->name, expr->definition->value);
				}
				break;

			case FORM_EXPR:
				if (expr->formula != NULL)
				{
					*globalResult = evaluateFormula(expr->formula);
					logDebugging(_logger, "Computed Formula Result: %s", globalResult->value ? "TRUE" : "FALSE");
				}
				break;
			}
		}
		currentExpr = currentExpr->next;
	}
}

static void _processElement(Element *element, EvaluationResult *globalResult)
{
	if (element == NULL)
		return;

	switch (element->elementType)
	{
	case MATH:
		// CORRECCIÓN: Iterar sobre la lista enlazada de nodos Math
		// El AST define Math como una lista: struct Math { ... Math *next; ... }
		if (element->math != NULL)
		{
			Math *currentMath = element->math;
			while (currentMath != NULL)
			{
				// Verificamos el tipo de CADA nodo en la lista
				if (currentMath->mathType == PROPOGATE)
				{
					if (currentMath->propogate != NULL)
					{
						logDebugging(_logger, "Processing PROPOGATE block found in Math list...");
						_processPropogateBlock(currentMath->propogate, globalResult);
					}
				}
				// Avanzamos al siguiente nodo (puede ser texto, otro propogate, o NULL)
				currentMath = currentMath->next;
			}
		}
		break;

	case ENVIRONMENT:
		// Environment tiene contenido recursivo
		if (element->content != NULL)
		{
			logDebugging(_logger, "Entering Environment...");
			_processContent(element->content, globalResult);
		}
		break;

	case TEXT_ONLY:
		// Ignorar texto plano fuera de bloques matemáticos
		break;
	}
}

static void _processContent(Content *content, EvaluationResult *globalResult)
{
	Content *current = content;
	while (current != NULL)
	{
		_processElement(current->element, globalResult);
		current = current->next;
	}
}

/** PUBLIC FUNCTIONS */
EvaluationResult evaluateFormula(Formula *formula);
EvaluationResult executePropogate(CompilerState *compilerState);

EvaluationResult evaluateFormula(Formula *formula)
{
	if (formula == NULL)
		return _invalidEvaluation();

	switch (formula->formulaType)
	{
	case VAR_FORMULA:
		if (formula->variable != NULL)
		{
			return _getVariableValue(formula->variable->name);
		}
		break;

	case BINARY:
	{
		EvaluationResult left = evaluateFormula(formula->leftFormula);
		EvaluationResult right = evaluateFormula(formula->rightFormula);
		return _operateBinary(formula->binaryFormulaType, left, right);
	}

	case UNARY:
	{
		EvaluationResult child = evaluateFormula(formula->formula);
		return _operateUnary(formula->unaryFormulaType, child);
	}
	}
	return _invalidEvaluation();
}

EvaluationResult executePropogate(CompilerState *compilerState)
{
	logDebugging(_logger, "Executing Propogate Module on Document Tree...");

	Program *program = (Program *)compilerState->abstractSyntaxtTree;

	if (program == NULL)
	{
		logError(_logger, "Empty program or AST.");
		return _invalidEvaluation();
	}

	EvaluationResult finalResult = {.succeeded = true, .value = false};

	if (program->content != NULL)
	{
		_processContent(program->content, &finalResult);
	}
	else
	{
		logDebugging(_logger, "Program has no content.");
	}

	compilerState->value = finalResult.value ? true : false;
	// We assume succeeded is generally true unless a specific formula failed
	return finalResult;
}