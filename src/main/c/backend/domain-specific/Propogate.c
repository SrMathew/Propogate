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

/** PRIVATE HELPER FUNCTIONS: SYMBOL TABLE */



/** PRIVATE FUNCTIONS */



/** LOGIC OPERATIONS */

/** PUBLIC FUNCTIONS */

