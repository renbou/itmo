import * as fs from "fs";

const left = "<";
const stay = "^";
const right = ">";
type Direction = typeof left | typeof stay | typeof right;

const start = "s";
const accept = "ac";
const reject = "rj";
const blank = "_";

type TuringMachine<
  NodesDecl extends keyof any,
  CharsDecl extends keyof any,
  Start extends keyof any = typeof start,
  Accept extends keyof any = typeof accept,
  Reject extends keyof any = typeof reject,
  Blank extends string = typeof blank,
  Node extends keyof any = NodesDecl | Start | Accept | Reject,
  Char extends keyof any = CharsDecl | Blank
> = {
  info?: {
    start?: Start;
    accept?: Accept;
    reject?: Reject;
    blank?: Blank;
  };
  nodes: {
    [node in Node]?: {
      [char in Char]?: [Node, Char, Direction];
    };
  };
};

function transform<
  NodesDecl extends keyof any,
  CharsDecl extends keyof any,
  Start extends keyof any = typeof start,
  Accept extends keyof any = typeof accept,
  Reject extends keyof any = typeof reject,
  Blank extends string = typeof blank,
  Node extends keyof any = NodesDecl | Start | Accept | Reject,
  Char extends keyof any = CharsDecl | Blank
>(
  tm: TuringMachine<
    NodesDecl,
    CharsDecl,
    Start,
    Accept,
    Reject,
    Blank,
    Node,
    Char
  >,
  out: string
) {
  const newline = "\n";
  let description = Object.entries({
    ...{ start, accept, reject, blank },
    ...tm.info,
  })
    .map(([k, v]) => `${k}: ${String(v)}`)
    .join(newline);

  description +=
    newline +
    Object.entries(tm.nodes)
      .flatMap(([name, definition]) =>
        Object.entries(<{}>definition).map(
          ([char, transition]) =>
            `${name} ${char} -> ${(<[Node, Char, Direction]>transition)
              .map(String)
              .join(" ")}`
        )
      )
      .join(newline);

  fs.writeFileSync(out, description);
}

export type { TuringMachine, Direction };
export { left, stay, right, start, accept, reject, blank };
export default transform;
