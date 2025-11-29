#include "Generator.h"

/* MODULE INTERNAL STATE */
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/** PRIVATE FUNCTIONS */
static void _generateContent(Content * content);
/*


/** PUBLIC FUNCTIONS */
void executeGenerator(CompilerState * compilerState) {
    logDebugging(_logger, "Generating final output...");
    
    Program * program = (Program *)compilerState->abstractSyntaxtTree;
    
    if (program != NULL && program->content != NULL) {
        _generateContent(program->content);
    }
    
    logDebugging(_logger, "Generation done.");
}