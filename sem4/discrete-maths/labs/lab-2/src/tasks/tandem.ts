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

const mvl = "mvl";
const flse = "flse";
const frss = "frss";
const mrsn = "mrsn";
const v = "v";
const v0 = "v0";
const v1 = "v1";
const zero = "0";
const leftZero = "'0";
const rightZero = "0'";
const one = "1";
const leftOne = "'1";
const rightOne = "1'";
const tm: TuringMachine<
  "v" | "v0" | "v1" | "mvl" | "flse" | "frss" | "mrsn",
  "0" | "1" | "'0" | "'1" | "0'" | "1'"
> = {
  nodes: {
    [start]: {
      /* Mark next left side number and start moving to the end of the right side */
      [zero]: [frss, leftZero, right],
      [one]: [frss, leftOne, right],
      /* Move to the start of the left side once no more numbers need marking */
      [rightZero]: [mvl, rightZero, left],
      [rightOne]: [mvl, rightOne, left],
    },
    // find right side start
    frss: {
      [zero]: [frss, zero, right],
      [one]: [frss, one, right],
      [rightZero]: [mrsn, rightZero, left],
      [rightOne]: [mrsn, rightOne, left],
      [blank]: [mrsn, blank, left],
    },
    // mark right side number
    mrsn: {
      /* If there are no numbers between marked left and right side, then
         the length of the string is odd => it should be rejected */
      [leftZero]: [reject, leftZero, stay],
      [leftOne]: [reject, leftOne, stay],
      [zero]: [flse, rightZero, left],
      [one]: [flse, rightOne, left],
    },
    // find left side end
    flse: {
      [zero]: [flse, zero, left],
      [one]: [flse, one, left],
      [leftZero]: [start, leftZero, right],
      [leftOne]: [start, leftOne, right],
    },
    // move to the left (beginning)
    mvl: {
      [leftZero]: [mvl, leftZero, left],
      [leftOne]: [mvl, leftOne, left],
      [rightZero]: [mvl, rightZero, left],
      [rightOne]: [mvl, rightOne, left],
      [zero]: [mvl, zero, left],
      [one]: [mvl, one, left],
      [blank]: [v, blank, right],
    },
    // validate
    v: {
      [zero]: [v, zero, right],
      [one]: [v, one, right],
      [leftZero]: [v0, zero, right],
      [leftOne]: [v1, one, right],
      [blank]: [accept, blank, stay],
    },
    // validate that next right number is a zero
    v0: {
      [zero]: [v0, zero, right],
      [one]: [v0, one, right],
      [leftZero]: [v0, leftZero, right],
      [leftOne]: [v0, leftOne, right],
      [rightZero]: [mvl, zero, left],
      [rightOne]: [reject, rightOne, stay],
    },
    // validate that next right number is a one
    v1: {
      [zero]: [v1, zero, right],
      [one]: [v1, one, right],
      [leftZero]: [v1, leftZero, right],
      [leftOne]: [v1, leftOne, right],
      [rightZero]: [reject, rightZero, stay],
      [rightOne]: [mvl, one, left],
    },
  },
};

transform(tm, path.join(__dirname, "tandem.out"));
