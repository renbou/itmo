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

const sm = "sm";
const sm0 = "sm0";
const sm1 = "sm1";
const s0 = "s0";
const s1 = "s1";
const marker = "#";
const zero = "0";
const one = "1";
const tm: TuringMachine<
  "sm" | "sm'" | "sm0" | "sm1" | "s0" | "s1" | "fs",
  "0" | "1" | "#"
> = {
  nodes: {
    [start]: {
      /* Place marker at the end of the string */
      [zero]: [start, zero, right],
      [one]: [start, one, right],
      [blank]: [sm, marker, stay],
    },
    // shift marker
    sm: {
      [zero]: [sm, zero, left],
      [one]: [sm, one, left],
      [marker]: ["sm'", blank, left],
    },
    "sm'": {
      /* Swap marker and next char */
      [zero]: [sm0, marker, right],
      [one]: [sm1, marker, right],
      /* If no more chars are left, then we're done */
      [blank]: ["fs", blank, right],
    },
    // set a zero instead of previous marker
    sm0: {
      [blank]: [s0, zero, right],
    },
    // set a one instead of previous marker
    sm1: {
      [blank]: [s1, one, right],
    },
    // set zero as the last char of mirrored string
    s0: {
      [zero]: [s0, zero, right],
      [one]: [s0, one, right],
      [blank]: [sm, zero, left],
    },
    // set one as the last char of mirrored string
    s1: {
      [zero]: [s1, zero, right],
      [one]: [s1, one, right],
      [blank]: [sm, one, left],
    },
    // single final step right
    fs: {
      [blank]: [accept, blank, right],
    },
  },
};

transform(tm, path.join(__dirname, "mirror.out"));
