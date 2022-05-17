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

const op = "op";
const cp = "cp";
const v = "v";
const open = "(";
const close = ")";
const marker = "#";
const tm: TuringMachine<"op" | "cp" | "v", "(" | ")" | "#"> = {
  nodes: {
    s: {
      [blank]: [accept, blank, stay],
      [open]: [op, open, stay],
      [close]: [reject, blank, stay],
    },
    // skip opening parentheses, consume closing
    op: {
      [open]: [op, open, right],
      [close]: [cp, marker, left],
      [marker]: [op, marker, right],
      [blank]: [v, blank, left],
    },
    // skip closing parentheses, consume opening
    cp: {
      [open]: [op, marker, right],
      [marker]: [cp, marker, left],
      /* No paired opening parentheses were found */
      [blank]: [reject, blank, stay],
    },
    // validate
    v: {
      /* Check that all pairs of open-close parentheses were replaced by markers */
      [marker]: [v, blank, left],
      [blank]: [accept, blank, stay],
      /* No paired closing parentheses were found */
      [open]: [reject, blank, stay],
    },
  },
};

transform(tm, path.join(__dirname, "balanced.out"));
