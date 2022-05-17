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

const mvr = "mvr";
const mvl = "mvl";
const rm = "rm";
const pd = "pd";
const fl = "fl";
const inc = "inc";
const zero = "0";
const one = "1";
const plus = "+";
const tm: TuringMachine<
  "mvr" | "mvl" | "fl" | "inc" | "rm'" | "rm" | "pd",
  "0" | "1" | "+"
> = {
  nodes: {
    // start
    s: {
      /* Switch to "move right" */
      [zero]: [mvr, zero, stay],
      [one]: [mvr, one, stay],
    },
    // move right
    mvr: {
      /* Find the blank after the last char of the expression */
      [zero]: [mvr, zero, right],
      [one]: [mvr, one, right],
      [plus]: [mvr, plus, right],
      [blank]: [pd, blank, left],
    },
    // pick digit
    pd: {
      /* If there are no digits left in the right number, then
         swap the plus to a blank and move to the leftmost position */
      [plus]: ["rm'", plus, right],
      /* Find the first "1", transform "10..0" into "01..1" (decrement by 1),
           increment the left number by the 1 we subtracted */
      [zero]: [pd, one, left],
      [one]: [fl, zero, left],
    },
    // find left number
    fl: {
      [zero]: [fl, zero, left],
      [one]: [fl, one, left],
      [plus]: [inc, plus, left],
    },
    // increment left number by 1
    inc: {
      [zero]: [mvr, one, stay],
      [blank]: [mvr, one, stay],
      [one]: [inc, zero, left],
    },
    // prepare for removal
    "rm'": {
      /* In the last step all of the right digits were zeros,
           which were then swapped out for ones. */
      [one]: ["rm'", one, right],
      [blank]: [rm, blank, left],
    },
    // remove right part
    rm: {
      [one]: [rm, blank, left],
      [plus]: [mvl, blank, left],
    },
    // move to the start of the answer
    mvl: {
      [zero]: [mvl, zero, left],
      [one]: [mvl, one, left],
      [blank]: [accept, blank, right],
    },
  },
};

transform(tm, path.join(__dirname, "aplusb.out"));
