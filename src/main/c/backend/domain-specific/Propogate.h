#ifndef PROPOGATE_HEADER
#define PROPOGATE_HEADER

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/language/String.h"
#include <stdbool.h>


/** Initialize module's internal state. */
ModuleDestructor initializePropogateModule();

/**
 * The result of boolean evaluation.
 */
typedef struct {
	bool succeeded;
	bool value;
} EvaluationResult;

/**
 * Evaluates a specific formula recursively.
 * Exposed to allow granular testing or usage by other modules.
 */
EvaluationResult evaluateFormula(Formula * formula);

/**
 * Executes the logic propagation on the full program.
 * It iterates through the expressions list, updating variable definitions
 * and evaluating formulas.
 * * Returns the result of the last/bigger formula evaluated in the list.
 */
EvaluationResult executePropogate(CompilerState * compilerState);

#endif