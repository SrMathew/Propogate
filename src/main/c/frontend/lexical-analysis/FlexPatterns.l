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

/* PUBLIC FUNCTIONS */
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();

/* LATEX FUNCTIONS */
extern CompilationStatus EnterEnvironmentLexemeAction(FlexContext context);
extern CompilationStatus LeaveEnvironmentLexemeAction();
extern CompilationStatus EnterMathmodeLexemeAction(FlexContext context, TokenLabel label);
extern CompilationStatus LeaveMathmodeLexemeAction(TokenLabel label);
extern CompilationStatus MathmodeEnvironmentLexemeAction();
extern CompilationStatus BracketLexemeAction(TokenLabel label);
extern CompilationStatus EnterCommentLexemeAction(FlexContext context);
extern CompilationStatus LeaveCommentLexemeAction();
extern CompilationStatus TextLexemeAction();
extern CompilationStatus UnknownLexemeAction();

/* PROPOGATE FUNCTIONS */
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

%x ENVIRONMENT_DEF
%x MATHMODE
%x COMMENT
%x PROPOGATE

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
variable            [a-z]+
formula             \\[alpha|beta|chi|delta|epsilon|eta|gamma|iota|kappa|lambda|mu|nu|omega|phi|pi|psi|rho|sigma|tau|theta|upsilon|xi|zeta]+
%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

"[!propogate]"                      { return EnterPropogateLexemeAction(PROPOGATE); }

<PROPOGATE>"\\wedge"                { return BinaryOperatorLexemeAction(AND); }
<PROPOGATE>"\\vee"                  { return BinaryOperatorLexemeAction(OR); }
<PROPOGATE>"\\rightarrow"           { return BinaryOperatorLexemeAction(IMPLY); }

<PROPOGATE>"\\neg"                  { return UnaryOperatorLexemeAction(NEG); }

<PROPOGATE>"="                      { return EqualsLexemeAction(); }

<PROPOGATE>{variable}               { return FormulaLexemeAction(VAR_NAME); }

<PROPOGATE>{formula}                { return FormulaLexemeAction(FORM_NAME); }

<PROPOGATE>"TRUE"                   { return TrueLexemeAction(); }
<PROPOGATE>"FALSE"                  { return FalseLexemeAction(); }

<PROPOGATE>"("						{ return BracketLexemeAction(OPEN_PARENTHESIS); }
<PROPOGATE>")"                      { return BracketLexemeAction(CLOSE_PARENTHESIS); }

<PROPOGATE>","                      { return SeparatorLexemeAction(); }
<PROPOGATE>"\n"                     { return IgnoredLexemeAction(); }

<PROPOGATE>"{"                      { return BracketLexemeAction(OPEN_BRACE); }
<PROPOGATE>"}"                      { return LeavePropogateLexemeAction(); }

<PROPOGATE>{whitespace}             { return IgnoredLexemeAction(); }
<PROPOGATE>.                        { return UnknownLexemeAction(); }

"\\begin"                           { return EnterEnvironmentLexemeAction(ENVIRONMENT_DEF); }
"\\end"                             { return LeaveEnvironmentLexemeAction(); }
<ENVIRONMENT_DEF>"math"             { return MathmodeEnvironmentLexemeAction(); }
<ENVIRONMENT_DEF>"displaymath"      { return MathmodeEnvironmentLexemeAction(); }
<ENVIRONMENT_DEF>"equation"         { return MathmodeEnvironmentLexemeAction(); }

"\\("                               { return EnterMathmodeLexemeAction(MATHMODE, MM_OPEN_PARENTHESIS); }
"\\["                               { return EnterMathmodeLexemeAction(MATHMODE, MM_OPEN_BRACKET); }
"$"                                 { return EnterMathmodeLexemeAction(MATHMODE, MM_OPEN_DOLLAR); }
"$$"                                { return EnterMathmodeLexemeAction(MATHMODE, MM_OPEN_DOUBLEDOLLAR); }
<MATHMODE>"\\)"                     { return LeaveMathmodeLexemeAction(MM_CLOSE_PARENTHESIS); }
<MATHMODE>"\\]"                     { return LeaveMathmodeLexemeAction(MM_CLOSE_BRACKET); }
<MATHMODE>"$"                       { return LeaveMathmodeLexemeAction(MM_CLOSE_DOLLAR); }
<MATHMODE>"$$"                      { return LeaveMathmodeLexemeAction(MM_CLOSE_DOUBLEDOLLAR); }

"{"                                 { return BracketLexemeAction(OPEN_BRACE); }
"}"                                 { return BracketLexemeAction(CLOSE_BRACE); }

"["                                 { return BracketLexemeAction(OPEN_BRACKET); }
"]"                                 { return BracketLexemeAction(CLOSE_BRACKET); }

"%"                                 { return EnterCommentLexemeAction(COMMENT); }
<COMMENT>"%"                        { return LeaveCommentLexemeAction(); }
<COMMENT>.                          { return IgnoredLexemeAction(); }

[^\\{}#\$%\^_~\[\]\n]+              { return TextLexemeAction(); }

.									{ return IgnoredLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
