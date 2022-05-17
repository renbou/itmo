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

const pm = "pm";
const mvr = "mvr";
const dec = "dec";
const inc = "inc";
const rm = "rm";
const fa = "fa";
const zero = "0";
const one = "1";
const two = "2";
const marker = "#";
const tm: TuringMachine<
  "pm" | "mvr" | "dec" | "inc" | "inc'" | "rm" | "fa",
  "0" | "1" | "2" | "#"
> = {
  nodes: {
    [start]: {
      /* No leading zeros means that a number which starts with zero is zero. */
      [zero]: [accept, zero, stay],
      [one]: [pm, one, left],
      [two]: [pm, two, left],
    },
    // place marker
    [pm]: {
      [blank]: [mvr, marker, right],
    },
    // move to the rightmost position
    [mvr]: {
      [zero]: [mvr, zero, right],
      [one]: [mvr, one, right],
      [two]: [mvr, two, right],
      [marker]: [mvr, marker, right],
      [blank]: [dec, blank, left],
    },
    // decrement ternary number
    [dec]: {
      [marker]: [rm, marker, right],
      [zero]: [dec, two, left],
      [one]: [inc, zero, left],
      [two]: [inc, one, left],
    },
    // remove ternary number
    [rm]: {
      [zero]: [rm, marker, right],
      [one]: [rm, marker, right],
      [two]: [rm, marker, right],
      [blank]: [fa, blank, left],
    },
    [fa]: {
      [marker]: [fa, blank, left],
      [zero]: [fa, zero, left],
      [one]: [fa, one, left],
      [blank]: [accept, blank, right],
    },
    // increment binary number
    [inc]: {
      [zero]: [inc, zero, left],
      [one]: [inc, one, left],
      [two]: [inc, two, left],
      [marker]: ["inc'", marker, left],
    },
    "inc'": {
      [zero]: [mvr, one, right],
      [blank]: [mvr, one, right],
      [one]: ["inc'", zero, left],
    },
  },
};

transform(tm, path.join(__dirname, "convertto2.out"));
