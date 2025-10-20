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

Variable *StringVariableSemanticAction(const char *name);
Formula *VariableFormulaSemanticAction(Variable *variable);
Formula *BinaryFormulaSemanticAction(Formula *leftFormula, Formula *rightFormula, BinaryFormulaType formulaType);
Formula *UnaryFormulaSemanticAction(Formula *formula, UnaryFormulaType formulaType);
Definition *VariableDefinitionSemanticAction(Variable *variable, bool value);
Definition *FormulaDefinitionSemanticAction(Formula *formula, const char *name);
Expression *DefinitionExpressionSemanticAction(Definition *definition);
Expression *FormulaExpressionSemanticAction(Formula *formula);
ExpressionList *ExpressionListSemanticAction(Expression *expression, ExpressionList *expressionList);
Program *ExpressionListProgramSemanticAction(ExpressionList *expressionList);
#endif
