#if !defined(FIELD) || !defined(VAR_FIELD)
#  error You need to define FIELD and VAR_FIELD macro
#else

VAR_FIELD(account, 16)
FIELD(capacity, char, char)
VAR_FIELD(cl_ord_id, 20)
VAR_FIELD(fee_code, 2)
VAR_FIELD(last_mkt, 4)
FIELD(max_floor, binary4, unsigned)
FIELD(order_qty, binary4, unsigned)
FIELD(ord_type, char, char)
FIELD(price, price, double)
FIELD(side, char, char)
VAR_FIELD(symbol, 8)
FIELD(time_in_force, char, char)

#undef FIELD
#undef VAR_FIELD

#endif
