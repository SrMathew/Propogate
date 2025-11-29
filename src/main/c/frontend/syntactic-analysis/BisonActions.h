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

Variable *StringVariableSemanticAction(char *name);
Formula *VariableFormulaSemanticAction(Variable *variable);
Formula *BinaryFormulaSemanticAction(Formula *leftFormula, Formula *rightFormula, BinaryFormulaType formulaType);
Formula *UnaryFormulaSemanticAction(Formula *formula, UnaryFormulaType formulaType);
Definition *VariableDefinitionSemanticAction(Variable *variable, bool value);
Definition *FormulaDefinitionSemanticAction(Formula *formula, char *name);
Expression *DefinitionExpressionSemanticAction(Definition *definition);
Expression *FormulaExpressionSemanticAction(Formula *formula);
ExpressionList *ExpressionListSemanticAction(Expression *expression, ExpressionList *expressionList);
Propogate *ExpressionListPropogateSemanticAction(ExpressionList *expressionList);
Text *StringTextSemanticAction(char *text);
Math *PropogateMathSemanticAction(Propogate *propogate, Math *next);
Math *TextMathSemanticAction(Text *text, Math *next);
Element *MathElementSemanticAction(Math *math);
Element *ContentElementSemanticAction(Content *content, Text *environmentLeft, Text *environmentRight);
Element *TextElementSemanticAction(Text *text);
Content *ElementContentSemanticAction(Element *element, Content *next);
Program *ContentProgramSemanticAction(Content *content);
#endif
