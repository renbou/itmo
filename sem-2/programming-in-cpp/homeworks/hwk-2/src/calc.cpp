#include "calc.h"

#include <cctype>     // for std::isspace
#include <cmath>      // various math functions
#include <functional> // for beautiful functions
#include <iostream>   // for error reporting via std::cerr

namespace {

const std::size_t max_decimal_digits = 10;

enum class Op
{
    ERR,
    SET,
    ADD,
    REDUCE_ADD,
    SUB,
    REDUCE_SUB,
    MUL,
    REDUCE_MUL,
    DIV,
    REDUCE_DIV,
    REM,
    REDUCE_REM,
    NEG,
    POW,
    REDUCE_POW,
    SQRT
};

enum class Arity
{
    ERROR,
    UNARY,
    BINARY,
    NARY
};

Arity arity(const Op op)
{
    switch (op) {
        // error
    case Op::ERR:
        return Arity::ERROR;
        // unary
    case Op::NEG:
        return Arity::UNARY;
    case Op::SQRT:
        return Arity::UNARY;
        // binary
    case Op::SET:
        return Arity::BINARY;
    case Op::ADD:
        return Arity::BINARY;
    case Op::SUB:
        return Arity::BINARY;
    case Op::MUL:
        return Arity::BINARY;
    case Op::DIV:
        return Arity::BINARY;
    case Op::REM:
        return Arity::BINARY;
    case Op::POW:
        return Arity::BINARY;
        // reduction
    case Op::REDUCE_ADD:
        return Arity::NARY;
    case Op::REDUCE_SUB:
        return Arity::NARY;
    case Op::REDUCE_MUL:
        return Arity::NARY;
    case Op::REDUCE_DIV:
        return Arity::NARY;
    case Op::REDUCE_REM:
        return Arity::NARY;
    case Op::REDUCE_POW:
        return Arity::NARY;
    }
    return Arity::ERROR;
}

Op parse_op(const std::string & line, std::size_t & i)
{
    const auto rollback = [&i, &line](const std::size_t n) {
        i -= n;
        std::cerr << "Unknown operation " << line << std::endl;
        return Op::ERR;
    };
    switch (line[i++]) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        --i; // a first digit is a part of op's argument
        return Op::SET;
    case '+':
        return Op::ADD;
    case '-':
        return Op::SUB;
    case '*':
        return Op::MUL;
    case '/':
        return Op::DIV;
    case '%':
        return Op::REM;
    case '_':
        return Op::NEG;
    case '^':
        return Op::POW;
    case 'S':
        switch (line[i++]) {
        case 'Q':
            switch (line[i++]) {
            case 'R':
                switch (line[i++]) {
                case 'T':
                    return Op::SQRT;
                default:
                    return rollback(4);
                }
            default:
                return rollback(3);
            }
        default:
            return rollback(2);
        }
    case '(': {
        const auto reduce_format_check = [&i, &line, &rollback](const Op operation) {
            if (line[i++] == ')') {
                return operation;
            }
            return rollback(3);
        };
        switch (line[i++]) {
        case '+':
            return reduce_format_check(Op::REDUCE_ADD);
        case '-':
            return reduce_format_check(Op::REDUCE_SUB);
        case '*':
            return reduce_format_check(Op::REDUCE_MUL);
        case '/':
            return reduce_format_check(Op::REDUCE_DIV);
        case '%':
            return reduce_format_check(Op::REDUCE_REM);
        case '^':
            return reduce_format_check(Op::REDUCE_POW);
        default:
            return rollback(2);
        }
    }
    default:
        return rollback(1);
    }
}

std::size_t handle_ws(const bool skip, const std::string & line, std::size_t i)
{
    while (i < line.size() && (static_cast<bool>(std::isspace(line[i])) == skip)) {
        ++i;
    }
    return i;
}

const auto skip_ws = std::bind(handle_ws, true, std::placeholders::_1, std::placeholders::_2);
const auto next_ws = std::bind(handle_ws, false, std::placeholders::_1, std::placeholders::_2);

// Return result and value representing the correctness of the parsed argument,
// since previously we could print error and return a value which might then be used
std::pair<double, bool> parse_arg(const std::string & line, std::size_t & i, const bool last_arg)
{
    double res = 0;
    std::size_t count = 0;
    bool good = true;
    bool integer = true;
    double fraction = 1;
    // If we are parsing the last argument (e.g. for binary op) then we don't want any leftovers
    // However for folding we don't care for ws, only if there are actual wrong chars in args
    const std::size_t end = last_arg ? line.size() : next_ws(line, i);
    while (good && i < end && count < max_decimal_digits) {
        switch (line[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (integer) {
                res *= 10;
                res += line[i] - '0';
            }
            else {
                fraction /= 10;
                res += (line[i] - '0') * fraction;
            }
            ++i;
            ++count;
            break;
        case '.':
            integer = false;
            ++i;
            break;
        default:
            good = false;
            break;
        }
    }
    return {res, good};
}

// We do this multiple times with different error messages for folding and binary ops, so makes sense
// to create a specific function and then use this with just one if case instead of multiple ones
std::pair<double, bool> parse_arg_with_errors(const std::string & line, std::size_t & i, const bool last_arg, const bool required, const std::string & no_arg_err)
{
    i = skip_ws(line, i);
    const auto old_i = i;
    const auto [arg, valid_arg] = parse_arg(line, i, last_arg);
    bool good = true;
    if (!valid_arg) {
        std::cerr << "Argument parsing error at " << i << ": '" << line.substr(i) << "'" << std::endl;
        good = false;
    }
    else if (i < next_ws(line, i)) {
        // no bad symbols but stopped before next ws -> invalid int
        std::cerr << "Argument isn't fully parsed, suffix left: '" << line.substr(i) << "'" << std::endl;
        good = false;
    }

    // if the arg isn't required and we've reached eol then no point in printing error
    if (old_i == i) {
        if (required) {
            // this arg is required and we haven't reached eol
            std::cerr << no_arg_err << std::endl;
        }
        good = false;
    }

    return {arg, good};
}

const auto parse_binary_arg = std::bind(parse_arg_with_errors, std::placeholders::_1, std::placeholders::_2, true, true, std::placeholders::_3);
const auto parse_fold_first_arg = std::bind(parse_arg_with_errors, std::placeholders::_1, std::placeholders::_2, false, true, std::placeholders::_3);
const auto parse_fold_arg = std::bind(parse_arg_with_errors, std::placeholders::_1, std::placeholders::_2, false, false, "");

double unary(const double current, const Op op)
{
    switch (op) {
    case Op::NEG:
        return -current;
    case Op::SQRT:
        if (current > 0) {
            return std::sqrt(current);
        }
        else {
            std::cerr << "Bad argument for SQRT: " << current << std::endl;
            [[fallthrough]];
        }
    default:
        return current;
    }
}

// Return value and validity of value for use in complex cases
std::pair<double, bool> binary(const Op op, const double left, const double right)
{
    switch (op) {
    case Op::SET:
        return {right, true};
    case Op::REDUCE_ADD: [[fallthrough]];
    case Op::ADD:
        return {left + right, true};
    case Op::REDUCE_SUB: [[fallthrough]];
    case Op::SUB:
        return {left - right, true};
    case Op::REDUCE_MUL: [[fallthrough]];
    case Op::MUL:
        return {left * right, true};
    case Op::REDUCE_DIV: [[fallthrough]];
    case Op::DIV:
        if (right != 0) {
            return {left / right, true};
        }
        else {
            std::cerr << "Bad right argument for division: " << right << std::endl;
            return {left, false};
        }
    case Op::REDUCE_REM: [[fallthrough]];
    case Op::REM:
        if (right != 0) {
            return {std::fmod(left, right), true};
        }
        else {
            std::cerr << "Bad right argument for remainder: " << right << std::endl;
            return {left, false};
        }
    case Op::REDUCE_POW: [[fallthrough]];
    case Op::POW:
        return {std::pow(left, right), true};
    default:
        return {left, true};
    }
}

} // anonymous namespace

double process_line(const double current, const std::string & line)
{
    std::size_t i = 0;
    const auto op = parse_op(line, i);
    switch (arity(op)) {
    case Arity::NARY: {
        // By definition our NARY operation takes >0 arguments,
        // reducing them with the equivalent binary operation
        auto [arg, good] = parse_fold_first_arg(line, i, "Reduction of binary operation requires at least one argument");
        double result = current;
        bool good_op = true;
        while (good && good_op) {
            // the argument might be valid, but the operation might fail, in which case we fail
            std::tie(result, good_op) = binary(op, result, arg);
            if (good_op) {
                std::tie(arg, good) = parse_fold_arg(line, i);
            }
        }
        // we couldn't make it to the end or the op failed -> invalid args
        if (i < line.size() || !good_op) {
            return current;
        }
        return result;
    }
    case Arity::BINARY: {
        const auto [arg, good] = parse_binary_arg(line, i, "No argument for a binary operation");
        if (!good) {
            break;
        }
        return binary(op, current, arg).first;
    }
    case Arity::UNARY: {
        if (i < line.size()) {
            std::cerr << "Unexpected suffix for a unary operation: '" << line.substr(i) << "'" << std::endl;
            break;
        }
        return unary(current, op);
    }
    default:
        break;
    }
    return current;
}
