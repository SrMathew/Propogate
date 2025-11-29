#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static bool _logIgnoredLexemes = true;
static InputBuffer *_inputBuffer = NULL;
static LexicalAnalyzer *_lexicalAnalyzer = NULL;
static Logger *_logger = NULL;

/** Shutdown module's internal state. */
void _shutdownFlexActionsModule()
{
	if (_logger != NULL)
	{
		logDebugging(_logger, "Destroying module: FlexActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	if (_inputBuffer != NULL)
	{
		destroyInputBuffer(_inputBuffer);
		_inputBuffer = NULL;
	}
	_lexicalAnalyzer = NULL;
}

ModuleDestructor initializeFlexActionsModule(LexicalAnalyzer *lexicalAnalyzer)
{
	_inputBuffer = NULL;
	_lexicalAnalyzer = lexicalAnalyzer;
	_logger = createLogger("FlexActions");
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	return _shutdownFlexActionsModule;
}

/* PRIVATE FUNCTIONS */

static void _logTokenAction(const char *actionName, Token *token);

/**
 * Logs a lexical-analyzer action over a token in DEBUGGING level.
 */
static void _logTokenAction(const char *actionName, Token *token)
{
	char *_lexeme = escape(token->lexeme);
	logDebugging(_logger, WARNING_COLOR "%s" DEFAULT_COLOR ": Token(context=%d, label=%d, length=%d, lexeme=%s\"%s\"%s, line=%d, semanticValue=%p)",
				 actionName,
				 token->context,
				 token->label,
				 token->length,
				 INFORMATION_COLOR, _lexeme, DEFAULT_COLOR,
				 token->line,
				 token->semanticValue);
	free(_lexeme);
	_lexeme = NULL;
}

/* PUBLIC FUNCTIONS */

CompilationStatus EOFLexemeAction()
{
	CompilationStatus status = IN_PROGRESS;
	Token *token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	if (!popInputBuffer(_lexicalAnalyzer))
	{
		status = pushToken(_lexicalAnalyzer, token);
		FlexContext context = currentLexicalAnalyzerContext(_lexicalAnalyzer);
		if (0 < context)
		{
			logError(_logger, "The final context is not closed (context=%d).", context);
			status = FAILED;
		}
	}
	destroyToken(token);
	return status;
}

CompilationStatus IgnoredLexemeAction()
{
	if (_logIgnoredLexemes)
	{
		Token *token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

/* LATEX FUNCTIONS */

/*
CompilationStatus EnterEnvironmentLexemeAction(FlexContext context)
{
	if (_logIgnoredLexemes)
	{
		Token *token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus LeaveEnvironmentLexemeAction()
{
	if (_logIgnoredLexemes)
	{
		Token *token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

// Necesito un contexto Mathmode porque $ y $$ abren y cierran el mathmode indistinguiblemente
CompilationStatus EnterMathmodeLexemeAction(FlexContext context, TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return status;
}

CompilationStatus LeaveMathmodeLexemeAction(TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return status;
}

CompilationStatus MathmodeEnvironmentLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, MM_ENVIRONMENT);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}
*/

CompilationStatus MathmodeEnvironmentLexemeAction(TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus BracketLexemeAction(TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus EnterCommentLexemeAction(FlexContext context)
{
	if (_logIgnoredLexemes)
	{
		Token *token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus LeaveCommentLexemeAction()
{
	if (_logIgnoredLexemes)
	{
		Token *token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

CompilationStatus TextLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, TEXT);
	token->semanticValue->string = strdup(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus UnknownLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	destroyToken(token);
	return FAILED;
}

/* PROPOGATE FUNCTIONS */

CompilationStatus EnterPropogateLexemeAction(FlexContext context)
{
	Token *token = createToken(_lexicalAnalyzer, PROPOGATE_COMMAND);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return status;
}

CompilationStatus LeavePropogateLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, CLOSE_BRACE);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return status;
}

CompilationStatus SeparatorLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, SEPARATOR);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus EqualsLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, EQUALS);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus BinaryOperatorLexemeAction(TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus UnaryOperatorLexemeAction(TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

extern CompilationStatus FormulaLexemeAction(TokenLabel label)
{
	Token *token = createToken(_lexicalAnalyzer, label);
	token->semanticValue->string = strdup(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

extern CompilationStatus TrueLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, VALUE);
	token->semanticValue->value = true;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

extern CompilationStatus FalseLexemeAction()
{
	Token *token = createToken(_lexicalAnalyzer, VALUE);
	token->semanticValue->value = false;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}