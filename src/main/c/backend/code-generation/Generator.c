#include "Generator.h"

/* MODULE INTERNAL STATE */
static Logger * _logger = NULL;
static FILE * _outputFile = NULL; // Pointer to output file
static int _nodeCounter = 0;

/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
		if (_outputFile != NULL) {
			fclose(_outputFile);
			_outputFile = NULL;
    	}
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/* LAYOUT STRUCTS */
typedef struct {
    char id[64];
    double x;
    double y;
} NodeInfo;

/** PRIVATE FUNCTIONS */
static void _generateContent(Content * content);
static void _emit(const char * format, ...);
static double _max(double a, double b);
static void _nextID(char * buffer);
static NodeInfo _drawFormula(Formula * formula, double * y_cursor);
static void _generateCircuit(Propogate * propogate);
static void _generateMath(Math * math);
static void _generateElement(Element * element);
static void _generateContent(Content * content);




/** PUBLIC FUNCTIONS */
void executeGenerator(CompilerState * compilerState) {
    logDebugging(_logger, "Generating final output...");
    
    Program * program = (Program *)compilerState->abstractSyntaxtTree;
    
    if (program != NULL && program->content != NULL) {
        _generateContent(program->content);
    }
    
    logDebugging(_logger, "Generation done.");
}