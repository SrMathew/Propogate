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
extern CompilationStatus UnknownLexemeAction();

extern CompilationStatus EnterCommandDefinedLexemeAction(FlexContext context);
extern CompilationStatus LeaveCommandDefinedLexemeAction();
extern CompilationStatus EnterLaTeXMathModeLexemeAction(FlexContext context);
extern CompilationStatus LeaveLaTeXMathModeLexemeAction();
extern CompilationStatus EnterPropogateLexemeAction(FlexContext context);
extern CompilationStatus LeavePropogateLexemeAction();

extern CompilationStatus SeparatorLexemeAction();
extern CompilationStatus EqualsLexemeAction();

extern CompilationStatus BinaryOperatorLexemeAction(TokenLabel label);
extern CompilationStatus UnaryOperatorLexemeAction(TokenLabel label);

extern CompilationStatus FormulaLexemeAction(TokenLabel label);

extern CompilationStatus TrueLexemeAction();
extern CompilationStatus FalseLexemeAction();
%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */

%x DEFINED
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

"\\newcommand{\\propogate}[1]{#1}"    { return EnterCommandDefinedLexemeAction(DEFINED); }

<DEFINED>"\\("                      { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
<DEFINED>"$"                        { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
<DEFINED>"\\begin{math}"            { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
<DEFINED>"\\["                      { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
<DEFINED>"\\begin{displaymath}"     { return EnterLaTeXMathModeLexemeAction(MATHMODE); }
<DEFINED>"\\begin{equation}"        { return EnterLaTeXMathModeLexemeAction(MATHMODE); }

<MATHMODE>"\\propogate{"            { return EnterPropogateLexemeAction(PROPOGATE); }

<PROPOGATE>"\\wedge"                { return BinaryOperatorLexemeAction(AND); }
<PROPOGATE>"\\vee"                  { return BinaryOperatorLexemeAction(OR); }
<PROPOGATE>"\\rightarrow"           { return BinaryOperatorLexemeAction(IMPLY); }

<PROPOGATE>"\\neg"                  { return UnaryOperatorLexemeAction(NEG); }

<PROPOGATE>"="                      { return EqualsLexemeAction(); }

<PROPOGATE>[a-z]+                   { return FormulaLexemeAction(VAR_NAME); }

<PROPOGATE>"\\alpha"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\beta"                 { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\chi"                  { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\delta"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\epsilon"              { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\eta"                  { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\gamma"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\iota"                 { return FormulaLexemeAction(FORM_NAME); }

<PROPOGATE>"\\kappa"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\lambda"               { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\mu"                   { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\nu"                   { return FormulaLexemeAction(FORM_NAME); }

<PROPOGATE>"\\omega"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\phi"                  { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\pi"                   { return FormulaLexemeAction(FORM_NAME); }

<PROPOGATE>"\\psi"                  { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\rho"                  { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\sigma"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\tau"                  { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\theta"                { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\upsilon"              { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\xi"                   { return FormulaLexemeAction(FORM_NAME); }
<PROPOGATE>"\\zeta"                 { return FormulaLexemeAction(FORM_NAME); }

<PROPOGATE>"TRUE"                   { return TrueLexemeAction(); }
<PROPOGATE>"FALSE"                  { return FalseLexemeAction(); }

<PROPOGATE>"("						{ return ParenthesisLexemeAction(OPEN_PARENTHESIS); }
<PROPOGATE>")"                      { return ParenthesisLexemeAction(CLOSE_PARENTHESIS); }

<PROPOGATE>","                      { return SeparatorLexemeAction(); }
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

%{
    // Ignore everything else in Math mode
%}
<MATHMODE>[^(?!(?:\\\)|\$|\\end\{math\}|\\\]|\\end\{displaymath\}|\\end\{equation\})$).*$] { return IgnoredLexemeAction(); }

<MATHMODE>.                         { return IgnoredLexemeAction(); }

<DEFINED>"\\end{document}"          { return LeaveCommandDefinedLexemeAction(); }

%{
    // Ignore everything else in the document    
%}

<DEFINED>[^(?!(?:\\\(|\$|\\begin\{math\}|\\\[|\\begin\{displaymath\}|\\begin\{equation\}|\\end\{document\})$).*$] { return IgnoredLexemeAction(); }

<DEFINED>.                          { return IgnoredLexemeAction(); }

%{
    // Ignore everything in the document if the command was not defined
%}
[^(?!\\newcommand\{\\propogate\}\[1\]\{#1\}$).*$] { return IgnoredLexemeAction(); }

.									{ return IgnoredLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
