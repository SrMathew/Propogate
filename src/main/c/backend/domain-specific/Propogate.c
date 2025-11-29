#include "Propogate.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;


/**
 * Symbol Table Node to store variable states.
 */
typedef struct VariableState {
	char * name;
	bool value;
	struct VariableState * next;
} VariableState;

static VariableState * _symbolTable = NULL;

/** Shutdown module's internal state. */
void _shutdownPropogateModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Propogate...");
		destroyLogger(_logger);
		_logger = NULL;
	}

    //Clean symbol table
    VariableState * current = _symbolTable;
	while (current != NULL) {
		VariableState * next = current->next;
		if (current->name != NULL) {
            free(current->name); 
        }
		free(current);
		current = next;
	}
	_symbolTable = NULL;
}

ModuleDestructor initializePropogateModule() {
	_logger = createLogger("Propogate");
	_symbolTable = NULL;
	return _shutdownPropogateModule;
}

/** PRIVATE FUNCTIONS */
/** SYMBOL TABLE */


static EvaluationResult _invalidEvaluation() {
	return (EvaluationResult){ .succeeded = false, .value = false };
}


/** LOGIC OPERATIONS */
static EvaluationResult _operateBinary(BinaryFormulaType type, EvaluationResult left, EvaluationResult right) {
	if (!left.succeeded || !right.succeeded) return _invalidEvaluation();
	
	bool resultVal = false;
	switch (type) {
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
	return (EvaluationResult){ .succeeded = true, .value = resultVal };
}

static EvaluationResult _operateUnary(UnaryFormulaType type, EvaluationResult op) {
	if (!op.succeeded) return _invalidEvaluation();

	if (type == NEG_TYPE) {
		return (EvaluationResult){ .succeeded = true, .value = !op.value };
	}
	return op;
}


/** PUBLIC FUNCTIONS */
EvaluationResult executePropogate(CompilerState * compilerState) {
	logDebugging(_logger, "Executing Propogate Module on Document Tree...");
	
	Program * program = (Program *)compilerState->abstractSyntaxtTree;
	
	if (program == NULL) {
		logError(_logger, "Empty program or AST.");
		return _invalidEvaluation();
	}

    // Initialize result to false (or success=true, value=false as default)
	EvaluationResult finalResult = { .succeeded = true, .value = false };

    // Traverse the document tree from the root content
    if (program->content != NULL) {
        _processContent(program->content, &finalResult);
    } else {
        logDebugging(_logger, "Program has no content.");
    }

	compilerState->value = finalResult.value ? true : false;
    // We assume succeeded is generally true unless a specific formula failed, 
    // which is captured in finalResult
	return finalResult;
}