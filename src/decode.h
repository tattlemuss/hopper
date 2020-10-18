#ifndef DECODE_H
#define DECODE_H

class buffer_reader;
class instruction;

extern int decode(buffer_reader& buffer, instruction& inst);

#endif
