#! /bin/bash

set -u

BASE_PATH="$(dirname "$0")/../../.."
cd "$BASE_PATH"

# Crear carpeta para los resultados si no existe
mkdir -p src/test/results

GREEN='\033[0;32m'
RED='\033[0;31m'
OFF='\033[0m'
STATUS=0

echo "Compiler should accept..."
echo ""

for test in $(ls src/test/c/accept/); do
    # Definimos el nombre del archivo de salida específico para este test
    OUTPUT_FILE="src/test/results/$test.tex"
    
    # Pasamos OUTPUT_FILE como argumento al compilador
    cat "src/test/c/accept/$test" | ".build/Flex-Bison-Compiler" "$OUTPUT_FILE" >/dev/null 2>&1
    
    RESULT="$?"
    if [ "$RESULT" == "0" ]; then
        echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT) -> generated $OUTPUT_FILE"
    else
        STATUS=1
        echo -e "    $test, ${RED}but it rejects${OFF} (status $RESULT)"
    fi
done

echo "Compiler should reject..."
echo ""

for test in $(ls src/test/c/reject/); do
	cat "src/test/c/reject/$test" | ".build/Flex-Bison-Compiler" >/dev/null 2>&1
	RESULT="$?"
	if [ "$RESULT" != "0" ]; then
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
	else
		STATUS=1
		echo -e "    $test, ${RED}but it accepts${OFF} (status $RESULT)"
	fi
done
echo ""

echo "All done."
exit $STATUS
