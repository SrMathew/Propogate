#include "Generator.h"

/* MODULE INTERNAL STATE */
static Logger * _logger = NULL;
static FILE * _outputFile = NULL; // Pointer to output file
static int _nodeCounter = 0;
static bool _isStandaloneMode = false; // Flag for latex wrapper

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
	bool isGate;
} NodeInfo;

/** PRIVATE FUNCTIONS */
static void _generateContent(Content * content);
static void _emit(const char * format, ...);
static double _max(double a, double b);
static void _nextID(char * buffer);
static bool _hasDocumentHeader(Program * program);
static bool _isWhitespace(const char * s);
static NodeInfo _drawFormula(Formula * formula, double * y_cursor);
static void _generateCircuit(Propogate * propogate);
static void _generateMath(Math * math);
static void _generateElement(Element * element);
static void _generateContent(Content * content);

//print inside file instead of standard output
static void _emit(const char * format, ...) {
    if (_outputFile == NULL) return;
    
    va_list args;
    va_start(args, format);
    vfprintf(_outputFile, format, args);
    va_end(args);
}

static double _max(double a, double b) {
    return (a > b) ? a : b;
}

static void _nextID(char * buffer) {
    sprintf(buffer, "gate_%d", ++_nodeCounter);
}

// scan AST to detect /documentclass
static bool _hasDocumentHeader(Program * program) {
    if (!program || !program->content) return false;
    
    Content * current = program->content;
    while (current != NULL) {
        if (current->element->elementType == TEXT_ONLY && current->element->text) {
            if (current->element->text->text && strstr(current->element->text->text, "\\documentclass")) {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}

static bool _isWhitespace(const char * s) {
    while (*s) {
        if (!isspace((unsigned char)*s)) return false;
        s++;
    }
    return true;
}

// Draw all the logic gates recursively
static NodeInfo _drawFormula(Formula * formula, double * y_cursor) {
    NodeInfo info = {0};
    if (formula == NULL) return info;

    switch (formula->formulaType) {
        case VAR_FORMULA: {
            _nextID(info.id);
            info.x = 0;
            info.y = *y_cursor;
			info.isGate = false;
            *y_cursor += 1.5; 
            char * name = formula->variable->name;
            _emit("    \\node (%s) at (%.2f, %.2f) {%s};\n", info.id, info.x, info.y, name);
            break;
        }
        case UNARY: {
            if (formula->unaryFormulaType == NEG_TYPE) {
                NodeInfo child = _drawFormula(formula->formula, y_cursor);
                _nextID(info.id);
                info.x = child.x + 2.0;
                info.y = child.y;info.isGate = true;
                _emit("    \\node[not port] (%s) at (%.2f, %.2f) {};\n", info.id, info.x, info.y);
                if (child.isGate)
                     _emit("    \\draw (%s.out) -- (%s.in);\n", child.id, info.id);
                else
                     _emit("    \\draw (%s) -- (%s.in);\n", child.id, info.id);
				
            } else {
                return _drawFormula(formula->formula, y_cursor);
            }
            break;
        }
        case BINARY: {
            NodeInfo left = _drawFormula(formula->leftFormula, y_cursor);
            NodeInfo right = _drawFormula(formula->rightFormula, y_cursor);
            
            _nextID(info.id);
            info.x = _max(left.x, right.x) + 2.5;
            info.y = (left.y + right.y) / 2.0;
            
            if (formula->binaryFormulaType == IMPLY_TYPE) {
                // Implies: (A -> B) == (!A || B)
                char not_id[64];
                _nextID(not_id);
                double not_x = left.x + 1.5;
                double not_y = left.y;
                _emit("    \\node[not port] (%s) at (%.2f, %.2f) {};\n", not_id, not_x, not_y);
                if (left.isGate) _emit("    \\draw (%s.out) -- (%s.in);\n", left.id, not_id);
                else             _emit("    \\draw (%s) -- (%s.in);\n", left.id, not_id);
                
                info.x = _max(not_x, right.x) + 2.5;
                _emit("    \\node[or port] (%s) at (%.2f, %.2f) {};\n", info.id, info.x, info.y);
                _emit("    \\draw (%s.out) -- (%s.in 1);\n", not_id, info.id);
                if (right.isGate) _emit("    \\draw (%s.out) |- (%s.in 2);\n", right.id, info.id);
                else              _emit("    \\draw (%s) |- (%s.in 2);\n", right.id, info.id);
            } else {
                char * shape = (formula->binaryFormulaType == AND_TYPE) ? "and port" : "or port";
                _emit("    \\node[%s] (%s) at (%.2f, %.2f) {};\n", shape, info.id, info.x, info.y);

                if (left.isGate) _emit("    \\draw (%s.out) |- (%s.in 1);\n", left.id, info.id);
                else             _emit("    \\draw (%s) |- (%s.in 1);\n", left.id, info.id);

                if (right.isGate) _emit("    \\draw (%s.out) |- (%s.in 2);\n", right.id, info.id);
                else              _emit("    \\draw (%s) |- (%s.in 2);\n", right.id, info.id);
            }
			info.isGate = true;
            break;
        }
    }
    return info;
}

static void _generateCircuit(Propogate * propogate) {
    if (propogate == NULL || propogate->expressionList == NULL) return;

    ExpressionList * current = propogate->expressionList;
    while (current != NULL) {
        if (current->expression == NULL) {
            current = current->next;
            continue;
        }

        // --- CASO 1: Forms (p & q) ---
        if (current->expression->expressionType == FORM_EXPR) {
            _nodeCounter = 0;
            double y_cursor = 0;
            
            _emit("\n%% --- Logic Circuit Generated by Propogate ---\n");
            
            if (!_isStandaloneMode) _emit("\\begin{center}\n");
            _emit("\\begin{circuitikz}[scale=0.8, transform shape]\n");
            
            NodeInfo out = _drawFormula(current->expression->formula, &y_cursor);
            
            if (out.isGate) {
                _emit("    \\draw (%s.out) -- ++(1,0) node[anchor=west] {OUT};\n", out.id);
            } else {
                _emit("    \\draw (%s.east) -- ++(1,0) node[anchor=west] {OUT};\n", out.id);
            }
            
            _emit("\\end{circuitikz}\n");
            if (!_isStandaloneMode) _emit("\\end{center}\n");
        }
        
        // ---  2: Definitions
        else if (current->expression->expressionType == DEF_EXPR) {
            Definition * def = current->expression->definition;
            // Verificación de nulidad y tipo
            if (def != NULL && def->definitionType == VAR_DEF) {
                _emit("\n%% --- Variable Definition ---\n");
                
                if (!_isStandaloneMode) _emit("\\begin{center}\n");
                _emit("\\begin{circuitikz}[scale=0.8, transform shape]\n");
                
                const char * valStr = def->value ? "1" : "0";
                
                // Dibujo: Nodo cuadrado (valor) -> Nodo texto (nombre variable)
                _emit("    \\node[draw, rectangle] (val) at (0,0) {%s};\n", valStr);
                _emit("    \\draw (val.east) -- ++(1,0) node[anchor=west] {%s};\n", def->variable->name);
                
                _emit("\\end{circuitikz}\n");
                if (!_isStandaloneMode) _emit("\\end{center}\n");
            }
			else if(def != NULL && def->definitionType == FORM_DEF)
			{
				_emit("\n%% --- Form Definition ---\n");
				double y_cursor = 0;
                
                if (!_isStandaloneMode) _emit("\\begin{center}\n");
                _emit("\\begin{circuitikz}[scale=0.8, transform shape]\n");
                             
				NodeInfo out = _drawFormula(def->formula, &y_cursor);
				if (out.isGate) {
					_emit("    \\draw (%s.out) -- ++(1,0) node[anchor=west] {OUT};\n", out.id);
				} else {
					_emit("    \\draw (%s.east) -- ++(1,0) node[anchor=west] {OUT};\n", out.id);
				}
                
                _emit("\\end{circuitikz}\n");
                if (!_isStandaloneMode) _emit("\\end{center}\n");
			}
			
        }
        
        current = current->next;
    }
}

/**
 * returns true for \[...\], \(...\), $$...$$. latex delimiters
 */
static bool _hasDelimiters(const char * text) {
    if (text == NULL) return false;
    while (isspace((unsigned char)*text)) text++;
    
    if (strncmp(text, "\\[", 2) == 0) return true;
    if (strncmp(text, "\\(", 2) == 0) return true;
    if (strncmp(text, "$$", 2) == 0) return true;
    
    return false;
}

static void _generateMath(Math * math) {
	bool insideMath = false;

    Math * current = math;
    while (current != NULL) {
        if (current->mathType == PROPOGATE) {
            if (insideMath) {
                _emit("$");
                insideMath = false;
            } 
            if (current->propogate != NULL) {
                _generateCircuit(current->propogate);
            }
        } else {
            if (current->text != NULL && current->text->text != NULL) {
                char * txt = current->text->text;
                
                // whitespaces
                if (_isWhitespace(txt)) {
                    if (!insideMath) _emit("%s", txt);
                    // Si estamos dentro de $, los espacios se ignoran o se manejan dentro.
                } 
                // (\[...\]). prints
                else if (_hasDelimiters(txt)) {
                     if (insideMath) { // Cerramos si estaba abierto
                         _emit("$");
                         insideMath = false;
                     }
                     _emit("%s", txt);
                }
                // math text
                else {
                    if (!insideMath) {
                        _emit("$");
                        insideMath = true;
                    }
                    _emit("%s", txt);
                }
            }
        }
        current = current->next;
    }
    
    // Si terminamos y quedó abierto, cerrar.
    if (insideMath) {
        _emit("$");
    }
}

static void _generateElement(Element * element) {
    if (element == NULL) return;

    switch (element->elementType) {
        case TEXT_ONLY:
            if (element->text != NULL && element->text->text != NULL) {
                char * txt = element->text->text;
                _emit("%s", txt);

				if (!_isStandaloneMode && strstr(txt, "\\documentclass") != NULL) {
                    _emit("\n\\usepackage{circuitikz}\n");
                }

            }
            break;
        case MATH:
            if (element->math != NULL) {
                _generateMath(element->math);
            }
            break;
        case ENVIRONMENT:
            if (element->environment != NULL && element->environment->text != NULL) {
                _emit("\\begin{%s}", element->environment->text);
                if (element->content != NULL) {
                    _generateContent(element->content);
                }
                _emit("\\end{%s}", element->environment->text);
            }
            break;
    }
}

static void _generateContent(Content * content) {
    Content * current = content;
    while (current != NULL) {
        _generateElement(current->element);
        current = current->next;
    }
}


/** PUBLIC FUNCTIONS */
void executeGenerator(CompilerState * compilerState) {
	const char * filename = compilerState->outputFilename ? compilerState->outputFilename : "output.tex";
    
    logDebugging(_logger, "Opening output file: %s", filename);
    
    _outputFile = fopen(filename, "w");
    if (_outputFile == NULL) {
        logError(_logger, "Could not open %s for writing.", filename);
        return;
    }

    logDebugging(_logger, "Generating final output...");
    Program * program = (Program *)compilerState->abstractSyntaxtTree;
    
    if (program != NULL) {
        
        bool hasHeader = _hasDocumentHeader(program);
        _isStandaloneMode = !hasHeader;

        if (_isStandaloneMode) {
            logDebugging(_logger, "No document header detected. Generating standalone wrapper.");
            _emit("\\documentclass[border=10pt, tikz]{standalone}\n");
            _emit("\\usepackage{circuitikz}\n");
            _emit("\\begin{document}\n");
        }

		logDebugging(_logger, "program content is: %s", program->content ? "some" : "null");

        if (program->content != NULL) {
            _generateContent(program->content);
        }

        if (_isStandaloneMode) {
            _emit("\n\\end{document}\n");
        }
    }
    
    fclose(_outputFile);
    _outputFile = NULL;
    logDebugging(_logger, "Generation done. 'output.tex' is ready for pdflatex.");
}