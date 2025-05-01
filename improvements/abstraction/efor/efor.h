#ifndef EFOR_UTILS_H
#define EFOR_UTILS_H

#define efor(VAR, START, COUNT) \
for (int VAR = (START), _end_##VAR = (START) + (COUNT); VAR < _end_##VAR; ++VAR)

#endif // EFOR_UTILS_H
