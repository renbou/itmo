import * as path from "path";
import type { TuringMachine, Direction } from "../turing-machine";
import {
  left,
  stay,
  right,
  start,
  accept,
  reject,
  blank,
} from "../turing-machine";
import transform from "../turing-machine";

const nc = "nc";
const c0 = "c0";
const c1 = "c1";
const v = "v";
const lt = "lt";
const gt = "gt";
const eq = "eq";
const zero = "0";
const one = "1";
const tm: TuringMachine<
  "nc" | "c0" | "c0'" | "c1" | "c1'" | "v" | "v'",
  "0" | "1" | "lt" | "gt" | "eq" | "<"
> = {
  nodes: {
    [start]: {
      [zero]: [start, zero, right],
      [one]: [start, one, right],
      "<": [start, "<", right],
      [lt]: [start, lt, right],
      [eq]: [start, eq, right],
      [gt]: [start, gt, right],
      [blank]: [nc, blank, left],
    },
    // next compare
    [nc]: {
      /* Last valid char is "<" => we compared all digits
         and now need to validate the results */
      "<": [v, blank, left],
      [zero]: [c0, blank, left],
      [one]: [c1, blank, left],
    },
    // compare next left side digit with 0
    [c0]: {
      [zero]: [c0, zero, left],
      [one]: [c0, one, left],
      "<": ["c0'", "<", left],
    },
    "c0'": {
      /* Skip previous comparisons, and set the comparison result */
      [lt]: ["c0'", lt, left],
      [eq]: ["c0'", eq, left],
      [gt]: ["c0'", gt, left],
      [zero]: [start, eq, right],
      [one]: [start, gt, right],
      /* Left side is smaller than the right side, which means that it is smaller
         (since both numbers are guaranteed not to have leading zeros) */
      [blank]: [accept, blank, stay],
    },
    // compare next left side digit with 1
    [c1]: {
      [zero]: [c1, zero, left],
      [one]: [c1, one, left],
      "<": ["c1'", "<", left],
    },
    "c1'": {
      /* Skip previous comparisons, and set the comparison result */
      [lt]: ["c1'", lt, left],
      [eq]: ["c1'", eq, left],
      [gt]: ["c1'", gt, left],
      [zero]: [start, lt, right],
      [one]: [start, eq, right],
      /* Left side is smaller than the right side, which means that it is smaller
         (since both numbers are guaranteed not to have leading zeros) */
      [blank]: [accept, blank, stay],
    },
    // validate
    v: {
      /* Find the leftmost comparison result and begin validation */
      [lt]: [v, lt, left],
      [eq]: [v, eq, left],
      [gt]: [v, gt, left],
      [blank]: ["v'", blank, right],
      /* If leading digits are left, then the left number must be greater,
         since both numbers are guaranteed not to have leading zeros. */
      [zero]: [reject, zero, stay],
      [one]: [reject, one, stay],
    },
    "v'": {
      [blank]: [reject, blank, stay],
      [lt]: [accept, lt, stay],
      [eq]: ["v'", eq, right],
      [gt]: [reject, gt, stay],
    },
  },
};

transform(tm, path.join(__dirname, "less.out"));
