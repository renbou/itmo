#ifndef FIELD
#   error You need to define FIELD macro
#else
FIELD(symbol, 2, 1)
FIELD(last_mkt, 7, 128)
FIELD(fee_code, 8, 1)
#undef FIELD
#endif