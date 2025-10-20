#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonParser.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeBisonActionsModule();

/**
 * Bison semantic actions.
 */

Variable *StringVariableSemanticAction(const char *variable /*, bool value*/);
Formula *VariableFormulaSemanticAction(Variable *variable);
Formula *BinaryFormulaSemanticAction(Formula *leftFormula, Formula *rightFormula, BinaryFormulaType formulaType);
Formula *UnaryFormulaSemanticAction(Formula *formula, UnaryFormulaType formulaType);
FormulaList *FormulaFormulaListSemanticAction(Formula *formula, FormulaList *formulaList);
Program *FormulaListProgramSemanticAction(FormulaList *formulaList);
#endif
