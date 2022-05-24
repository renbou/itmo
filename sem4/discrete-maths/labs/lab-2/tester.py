#!/usr/bin/python3
from dataclasses import dataclass
import sys

class ParseError(Exception):
  def __init__(self, line_i: int, line: str, message: str):
    super().__init__(message)
    self.line_i = line_i
    self.line = line
  
  def __str__(self):
    return f"{super().args[0]} (line {self.line_i}): {self.line}"

class TuringMachine:
  directions = {"<": -1, "^": 0, ">": 1}
  info = {"accept": "AC", "reject": "RJ", "start": "S", "blank": "_"}

  def __init__(self, path: str):
    self.info = TuringMachine.info.copy()
    self.graph = dict()
    self.ntapes = 1
    nodes = set()

    with open(path, "r") as tm_desc:
      for line_i, line in enumerate(tm_desc):
        tokens = line.split()

        match (line_i, tokens):
          case (_, []):
            continue
          case (0, [ntapes]):
            try:
              self.ntapes = int(ntapes)
              continue
            except ValueError:
              raise ParseError(line_i, line, "parsing number of tapes failed")

        match tokens:
          case ["accept:"|"reject:"|"start:"|"blank:" as info_token, value]:
            self.info[info_token[:-1]] = value
          case _:
            if len(tokens) != 3 + self.ntapes * 3:
              raise ParseError(line_i, line, f"expected {3 + self.ntapes * 3} tokens")
            elif tokens[self.ntapes + 1] != "->":
              raise ParseError(line_i, line, f"expected -> as {self.ntapes + 1}'th token")
            
            from_node, to_node = tokens[0], tokens[self.ntapes + 2]
            via_char = tuple(tokens[1:self.ntapes + 1])
            new_char, moves = [], []
            for i in range(self.ntapes + 3, len(tokens), 2):
              new_char.append(tokens[i])
              moves.append(TuringMachine.parse_direction(tokens[i + 1]))
            
            self.graph[(from_node, via_char)] = (to_node, tuple(new_char), tuple(moves))
            nodes.add(from_node)

    for transition in self.graph:
      to_node = self.graph[transition][0]
      if to_node != self.info["reject"] and to_node != self.info["accept"] and not to_node in nodes:
        raise ParseError(f"There are edges leading to node {to_node}, but none leaving it")

  @staticmethod
  def parse_direction(direction: str):
    parsed = TuringMachine.directions.get(direction)
    if parsed == None:
      raise ValueError(f"{direction} is not a correct direction")
    return parsed

def read_tm(path):
  def parse_direction(direction):
    directions = {"<": -1, "^": 0, ">": 1}
    if direction in directions:
      return directions[direction]
    raise ValueError(f"{direction} is not a correct direction")

  info = {"accept": "AC", "reject": "RJ", "start": "S", "blank": "_"}
  graph = dict()
  nodes = set()

  with open(path, "r") as tm_desc:
    for line in tm_desc:
      tokens = line.split()

      # Skip blank lines
      if len(tokens) == 0:
        continue

      # Parse info if present
      if tokens[0] in ["start:", "accept:", "reject:", "blank:"]:
        info_token = tokens[0][:-1]
        # Sanity check
        if len(tokens) != 2:
          raise ValueError(f"Invalid info token \"{info_token}\" on line: \"{line}\"")
        info[info_token] = tokens[1]
        continue

      # Sanity checks
      if tokens[2] != "->":
        raise ValueError(f"Invalid line (not info and doesn't contain ->): \"{line}\"")
      elif len(tokens) != 6:
        raise ValueError(f"Invalid number of tokens on line (expected 6): \"{line}\"")

      from_node, via_char = tokens[:2]
      to_node, new_char, direction = tokens[3:]
      
      graph[(from_node, via_char)] = (to_node, new_char, parse_direction(direction))
      nodes.add(from_node)

  for transition in graph:
    to_node = transition[0]
    if to_node != info["reject"] and to_node != info["accept"] and not to_node in nodes:
      raise ValueError(f"There are edges leading to node {to_node}, but none leaving it")
  return info, graph

def execute_tm(info, graph, tape):
  accept, reject, start, blank = info["accept"], info["reject"], info["start"], info["blank"]
  carriage_position = 0
  node = start
  tape = dict(enumerate(tape))

  def tape_char(position):
    nonlocal tape
    if position in tape:
      return tape[position]
    tape[position] = blank
    return blank

  def tm_step():
    nonlocal carriage_position, node
    ch = tape_char(carriage_position)
    if (node, ch) not in graph:
      raise ValueError(f"Transition ({node}, {ch}) not in graph")

    to_node, new_char, direction = graph[(node, ch)]
    node = to_node
    tape[carriage_position] = new_char
    carriage_position += direction

  steps = 0
  while node != accept:
    tm_step()
    steps += 1

  result = ""
  i = -1
  while (carriage_position + i) in tape:
    result += tape[carriage_position + i]
    i -= 1
  result = result[::-1]
  i = 0
  while (carriage_position + i) in tape:
    result += tape[carriage_position + i]
    i += 1
  return (result, steps)

def main():
  tm = TuringMachine(sys.argv[1])
  # info, graph = read_tm(sys.argv[1])
  # tape = sys.argv[2]
  # result, steps = execute_tm(info, graph, tape)
  # result = result.strip(info["blank"])
  # print(f"got {result} in {steps} steps")

def test():
  info, graph = read_tm(sys.argv[1])

  import math
  for a in range(1, 31):
    tape = f"{bin(a)[2:]}"
    result, _ = execute_tm(info, graph, tape)
    result = result.strip(info["blank"])
    factorial = bin(math.factorial(a))[2:]
    if factorial != result:
      print(f"{a} expected {factorial} but got {result}")

  # for a in range(1, 100):
  #   for b in range(1, 100):
  #     tape = f"{bin(a)[2:]}*{bin(b)[2:]}"
  #     result, _ = execute_tm(info, graph, tape)
  #     result = result.strip(info["blank"])
  #     if f"{bin(a * b)[2:]}*{bin(b)[2:]}" != result:
  #       print(f"{a}*{b} expected {bin(a * b)[2:]}*{bin(b)[2:]} but got {result}")
  
if __name__ == '__main__':
  main()
