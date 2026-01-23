#ifndef __MATRIXKEY_H
#define __MATRIXKEY_H

#include <stdint.h>

void MatrixKey_Init(void);
void MatrixKey_Update(void);
char MatrixKey_GetValue(void);
char MatrixKey_GetHold(void);

#endif
