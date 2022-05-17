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
const fa = "fa";
const s0 = "s0";
const s1 = "s1";
const zero = "0";
const one = "1";
const marker = "#";
const fakenum = "2";
const tm: TuringMachine<
  "setfn" | "sm" | "sm'" | "fa" | "s0" | "s1" | "s0'" | "s1'",
  "0" | "1" | "#" | "2"
> = {
  nodes: {
    [start]: {
      /* Place marker at the end of the string */
      [zero]: [start, zero, right],
      [one]: [start, one, right],
      [blank]: ["setfn", marker, right],
    },
    // set fake number to be replaced by first char
    setfn: {
      [blank]: [sm, fakenum, left],
    },
    // shift marker
    sm: {
      [zero]: [sm, zero, left],
      [one]: [sm, one, left],
      [blank]: [sm, blank, left],
      [marker]: ["sm'", blank, left],
    },
    "sm'": {
      /* Swap marker and next char */
      [zero]: [s0, marker, right],
      [one]: [s1, marker, right],
      /* If no more chars are left, then we're done */
      [blank]: [fa, blank, right],
    },
    // set zero as last char of reversed string
    s0: {
      [blank]: [s0, blank, right],
      [zero]: ["s0'", zero, right],
      [one]: ["s0'", one, right],
      [fakenum]: [sm, zero, left],
    },
    "s0'": {
      [zero]: ["s0'", zero, right],
      [one]: ["s0'", one, right],
      [blank]: [sm, zero, left],
    },
    s1: {
      [blank]: [s1, blank, right],
      [zero]: ["s1'", zero, right],
      [one]: ["s1'", one, right],
      [fakenum]: [sm, one, left],
    },
    "s1'": {
      [zero]: ["s1'", zero, right],
      [one]: ["s1'", one, right],
      [blank]: [sm, one, left],
    },
    // find answer
    fa: {
      [blank]: [fa, blank, right],
      [zero]: [accept, zero, stay],
      [one]: [accept, one, stay],
    },
  },
};

transform(tm, path.join(__dirname, "reverse.out"));
