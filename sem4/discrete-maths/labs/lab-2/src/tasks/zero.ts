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

const n = "n";
const tm: TuringMachine<"n", "0" | "1"> = {
  nodes: {
    s: {
      _: [accept, blank, stay],
      0: [n, blank, right],
    },
    n: {
      0: [start, blank, right],
      _: [reject, blank, stay],
    },
  },
};

transform(tm, path.join(__dirname, "zero.out"));
