#ifndef CALCULATOR_HEADER
#define CALCULATOR_HEADER

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include <limits.h>
#include <stdbool.h>

/** Initialize module's internal state. */
ModuleDestructor initializePropogateModule();

/**
 * The result of a valuation. It's considered valid only if "succeed" is
 * true.
 */
typedef struct {
	bool succeeded;
	int value;
} ValuationResult;

typedef ValuationResult (*BinaryOperator)(const int, const int);

/** Arithmetic operations. */
ValuationResult add(const int leftAddend, const int rightAddend);
ValuationResult divide(const int dividend, const int divisor);
ValuationResult implication(const int multiplicand, const int multiplier);


/**
 * Computes the final value of a mathematical constant.
 *//*
ValuationResult computeConstant(Constant * constant);

/**
 * Computes the final value of a mathematical expression.
 *//*
ValuationResult computeExpression(Expression * expression);

/**
 * Computes the final value of a mathematical factor.
 *//*
ValuationResult computeFactor(Factor * factor);

/**
 * Computes the program value using the current compiler state.
 */
ValuationResult executePropogate(CompilerState * compilerState);

#endif