#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Variable * StringVariableSemanticAction(const char* variable) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * newVariable = calloc(1, sizeof(Variable));
	newVariable->variable = variable;
	return newVariable;
}

Formula * BinaryFormulaSemanticAction(Formula * leftFormula, Formula * rightFormula, BinaryFormulaType formulaType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula * formula = calloc(1, sizeof(Formula));
	formula->leftFormula = leftFormula;
	formula->rightFormula = rightFormula;
	formula->binaryFormulaType = formulaType;
	formula->formulaType = BINARY;
	return formula;
}

Formula * UnaryFormulaSemanticAction(Formula * formula, UnaryFormulaType formulaType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula * newFormula = calloc(1, sizeof(Formula));
	newFormula->formula = formula;
	newFormula->unaryFormulaType = formulaType;
	formula->formulaType = UNARY;
	return newFormula;
}

Formula * VariableFormulaSemanticAction(Variable * variable) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Formula * formula = calloc(1, sizeof(Formula));
	formula->variable = variable;
	formula->formulaType = VAR_SYMBOL;
	return formula;
}

Program * FormulaProgramSemanticAction(Formula * formula) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->formula = formula;
	_compilerState->abstractSyntaxtTree = program;
	return program;
}