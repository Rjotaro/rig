#include <time.h>
#include <stdlib.h>
#include "pnggen.h"

int main(void) {
    srand(time(NULL));
    
    generate_png();
}
