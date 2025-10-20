%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

// TODO check which function will be recycle, modified or deleted
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus ParenthesisLexemeAction(TokenLabel label);
//extern CompilationStatus SubexpressionLexemeAction();
extern CompilationStatus UnknownLexemeAction();

extern CompilationStatus EnterLaTeXMathModeLexemeAction(FlexContext context);
extern CompilationStatus LeaveLaTeXMathModeLexemeAction();
extern CompilationStatus EnterPropogateLexemeAction(FlexContext context);
extern CompilationStatus LeavePropogateLexemeAction();

extern CompilationStatus BinaryOperatorLexemeAction(TokenLabel label);
extern CompilationStatus UnaryOperatorLexemeAction(TokenLabel label);          //TODO: label?

extern CompilationStatus DefineOperatorLexemeAction();  //TODO: label?
extern CompilationStatus FormulaLexemeAction(TokenLabel label);

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MATHMODE
%x PROPOGATE

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

"\\("                               { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
"$"                                 { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
"\\begin{math}"                     { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
"\\["                               { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
"\\begin{displaymath}"              { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
"\\begin{equation}"                 { return EnterLaTeXMathModeLexemeAction(MATHMODE); }

<MATHMODE>"\\propogate{"            { return EnterPropogateLexemeAction(PROPOGATE); }

<PROPOGATE>"\\wedge"                { return BinaryOperatorLexemeAction(AND); }
<PROPOGATE>"\\vee"                  { return BinaryOperatorLexemeAction(OR); }
<PROPOGATE>"\\rightarrow"           { return BinaryOperatorLexemeAction(IMPLY); }

<PROPOGATE>"\\neg"                  { return UnaryOperatorLexemeAction(NEG); }

<PROPOGATE>"="                      { return DefineOperatorLexemeAction(); }

<PROPOGATE>[a-z]+                   { return FormulaLexemeAction(VAR_SYMBOL); }

<PROPOGATE>"\\alpha"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\beta"                 { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\chi"                  { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\delta"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\epsilon"              { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\eta"                  { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\gamma"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\iota"                 { return FormulaLexemeAction(FORM_SYMBOL); }

<PROPOGATE>"\\kappa"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\lambda"               { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\mu"                   { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\nu"                   { return FormulaLexemeAction(FORM_SYMBOL); }

<PROPOGATE>"\\omega"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\phi"                  { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\pi"                   { return FormulaLexemeAction(FORM_SYMBOL); }

<PROPOGATE>"\\psi"                  { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\rho"                  { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\sigma"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\tau"                  { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\theta"                { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\upsilon"              { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\xi"                   { return FormulaLexemeAction(FORM_SYMBOL); }
<PROPOGATE>"\\zeta"                 { return FormulaLexemeAction(FORM_SYMBOL); }

<PROPOGATE>"("						{ return ParenthesisLexemeAction(OPEN_PARENTHESIS); }
<PROPOGATE>")"                      { return ParenthesisLexemeAction(CLOSE_PARENTHESIS); }

<PROPOGATE>","                      { return IgnoredLexemeAction(); }
<PROPOGATE>"\n"                     { return IgnoredLexemeAction(); }

<PROPOGATE>"}"                      { return LeavePropogateLexemeAction(); }

<PROPOGATE>{whitespace}             { return IgnoredLexemeAction(); }
<PROPOGATE>.                        { return UnknownLexemeAction(); }

<MATHMODE>"\\)"                     { return LeaveLaTeXMathModeLexemeAction(); }
<MATHMODE>"$"                       { return LeaveLaTeXMathModeLexemeAction(); }
<MATHMODE>"\\end{math}"             { return LeaveLaTeXMathModeLexemeAction(); }
<MATHMODE>"\\]"                     { return LeaveLaTeXMathModeLexemeAction(); }
<MATHMODE>"\\end{displaymath}"      { return LeaveLaTeXMathModeLexemeAction(); }
<MATHMODE>"\\end{equation}"         { return LeaveLaTeXMathModeLexemeAction(); }

<MATHMODE>.                         { return IgnoredLexemeAction(); }

.									{ return IgnoredLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
